#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "oledfont.h"
#include "graphic.h"

/*
	OLED_SCL - PE7
	OLED_SDIN - PE8
	OLED_RST - PE9
	OLED_DC - PE10
	OLED_CS - GND
*/

//默认6x8字体,行列坐标,x：0-20，y：0-7,可显示：21*8=168个字符
//默认8x16字体,行列坐标显示,x：0-15，y：0-3，可显示：16*4=64个字符
//可根据需要修改为像素坐标显示，x:0-127,y:0-63
//可设置8*16字体反显,闪烁字符数字均可,2017年5月12日11:10:19增加
#define OLED_SCL PEout(7)
#define OLED_SDA PEout(8)
#define OLED_RST PEout(9)
#define OLED_DC PEout(10)

#define OLED_DB GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10

void OLED_GPIO_Config(void)
{
    GPIO_InitTypeDef IO_Init;
	IO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	IO_Init.GPIO_Pin = OLED_DB;
	IO_Init.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOE, &IO_Init);
}

void OLED_WriteCmd(u8 cmd)
{
    u8 i = 8;
    OLED_DC = 0;
    OLED_SCL = 0;
    for(i = 0; i < 8; i++)
    {
        if(cmd & 0x80)
            OLED_SDA = 1;
        else
            OLED_SDA = 0;
        OLED_SCL = 1;
        OLED_SCL = 0;
        cmd <<= 1;;
    }
}

void OLED_WriteData(u8 dat)		//写一字节数据
{
    u8 i = 8;
    OLED_DC = 1;
    OLED_SCL = 0;
    for(i = 0; i < 8; i++)
    {
        if(dat & 0x80)
            OLED_SDA = 1;
        else
            OLED_SDA = 0;
        OLED_SCL = 1;
        OLED_SCL = 0;
        dat <<= 1;
    }
}

void OLED_WriteData2(u8 dat)	//反显数据
{
    u8 i = 8;
    OLED_DC = 1;
    OLED_SCL = 0;
    for(i = 0; i < 8; i++)
    {
        if(dat & 0x80)
            OLED_SDA = 0;
        else
            OLED_SDA = 1;
        OLED_SCL = 1;
        OLED_SCL = 0;
        dat <<= 1;
    }
}

void OLED_DisON(void)
{
    OLED_WriteCmd(0x8d);		//电荷泵设置
    OLED_WriteCmd(0x14);		//开启电荷泵
    OLED_WriteCmd(0xaf);		//显示开
}

void OLED_DisOFF(void)
{
    OLED_WriteCmd(0x8d);		//电荷泵设置
    OLED_WriteCmd(0x10);		//关闭电荷泵
    OLED_WriteCmd(0xae);		//显示关
}

void OLED_SetCursor(u8 x, u8 y)
{
    OLED_WriteCmd(0xb0 + (y >> 3));
//    OLED_WriteCmd(0xb0 + y);
    OLED_WriteCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Clear(void)		//全部是黑色的.没有点亮
{
    u8 i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_WriteCmd (0xb0 + i); //设置页地址（0~7）
        OLED_WriteCmd (0x00);     //设置显示位置—列低地址
        OLED_WriteCmd (0x10);     //设置显示位置—列高地址
        for(n = 0; n < 128; n++)	//每行128个点
            OLED_WriteData(0);		//熄灭像素点
    }
}
//设置对比度
void OLED_SetContrast(u8 con)
{
    OLED_WriteCmd(0x81);	 //对比度设置
    OLED_WriteCmd(con); 	 //1~255;默认0X7F (亮度设置,越大越亮)
}



