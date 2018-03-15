#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__
#include "stm32f10x.h"

void OLED_RefreshGRAM(void);
void OLED_DrawPoint(u8 x, u8 y,u8 dot);
void OLED_FillArea(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2,u8 dot);
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u8 dot);
void OLED_DrawCircle(u16 x0,u16 y0,u8 r,u8 dot);

#endif


