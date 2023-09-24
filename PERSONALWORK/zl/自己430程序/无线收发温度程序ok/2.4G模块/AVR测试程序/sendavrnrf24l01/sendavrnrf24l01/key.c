//ICC-AVR application builder : 2006-02-12 14:00:00
// Target : ATmega48
// Crystal: 1.000Mhz
// Author:  jackyan

#include "iom48v.h"
#include "macros.h"
#include "Defs.h"
#include "I2C.h"
#include "var.h"
#include "eeprom.h"

extern uword ADRead(ubyte chal);

ubyte key(void)
{
	ubyte keyT;
	uword ADBuf2=0;
	
	ADBuf2=ADRead(3);

	keyT=NULL_KEY;

	if(((0x0000<ADBuf2)||(0x0000==ADBuf2))&&(ADBuf2<0x0030))
			keyT=Bright_Dw;
	
	if((0x00a0<ADBuf2)&&(ADBuf2<0x0120))
			keyT=Bright_Up;
	
	if((0x01f0<ADBuf2)&&(ADBuf2<0x0230))
			keyT=Contrast_Up;
	
	if((0x02d0<ADBuf2)&&(ADBuf2<0x0320))
			keyT=Contrast_Dw;
	
	if((0x0350<ADBuf2)&&(ADBuf2<0x0380))
			keyT=ModeKey;
	
	EEPROM_WRITE(0x20, ADBuf2);		// test ads value of key
	return keyT;
}



void keyLog(void)
{
	KeyTemp=key();
	
	if(KeyBuf==KeyTemp) 	// 去抖动，连续两次读键相同，认为按键状态有效
		{
		KeyValue=KeyBuf;	//	取按键值
		KeyTemp=0;			//	清除本次扫描结果
		if(KeyValue==KeyValue2) 	// 读取的按键值和先前的值相同，是同一按键，为连击。 
			{
			if(chk(Flag,bitKeyContinue))		// 允许连击
				{
				if(KeepKeyCnt!=0)
					{
					clr(Flag,keybhits);		// 连击计数未到
					KeepKeyCnt--; 		// 连击计数器
					}
				else //(KeepKeyCnt==0)//&&(bitADError==0)) // AD 异常下，锁定按键
					{ 
					chKeyVal=KeyValue;	//	连击按键有效
					set(Flag,keybhits);		// 有按键闭合
					}
				}
			else
				{ 	// 不允许连击，认为没有按键闭合
				clr(Flag,keybhits);
				clr(Flag,bitKeepKeyL);
				KeepKeyCnt=255;
				clr(Flag,bitKeepKeyKeep);	// 非连击中				
				}
			}
		else
			{	//	读取的按键与先前闭合的按键不同,有新的按键
			chKeyVal=KeyValue2=KeyValue; 		
			KeepKeyCnt=255;
			clr(Flag,bitKeyContinue);
			set(Flag,keybhits);
			clr(Flag,bitKeepKeyL);
			}
		}

	else
		{	//	连续2次读取的按键状态不同,只保留第2次的结果
		KeyBuf=KeyTemp;
		chKeyVal=NULL_KEY;
		KeepKeyCnt=255;
		clr(Flag,keybhits);
		clr(Flag,bitKeepKeyKeep);
		clr(Flag,bitKeepKeyL);
		}
}