void OLED_Init(void)
{
    OLED_RST = 1;
    delay_ms(100);
    OLED_RST = 0;
    delay_ms(100);
    OLED_RST = 1;
    /*官方代码，显示正常*/
	/*
//	  OLED_WriteCmd(0xAE); //--turn off oled panel
//    OLED_WriteCmd(0x00); //---set low column address
//    OLED_WriteCmd(0x10); //---set high column address
//    OLED_WriteCmd(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//    OLED_WriteCmd(0x81); //设置对比度
//    OLED_WriteCmd(0xCF); // Set SEG Output Current Brightness
//    OLED_WriteCmd(0xA1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//    OLED_WriteCmd(0xC8); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//    OLED_WriteCmd(0xA6); //--set normal display
//    OLED_WriteCmd(0xA8); //--set multiplex ratio(1 to 64)
//    OLED_WriteCmd(0x3f); //--1/64 duty
//    OLED_WriteCmd(0xD3); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//    OLED_WriteCmd(0x00); //-not offset
//    OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
//    OLED_WriteCmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
//    OLED_WriteCmd(0xD9); //--set pre-charge period
//    OLED_WriteCmd(0xF1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    OLED_WriteCmd(0xDA); //--set com pins hardware configuration
//    OLED_WriteCmd(0x12);
//    OLED_WriteCmd(0xDB); //--set vcomh
//    OLED_WriteCmd(0x40); //Set VCOM Deselect Level
//    OLED_WriteCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
//    OLED_WriteCmd(0x02); //
//    OLED_WriteCmd(0x8D); //--set Charge Pump enable/disable
//    OLED_WriteCmd(0x14); //--set(0x10) disable
//    OLED_WriteCmd(0xA4); // Disable Entire Display On (0xa4/0xa5)
//    OLED_WriteCmd(0xA6); // Disable Inverse Display On (0xa6/a7)
//    OLED_WriteCmd(0xAF); //--turn on oled panel
//    OLED_WriteCmd(0xAF); //display ON


*/
    /*原子视频代码,显示正常*/
    OLED_WriteCmd(0xAE); //关闭显示
    OLED_WriteCmd(0xD5); //设置时钟分频因子,震荡频率
    OLED_WriteCmd(0x80);  //[3:0],分频因子;[7:4],震荡频率
    OLED_WriteCmd(0xA8); //设置驱动路数
    OLED_WriteCmd(0X3F); //默认0X3F(1/64)
    OLED_WriteCmd(0xD3); //设置显示偏移
    OLED_WriteCmd(0X00); //默认为0
    OLED_WriteCmd(0x40); //设置显示开始行 [5:0],行数.
    OLED_WriteCmd(0x8D); //电荷泵设置
    OLED_WriteCmd(0x14); //bit2，开启/关闭
    OLED_WriteCmd(0x20); //设置内存地址模式
    OLED_WriteCmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WriteCmd(0xA1); //0xa0左右反置 0xa1正常
    OLED_WriteCmd(0xC8); //0xc0上下反置 0xc8正常
    OLED_WriteCmd(0xDA); //设置COM硬件引脚配置
    OLED_WriteCmd(0x12); //[5:4]配置
    OLED_WriteCmd(0x81); //对比度设置
    OLED_WriteCmd(0x7f); //1~255;默认0X7F--------(亮度设置,越大越亮)
    OLED_WriteCmd(0xD9); //设置预充电周期
    OLED_WriteCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WriteCmd(0xDB); //设置VCOMH 电压倍率
    OLED_WriteCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WriteCmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WriteCmd(0xA6); //设置显示方式;bit0:1-0xa7反相显示;0xa6正常显示
    OLED_WriteCmd(0xAF); //开启显示

    OLED_Clear();
    OLED_SetCursor(0, 0);
}


void Display_LOGO(void)
{
    u8 n;
    OLED_DrawBMP(0, 0, 128, 8, IPHONE);
	
    OLED_DisString_F8X16(0, 3, "Starting...");
    for( n = 5; n > 0; n--)
    {
        OLED_DisChar_F8X16(12, 3, n + 0x30);
        delay_ms(800);
    }
    OLED_Clear();
    delay_ms(800);
    OLED_DrawBMP(0, 0, 128, 2, BAR);
    delay_ms(1000);
}

//显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMPx[])
{
    u16 j = 0;
    u8 x, y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for(y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for(x = x0; x < x1; x++)
        {
            OLED_WriteData(BMPx[j++]);
        }
    }
}
void OLED_Set_Pos(u8 x, u8 y)	 //汉字显示，设置显示坐标
{
    OLED_WriteCmd(0xb0 + y);
    OLED_WriteCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WriteCmd((x & 0x0f) | 0x01);
}
void OLED_DisChinese(u8 x, u8 y, u8 Num)
{
    u8 i;
    OLED_Set_Pos(x, y);
    for(i = 0; i < 16; i++)
        OLED_WriteData(CHN[2 * Num][i]);		//汉字编码的第一行，
    OLED_Set_Pos(x, y + 1);
    for(i = 0; i < 16; i++)
        OLED_WriteData(CHN[2 * Num + 1][i]);	//汉字编码的第二行
}
#if 1		//8x16字体,行列坐标显示,x：0-15，y：0-3

