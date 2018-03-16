#include "graphic.h"
#include "oled.h"
#include "QR_Encode.h"      //二维码显示库

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
void OLED_DrawPoint(u8 x, u8 y)
{
    u8 pos, bx, temp = 0;
    if(x > 127 || y > 63)return; //超出范围了.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(1)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}
//画点，二维码
void OLED_DrawPoint_QRcode(u8 x, u8 y, u8 dot)
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
void OLED_FillArea(u8 x1, u8 y1, u8 x2, u8 y2)
{
    u8 x, y;
    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)
            OLED_DrawPoint(x, y);
    }
}
//画直线
//起点坐标,终点坐标,注意左下角为原点
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2)
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
        OLED_DrawPoint(uRow, uCol); //画点
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
//(x1,y1)左下角坐标,(x2,y2)右上角坐标
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    //画四条直线
    OLED_DrawLine(x1, y1, x2, y1);
    OLED_DrawLine(x1, y1, x1, y2);
    OLED_DrawLine(x1, y2, x2, y2);
    OLED_DrawLine(x2, y1, x2, y2);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
//使用Bresenham算法画圆
void OLED_DrawCircle(u16 x0, u16 y0, u8 r)
{
    OLED_DrawArc(x0, y0, r, 1);
    OLED_DrawArc(x0, y0, r, 2);
    OLED_DrawArc(x0, y0, r, 3);
    OLED_DrawArc(x0, y0, r, 4);
   
    /*
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);         //判断下个点位置的标志
    while(a <= b)
    {
        OLED_DrawPoint(x0 + a, y0 - b);       //5
        OLED_DrawPoint(x0 + b, y0 - a);       //0
        OLED_DrawPoint(x0 + b, y0 + a);       //4
        OLED_DrawPoint(x0 + a, y0 + b);       //6
        OLED_DrawPoint(x0 - a, y0 + b);
        OLED_DrawPoint(x0 - b, y0 + a);
        OLED_DrawPoint(x0 - a, y0 - b);
        OLED_DrawPoint(x0 - b, y0 - a);
        a++;
        //使用Bresenham算法画圆
        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
    */
}


//画三角形，三个坐标点
void OLED_DrawTriangle(u8 x0,u8 y0,u8 x1,u8 y1, u8 x2,u8 y2)
{
    OLED_DrawLine(x0, y0, x1, y1);
    OLED_DrawLine(x1, y1, x2, y2);
    OLED_DrawLine(x2, y2, x0, y0);    
}

//画竖直的线段，起点和长度,长度为负，向下画
void OLED_DrawVLine(u8 x0, u8 y0,int length)
{
    OLED_DrawLine(x0,y0,x0,y0+length);   
}
//画水平的线段
void OLED_DrawHLine(u8 x0, u8 y0,int length)
{
    OLED_DrawLine(x0,y0,x0+length,y0);   
}
//画1/4圆弧，圆心，半径，位置，按照象限1234
void OLED_DrawArc(u16 x0, u16 y0, u8 r, u8 part)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);         //判断下个点位置的标志
    while(a <= b)
    {
        switch(part)
        {
            case 1:
                OLED_DrawPoint(x0 + b, y0 + a);   
                OLED_DrawPoint(x0 + a, y0 + b);      
                break;
            case 2:
                OLED_DrawPoint(x0 - a, y0 + b);
                OLED_DrawPoint(x0 - b, y0 + a);
                break;
            case 3:
                OLED_DrawPoint(x0 - a, y0 - b);
                OLED_DrawPoint(x0 - b, y0 - a);
                break;
            case 4:
                OLED_DrawPoint(x0 + a, y0 - b);    
                OLED_DrawPoint(x0 + b, y0 - a);  
                break;
            default:
                break;
        }
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

//画圆角矩形，坐标和圆角半径
void OLED_DrawRRectangle(u8 x0, u8 y0, u8 x1, u8 y1, u8 R)
{
    u8 L,W;
    L = x1 - x0 - 2 * R;
    W = y1 - y0 - 2 * R;
    OLED_DrawHLine(x0 + R, y0, L);
    OLED_DrawHLine(x0 + R, y1, L);
    OLED_DrawVLine(x0, y0 + R, W);
    OLED_DrawVLine(x1, y0 + R, W);
    
    OLED_DrawArc(x1 - R, y1 - R, R, 1);
    OLED_DrawArc(x0 + R, y1 - R, R, 2);
    OLED_DrawArc(x0 + R, y0 + R, R, 3);
    OLED_DrawArc(x1 - R, y0 + R, R, 4);
}
//显示二维码，网站长度不能太长，二维码只支持支付宝扫码
void OLED_DrawQRCode(char *website)
{
//  MAX_MODULESIZE = 33;
//  m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE];
    u8 i,j;
    EncodeData(website);
    for(i = 0; i < MAX_MODULESIZE; i++)
    {
        for(j = 0; j < MAX_MODULESIZE; j++)
        {
            if(m_byModuleData[i][j] == 1)
            {
            //2倍放大，1个点放大为4个点
                OLED_DrawPoint(2*i, 2*j);
                OLED_DrawPoint(2*i, 2*j+1);        
                OLED_DrawPoint(2*i+1, 2*j);  
                OLED_DrawPoint(2*i+1, 2*j+1);
//            OLED_DrawPoint_QRcode(2*i, 2*j, m_byModuleData[i][j]);
//            OLED_DrawPoint_QRcode(2*i, 2*j+1, m_byModuleData[i][j]);        
//            OLED_DrawPoint_QRcode(2*i+1, 2*j, m_byModuleData[i][j]);  
//            OLED_DrawPoint_QRcode(2*i+1, 2*j+1, m_byModuleData[i][j]);
            }   
        }
    }    
}
    

