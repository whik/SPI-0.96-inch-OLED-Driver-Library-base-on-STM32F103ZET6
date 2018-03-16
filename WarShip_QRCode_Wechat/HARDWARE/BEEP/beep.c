#include "beep.h"

//初始化PB8为输出口.并使能这个口的时钟		    
//蜂鸣器初始化
void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	   	  
	GPIOB->CRH&=0XFFFFFFF0; 
	GPIOB->CRH|=0X00000003;	//PB.8 推挽输出    
	BEEP=0;					//关闭蜂鸣器输出
}