//显示字符,8*16字体,x：0-15，y：0-3
void OLED_DisChar_F8X16(u8 x, u8 y, u8 ch)
{
    u8 c, i;
    c = ch - ' ';
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)					//显示上半部分8*8
        OLED_WriteData(F8X16[c][i]);		//每一行的前8个
    OLED_SetCursor(x * 8, y * 16 + 8);			//显示下半部分8*8
    for(i = 0; i < 8; i++)
        OLED_WriteData(F8X16[c][i + 8]);	//每一行的后8个
}
//显示8*16字符反显,x：0-15，y：0-3
void OLED_CharInverse_F8X16(u8 x, u8 y, u8 ch)
{
    u8 c, i;
    c = ch - ' ';
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(F8X16[c][i]);
    OLED_SetCursor(x * 8, y * 16 + 8);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(F8X16[c][i + 8]);
}
//显示8x16字符闪烁
void OLED_CharBlink_F8X16(u8 x, u8 y, u8 ch)
{
    OLED_CharInverse_F8X16(x, y, ch);
    delay_ms(400);
    OLED_DisChar_F8X16(x, y, ch);
    delay_ms(400);
}
//显示两位数字，8X16字体，x：0-14，y：0-3
void OLED_DisNumber_F8X16(u8 x, u8 y, u8 num)
{
    //调用字符函数显示
    OLED_DisChar_F8X16(x, y, num / 10 + 0x30);
    OLED_DisChar_F8X16(x + 1, y, num % 10 + 0x30);
}
//显示两位数字反显,8*16字体,x：0-14，y：0-3
void OLED_NumberInverse_F8X16(u8 x, u8 y, u8 num)
{
    OLED_CharInverse_F8X16(x, y, num / 10 + 0x30);
    OLED_CharInverse_F8X16(x + 1, y, num % 10 + 0x30);
}
//显示两位数字闪烁:正常和反显交替
void OLED_NumberBlink_F8X16(u8 x, u8 y, u8 num)
{
    OLED_NumberInverse_F8X16(x, y, num);
    delay_ms(100);
    OLED_DisNumber_F8X16(x, y, num);
    delay_ms(100);
}
//显示字符串,8X16字体，x：0-15，y：0-3
void OLED_DisString_F8X16(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F8X16(x, y, *str++);
        x++;
    }
}
//指定位置显示8*16黑块字符
void OLED_DisBlack_F8X16(u8 x, u8 y)		//指定位置显示字符黑块
{
    u8 i;
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)
        OLED_WriteData(0);			//熄灭像素点
    OLED_SetCursor(x * 8, y * 16 + 8);
    for(i = 0; i < 8; i++)
        OLED_WriteData(0);
}
//指定位置显示8*16白块字符
void OLED_DisWhite_F8X16(u8 x, u8 y)		//指定位置显示字符白块
{
    u8 i;
    OLED_SetCursor(x * 8, y * 16);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(0);			//点亮像素点
    OLED_SetCursor(x * 8, y * 16 + 8);
    for(i = 0; i < 8; i++)
        OLED_WriteData2(0);
}
#endif


#if 0		//8x16字体,像素坐标显示,x：0-127，y：0-63

/*显示字符,8X16字体,x：0-127，y：0-63*/
void OLED_DisChar_F8X16(u8 x, u8 y, u8 ch)
{
    u8 c, i;
    c = ch - ' ';		//地址偏移
    OLED_SetCursor(x, y);
    for(i = 0; i < 8; i++)		//显示上半部分8*8
        OLED_WriteData(F8X16[c][i]);			//每一行的前8个
    OLED_SetCursor(x, y + 8);		//显示下半部分8*8
    for(i = 0; i < 8; i++)
        OLED_WriteData(F8X16[c][i + 8]);		//每一行的后8个
}
/*显示字符串,8X16字体，x：0-15，y：0-3*/
void OLED_DisString_F8X16(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F8X16(x, y, *str++);
        x += 8;
    }
}
/*显示两位数字，8X16字体，x：0-14，y：0-3*/
void OLED_DisNumber_F8X16(u8 x, u8 y, u8 num)
{
    //调用字符函数显示
    OLED_DisChar_F8X16(x, y, num / 10 + 0x30);
    OLED_DisChar_F8X16(x + 8, y, num % 10 + 0x30);
}

#endif





#if 1		//6x8字体,行列坐标,x：0-20，y：0-7

/*显示字符：6X8字体，x：0-20，y：0-7*/
void OLED_DisChar_F6X8(u8 x, u8 y, u8 ch)
{
    u8 c = 0, i = 0;
    c = ch - ' ';
    OLED_SetCursor(x * 6, y * 8);
    for(i = 0; i < 6; i++)
        OLED_WriteData(F6X8[c][i]);
}
/*显示字符串，6X8字体,长度不能超过21个字符*/
void OLED_DisString_F6X8(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F6X8(x, y, *str++);
        x++;
    }
}
/*显示两位数字,6X8字体,x：0-19，y:0-7*/
void OLED_DisNumber_F6X8(u8 x, u8 y, u8 num)
{
    //调用字符函数显示
    OLED_DisChar_F6X8(x, y, num / 10 + 0x30);
    OLED_DisChar_F6X8(x + 1, y, num % 10 + 0x30);
}

#endif

#if 0		//6x8字体,像素坐标显示,x：0-127，y：0-63

/*显示字符：6X8字体，x：0-127，y：0-36*/
void OLED_DisChar_F6X8(u8 x, u8 y, u8 ch)
{
    u8 c = 0, i = 0;
    c = ch - ' ';
    OLED_SetCursor(x, y);
    for(i = 0; i < 6; i++)
        OLED_WriteData(F6X8[c][i]);
}
/*显示字符串，6X8字体,长度不能超过21个字符*/
void OLED_DisString_F6X8(u8 x, u8 y, u8 *str)
{
    while (*str != '\0')
    {
        OLED_DisChar_F6X8(x, y, *str++);
        x += 6;
    }
}
/*显示两位数字,6X8字体,x：0-19，y:0-7*/
void OLED_DisNumber_F6X8(u8 x, u8 y, u8 num)
{
    //调用字符函数显示
    OLED_DisChar_F6X8(x, y, num / 10 + 0x30);
    OLED_DisChar_F6X8(x + 6, y, num % 10 + 0x30);
}

#endif

