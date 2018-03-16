#include "wdg.h"
#include "led.h"

//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG->KR=0X5555;//使能对IWDG->PR和IWDG->RLR的写		 										  
  	IWDG->PR=prer;  //设置分频系数   
  	IWDG->RLR=rlr;  //从加载寄存器 IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//使能看门狗	
}
//喂独立看门狗
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}
//保存WWDG计数器的设置值,默认为最大. 
u8 WWDG_CNT=0x7f; 
//初始化窗口看门狗 	
//tr   :T[6:0],计数器值 
//wr   :W[6:0],窗口值 
//fprer:分频系数（WDGTB）,仅最低2位有效 
//Fwwdg=PCLK1/(4096*2^fprer). 
void WWDG_Init(u8 tr,u8 wr,u8 fprer) 
{    
	RCC->APB1ENR|=1<<11; 	//使能wwdg时钟 
	WWDG_CNT=tr&WWDG_CNT;   //初始化WWDG_CNT.     
	WWDG->CFR|=fprer<<7;    //PCLK1/4096再除2^fprer 
	WWDG->CFR&=0XFF80;      
	WWDG->CFR|=wr;     		//设定窗口值      
	WWDG->CR|=WWDG_CNT; 	//设定计数器值 
	WWDG->CR|=1<<7;  		//开启看门狗      
	MY_NVIC_Init(2,3,WWDG_IRQChannel,2);//抢占2，子优先级3，组2     
	WWDG->SR=0X00; 			//清除提前唤醒中断标志位 
	WWDG->CFR|=1<<9;        //使能提前唤醒中断 
} 
//重设置WWDG计数器的值 
void WWDG_Set_Counter(u8 cnt) 
{ 
	WWDG->CR =(cnt&0x7F);//重设置7位计数器 
} 
//窗口看门狗中断服务程序 
void WWDG_IRQHandler(void) 
{      
	WWDG_Set_Counter(WWDG_CNT);//重设窗口看门狗的值!         
	WWDG->SR=0X00;//清除提前唤醒中断标志位 
	LED1=!LED1; 
}















