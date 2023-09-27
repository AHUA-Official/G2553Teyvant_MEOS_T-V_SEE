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
	
	if(KeyBuf==KeyTemp) 	// ȥ�������������ζ�����ͬ����Ϊ����״̬��Ч
		{
		KeyValue=KeyBuf;	//	ȡ����ֵ
		KeyTemp=0;			//	�������ɨ����
		if(KeyValue==KeyValue2) 	// ��ȡ�İ���ֵ����ǰ��ֵ��ͬ����ͬһ������Ϊ������ 
			{
			if(chk(Flag,bitKeyContinue))		// ��������
				{
				if(KeepKeyCnt!=0)
					{
					clr(Flag,keybhits);		// ��������δ��
					KeepKeyCnt--; 		// ����������
					}
				else //(KeepKeyCnt==0)//&&(bitADError==0)) // AD �쳣�£���������
					{ 
					chKeyVal=KeyValue;	//	����������Ч
					set(Flag,keybhits);		// �а����պ�
					}
				}
			else
				{ 	// ��������������Ϊû�а����պ�
				clr(Flag,keybhits);
				clr(Flag,bitKeepKeyL);
				KeepKeyCnt=255;
				clr(Flag,bitKeepKeyKeep);	// ��������				
				}
			}
		else
			{	//	��ȡ�İ�������ǰ�պϵİ�����ͬ,���µİ���
			chKeyVal=KeyValue2=KeyValue; 		
			KeepKeyCnt=255;
			clr(Flag,bitKeyContinue);
			set(Flag,keybhits);
			clr(Flag,bitKeepKeyL);
			}
		}

	else
		{	//	����2�ζ�ȡ�İ���״̬��ͬ,ֻ������2�εĽ��
		KeyBuf=KeyTemp;
		chKeyVal=NULL_KEY;
		KeepKeyCnt=255;
		clr(Flag,keybhits);
		clr(Flag,bitKeepKeyKeep);
		clr(Flag,bitKeepKeyL);
		}
}
