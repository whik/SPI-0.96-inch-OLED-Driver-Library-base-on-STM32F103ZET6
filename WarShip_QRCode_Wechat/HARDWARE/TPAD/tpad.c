#include "tpad.h"

#define TPAD_ARR_MAX_VAL 	0XFFFF	//最大的ARR值	  
vu16 tpad_default_val=0;//空载的时候(没有手按下),计数器需要的时间
//初始化触摸按键
//获得空载的时候触摸按键的取值.
//systick:系统时钟频率
//返回值:0,初始化成功;1,初始化失败
u8 TPAD_Init(u8 systick)
{
	u16 buf[10];
	u16 temp;
	u8 j,i;
	TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,systick-1);//以1Mhz的频率计数 
	for(i=0;i<10;i++)//连续读取10次
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)//排序
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];//取中间的8个数据进行平均
	tpad_default_val=temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;//初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
	return 0;		     	    					   
}
//复位一次
//释放电容电量，并清除定时器的计数值
void TPAD_Reset(void)
{				   
	GPIOA->CRL&=0XFFFFFF0F;	//PA1  输入  
	GPIOA->CRL|=0X00000030;	//复用功能输出       
	GPIOA->ODR&=~(1<<1);	//输出0,放电
	delay_ms(5);
	TIM5->SR=0;   			//清除标记
	TIM5->CNT=0;			//归零     
	GPIOA->CRL&=0XFFFFFF0F;	//PA1  输入  
	GPIOA->CRL|=0X00000040;	//复用功能输出       
}
//得到定时器捕获值
//如果超时,则直接返回定时器的计数值.
//返回值：捕获值/计数值（超时的情况下返回）
u16 TPAD_Get_Val(void)
{				   
	TPAD_Reset();
	while((TIM5->SR&0X02)==0)//等待捕获上升沿
	{
		if(TIM5->CNT>TPAD_ARR_MAX_VAL-500)return TIM5->CNT;//超时了,直接返回CNT的值
	};	
	return TIM5->CCR2;	  
} 	  
//读取n次,取最大值
//n：连续获取的次数
//返回值：n次读数里面读到的最大读数值
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//得到一次值
		if(temp>res)res=temp;
	};
	return res;
}  
//扫描触摸按键
//mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
//返回值:0,没有按下;1,有按下;										  
#define TPAD_GATE_VAL 	80	//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,可以开始检测;>0,还不能开始检测	 
	u8 res=0;
	u8 sample=3;		//默认采样次数为3次	 
	u16 rval;
	if(mode)
	{
		sample=6;	//支持连按的时候，设置采样次数为6次
		keyen=0;	//支持连按	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL))//大于tpad_default_val+TPAD_GATE_VAL,有效
	{						
  		rval=TPAD_Get_MaxVal(sample);		 
		if((keyen==0)&&(rval>(tpad_default_val+TPAD_GATE_VAL)))//大于tpad_default_val+TPAD_GATE_VAL,有效
		{
			res=1;
		}	   
		//printf("r:%d\r\n",rval);			     	    					   
		keyen=5;				//至少要再过5次之后才能按键有效   
	}else if(keyen>2)keyen=2; 	//如果检测到按键松开,则直接将次数将为2,以提高响应速度
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 
//定时器2通道2输入捕获配置	 
//arr：自动重装值
//psc：时钟预分频数
void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
	//此部分需手动修改 IO口设置  
 	RCC->APB1ENR|=1<<3;   	//TIM5 时钟使能 
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟 

	GPIOA->CRL&=0XFFFFFF0F;	//PA1  输入  
	GPIOA->CRL|=0X00000040;	//浮空输入     	  
	  
 	TIM5->ARR=arr;  		//设定计数器自动重装值//刚好1ms    
	TIM5->PSC=psc;  		//预分频器,1M的计数频率	 

	TIM5->CCMR1|=1<<8;		//CC2S=01 	选择输入端 IC2映射到TI2上
 	TIM5->CCMR1|=0<<12; 	//IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
 	TIM5->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 

	TIM5->CCER|=0<<5; 		//CC2P=0	上升沿捕获
	TIM5->CCER|=1<<4; 		//CC2E=1 	允许捕获计数器的值到捕获寄存器中
																 
	TIM5->CR1|=0x01;    	//使能定时器5
}













