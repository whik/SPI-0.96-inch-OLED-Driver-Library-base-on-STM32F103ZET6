#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
 
//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY3==1)	 //WK_UP按键
	{				 
		BEEP=!BEEP;	
	}		 
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	 //按键KEY2
	{
		LED0=!LED0;
	}		 
	EXTI->PR=1<<2;  //清除LINE2上的中断标志位  
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 
		LED1=!LED1;
	}		 
	EXTI->PR=1<<3;  //清除LINE3上的中断标志位  
}
//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0
	{
		LED0=!LED0;
		LED1=!LED1;
	}		 
	EXTI->PR=1<<4;  //清除LINE4上的中断标志位  
}		   
//外部中断初始化程序
//初始化PA0/PE2/PE3/PE4为中断输入.
void EXTIX_Init(void)
{
	KEY_Init();

	Ex_NVIC_Config(GPIO_A,0,RTIR); 			//上升沿触发
	Ex_NVIC_Config(GPIO_E,2,FTIR); 			//下降沿触发
	Ex_NVIC_Config(GPIO_E,3,FTIR); 			//下降沿触发
 	Ex_NVIC_Config(GPIO_E,4,FTIR); 			//下降沿触发

	MY_NVIC_Init(2,3,EXTI0_IRQChannel,2);	//抢占2，子优先级3，组2
	MY_NVIC_Init(2,2,EXTI2_IRQChannel,2);	//抢占2，子优先级2，组2	   
	MY_NVIC_Init(2,1,EXTI3_IRQChannel,2);	//抢占2，子优先级1，组2	   
	MY_NVIC_Init(2,0,EXTI4_IRQChannel,2);	//抢占2，子优先级0，组2	   
}












