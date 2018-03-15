#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__
#include "stm32f10x.h"

void OLED_RefreshGRAM(void);
void OLED_DrawPoint(u8 x, u8 y);
void OLED_FillArea(u8 x1, u8 y1, u8 x2, u8 y2);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void OLED_DrawCircle(u16 x0,u16 y0,u8 r);
void OLED_DrawTriangle(u8 x0,u8 y0,u8 x1,u8 y1, u8 x2,u8 y2);
void OLED_DrawVLine(u8 x0, u8 y0,int length);
void OLED_DrawHLine(u8 x0, u8 y0,int length);
void OLED_DrawArc(u16 x0, u16 y0, u8 r, u8 part);
void OLED_DrawRRectangle(u8 x0, u8 y0, u8 x1, u8 y1, u8 R);

#endif


