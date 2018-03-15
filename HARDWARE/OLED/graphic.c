#include "graphic.h"
#include "oled.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

//绘图函数,左下角为原点坐标,横向x,纵向y

void OLED_RefreshGRAM(void)		//刷新整个屏幕12864,可根据需要调整
{
    u8 i, n;
    for(i = 0; i < 8; i++)		//页 0-7
    {
        OLED_WriteCmd(0xb0 + i); //设置页地址（0~7）
        OLED_WriteCmd(0x00);     //设置显示位置—列低地址
        OLED_WriteCmd(0x10);     //设置显示位置—列高地址
        for(n = 0; n < 128; n++)
            OLED_WriteData(OLED_GRAM[n][i]);
    }
}
//画点
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x, u8 y, u8 dot)
{
    u8 pos, bx, temp = 0;
    if(x > 127 || y > 63)return; //超出范围了.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(dot)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2,x1<=127 ,0<=y1<=63
//dot:0,清空;1,填充
void OLED_FillArea(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot)
{
    u8 x, y;
    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)
            OLED_DrawPoint(x, y, dot);
    }
}
//画直线
//起点坐标,终点坐标,注意左下角为原点
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if(delta_x > 0)
        incx = 1; 			//设置单步方向
    else if(delta_x == 0)
        incx = 0; //垂直线
    else			//小于0
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)
        incy = 1;
    else if(delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if( delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;

    for(t = 0; t <= distance + 1; t++ ) //画线输出
    {
        OLED_DrawPoint(uRow, uCol, dot); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 dot)
{
    //画四条直线
    OLED_DrawLine(x1, y1, x2, y1, dot);
    OLED_DrawLine(x1, y1, x1, y2, dot);
    OLED_DrawLine(x1, y2, x2, y2, dot);
    OLED_DrawLine(x2, y1, x2, y2, dot);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
//使用Bresenham算法画圆
void OLED_DrawCircle(u16 x0, u16 y0, u8 r, u8 dot)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);         //判断下个点位置的标志
    while(a <= b)
    {
        OLED_DrawPoint(x0 + a, y0 - b, dot);       //5
        OLED_DrawPoint(x0 + b, y0 - a, dot);       //0
        OLED_DrawPoint(x0 + b, y0 + a, dot);       //4
        OLED_DrawPoint(x0 + a, y0 + b, dot);       //6
        OLED_DrawPoint(x0 - a, y0 + b, dot);
        OLED_DrawPoint(x0 - b, y0 + a, dot);
        OLED_DrawPoint(x0 - a, y0 - b, dot);
        OLED_DrawPoint(x0 - b, y0 - a, dot);
        a++;
        //使用Bresenham算法画圆
        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}




