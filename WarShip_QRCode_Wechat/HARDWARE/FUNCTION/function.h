#ifndef __FUNCTION_H
#define __FUNCTION_H
//#include "sys.h"
#include "stm32f10x_type.h"

extern const u8 HEXBITTable[16];
extern const u8 ConfigValTime[4];
void PrintF_Information(u8 error0);
void PrintF_Information16(u16 error0);
//void memcpy(u8 *da,u8 *db,u16 len);
//void memmove(u8 *da,u8 *db,u16 len);
//void memset(u8 *da,u8 db,u16 len);
void HexToAsc(u8 hex,u8 *asc);
void HexPtrAsc(u8 *hex,u8 *asc,u16 len);
void BcdToAsc(u8 bcd,u8 *asc);
u8 AscToHex(u8 *asc);
void AscPtrHex(u8 *asc,u8 *hex,u16 len);
u8 Hex_Asc(u8 hex);
u8 Asc_Hex(u8 asc);
u8 Bcd_Hex(u8 bcd);
u8 Hex_Bcd(u8 hex);

u8 memcmpstr(u8 *da,u8 *db,u16 len);
u8 memcmpone(u8 *da,u8 db,u16 len);
u8 memcmpnum(u8 *da,u8 db,u16 len);
u8 onecmpstr(u8 da,u8 *db,u16 len);

u16 BytesToWord(u8 *bytes);

#endif
