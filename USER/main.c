#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "graphic.h"      //绘制图形函数库
#include "QR_Encode.h"
#include "usart.h"	
/*
    3线SPI模式，单向传输
	OLED_SCL - PE7
	OLED_SDIN - PE8
	OLED_RST - PE9
	OLED_DC - PE10
	OLED_CS - GND
*/
int main(void)
{
    extern u8 BAR[];
    extern u8 STMMCU[];
    extern u8 RENESAS[];

    u8 i,j;  
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    LED_Init();			     //LED端口初始化
    KEY_Init();
    uart_init(9600);
    OLED_GPIO_Config();
    OLED_Init();
//    OLED_WriteCmd(0xa7);    //反向显示

    while(1)
    {
//        8x16字体显示测试
//        OLED_DisChar_F8X16(0,0,'A');
//        OLED_CharInverse_F8X16(0,1,'Z');
//        OLED_CharBlink_F8X16(1,0,'Z');
//        OLED_DisNumber_F8X16(0,2,99);
//        OLED_NumberInverse_F8X16(2,2,23);
//        OLED_NumberBlink_F8X16(2,0,34);
//        OLED_DisString_F8X16(0,3,"HELLO OLED!");
//        OLED_DisWhite_F8X16(2,1);

//        图片显示测试
//        OLED_DrawBMP(0,0,128,2,BAR);
//        OLED_DrawBMP(0,0,128,8,RENESAS);

//      6x8 字体测试
//        OLED_DisChar_F6X8(0,0,'Z');
//        OLED_DisNumber_F6X8(0,2,99);
//        OLED_DisString_F6X8(0,4,"HELLO STM32!");

//        开机界面测试
//        Display_LOGO();     //苹果图标，启动倒计时，仿手机状态栏
//        while(1);

//      绘制图形测试
//        OLED_DrawLine(0,0,120,50);           //画直线
//        OLED_DrawRectangle(90,0,120,20);      //画矩形
//        OLED_FillArea(0,40,30,80);         //填充矩形区域
//        OLED_DrawCircle(60,35,28);        //画圆
//        OLED_DrawTriangle(0,0,40,20,20,60);     //画三角形
//        OLED_DrawHLine(70,10,20);               //画水平线
//        OLED_DrawHLine(70,30,-20);
//        OLED_DrawVLine(70,20,20);           //画竖直线
//        OLED_DrawVLine(80,40,-20);
//        OLED_DrawArc(60,35,28,3);         //画1/4圆弧
//        OLED_DrawRRectangle(50,10,120,60,10);       //画圆角矩形
//        OLED_DrawQRCode("HELLO STM32 OLED QRCODE");     //只支持支付宝扫码
        OLED_DrawQRCode("https://github.com/whik");     //只支持支付宝扫码
        OLED_RefreshGRAM();             //更新显存，不更新不显示
        LED0 = !LED0;
        delay_ms(400);
    }

}

