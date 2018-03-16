#include "function.h"

const u8 ASCIITable[16] = 
{
	'0','1','2','3','4','5','6','7',
	'8','9','A','B','C','D','E','F',
};
const u8 HEXBITTable[16]=
{
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
	0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,
};
const u8 ConfigValTime[4]={0,0,0x23,0x59};
//将16进制数(0~f)转换成字符('0'~'F')
//例:hex=0x0F -> asc='F'
u8 Hex_Asc(u8 hex)
{      
	if(hex >15) return 'k';
	return ASCIITable[hex]; 
}
//将一个ASCII字符转换成16进制数
//例:asc='F' -> hex=0x0F 
u8 Asc_Hex(u8 asc)
{
	unsigned char p;
	switch(asc)
	{
		case '0':{p=0; } break;
		case '1':{p=1; } break;
		case '2':{p=2; } break;
		case '3':{p=3; } break;
		case '4':{p=4; } break;
		case '5':{p=5; } break;
		case '6':{p=6; } break;
		case '7':{p=7; } break;
		case '8':{p=8; } break;
		case '9':{p=9; } break;
		case 'A':{p=10; } break;
		case 'B':{p=11; } break;
		case 'C':{p=12; } break;
		case 'D':{p=13; } break;
		case 'E':{p=14; } break;
		case 'F':{p=15; } break;
		default: {p='k';}
	}
	return(p);
}
/*==========================================================================
[   void  HexToAsc(unsigned char hex,unsigned char *asc)					]
[	In	:	hex -- unsigned char, hexadecimal constant						]
[	Out	:	asc -- pointer which point to ascii code						]
[				asc[0] = high character, asc[1] = low character				]
[	Return : none															]
[	Comment: This call may be converte one byte hexadecimal constant to		]
[			two ascii character.											]
========================================================================== */
//将一个16进制数转换成2个字符
//例:hex=0x12 -> asc[0]='1',asc[1]='2';
void HexToAsc(u8 hex,u8 *asc)
{
	asc[0] = ASCIITable[hex >> 4];
	asc[1] = ASCIITable[hex & 0x0F];
}
void HexPtrAsc(u8 *hex,u8 *asc,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		HexToAsc(hex[i],&asc[i*2]);
	}
	
}
//将一个10进制数转换成2个字符
//例:bcd=12 -> asc[0]='1',asc[1]='2';
void BcdToAsc(u8 bcd,u8 *asc)
{
	asc[0] = ASCIITable[bcd/10];
	asc[1] = ASCIITable[bcd%10];
}
/*==========================================================================
[	unsigned char  AscToHex(unsigned char *asc,unsigned char *hex)			]
[	In	:	asc -- pointer which point to ascii code						]
[				asc[0] = high character, asc[1] = low character				]
[	Out	:	hex -- unsigned char, hexadecimal constant						]
[	Return : 0, converted failed; 1, OK										]
[	Comment: This call may be converte two ascii character to				]
[			one byte hexadecimal constant.									]
========================================================================== */
//将2个字符转换长一个16进制数,
//例:asc[0]='1',asc[1]='2' -> hex=0x12
u8 AscToHex(u8 *asc)
{
	u8 hex;
	hex = Asc_Hex(asc[0]);
	hex <<= 4;
	hex |=Asc_Hex(asc[1]);
	return hex;
}
void AscPtrHex(u8 *asc,u8 *hex,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		hex[i] = Asc_Hex(asc[i*2]);
		hex[i] <<= 4;
		hex[i] |=Asc_Hex(asc[i*2+1]);
	}
}
//10进制转16进制
//例:bcd=20 -> hex=0x20
u8 Bcd_Hex(u8 bcd)
{
	u8 temp_buf0,temp_buf1;
	temp_buf0=bcd/10;
	temp_buf1=bcd%10;
	temp_buf0<<=4;
	temp_buf0|=temp_buf1;
	return temp_buf0;
}
//16进制转10进制
//例:hex=0x20 -> bcd=20 
u8 Hex_Bcd(u8 hex)
{
    u8  temp_buf0,temp_buf1;
    temp_buf0=hex>>4;
    temp_buf1=hex&0x0F;
    temp_buf0=temp_buf0*10+temp_buf1;
    return(temp_buf0);
}  

//比较2组数据是否相等
u8 memcmpstr(u8 *da,u8 *db,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		if(da[i]!=db[i])	break;
	}
	if(i==len)	return 1;//数据全部相等
	return 0;//不相等
}
//比较1组数据中是否和一个数相等
u8 memcmpone(u8 *da,u8 db,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		if(da[i]==db)	return 1;//有一个相等
	}
	return 0;//不相等
}
//比较一组数据和一个数是否全部相等
u8 memcmpnum(u8 *da,u8 db,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		if(da[i]!=db)	return 0;//不相等
	}
	return 1;//相等
}
u8 onecmpstr(u8 da,u8 *db,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		if(da == db[i])	return 1;
	}
	return 0;
}
u16 BytesToWord(u8 *bytes)
{
	u16 word;
	word = bytes[0];
	word <<= 8;
	word += bytes[1];
	return word;
}






