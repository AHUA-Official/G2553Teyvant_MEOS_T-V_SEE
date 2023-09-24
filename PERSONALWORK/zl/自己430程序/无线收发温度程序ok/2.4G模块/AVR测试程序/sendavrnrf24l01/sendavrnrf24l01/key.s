	.module key.c
	.area text(rom, con, rel)
	.dbfile D:\OLED\key.c
	.dbfunc e key _key fc
;           keyT -> R20
;         ADBuf2 -> y+2
	.even
_key::
	rcall push_gset1
	sbiw R28,4
	.dbline -1
	.dbline 16
; //ICC-AVR application builder : 2006-02-12 14:00:00
; // Target : ATmega48
; // Crystal: 1.000Mhz
; // Author:  jackyan
; 
; #include "iom48v.h"
; #include "macros.h"
; #include "Defs.h"
; #include "I2C.h"
; #include "var.h"
; #include "eeprom.h"
; 
; extern uword ADRead(ubyte chal);
; 
; ubyte key(void)
; {
	.dbline 18
; 	ubyte keyT;
; 	uword ADBuf2=0;
	clr R2
	clr R3
	std y+3,R3
	std y+2,R2
	.dbline 20
; 	
; 	ADBuf2=ADRead(3);
	ldi R16,3
	rcall _ADRead
	std y+3,R17
	std y+2,R16
	.dbline 22
; 
; 	keyT=NULL_KEY;
	ldi R20,255
	.dbline 24
; 
; 	if(((0x0000<ADBuf2)||(0x0000==ADBuf2))&&(ADBuf2<0x0030))
	cpi R16,0
	cpc R16,R17
	brne L4
X0:
	ldd R2,y+2
	ldd R3,y+3
	tst R2
	brne L2
	tst R3
	brne L2
X1:
L4:
	ldd R24,y+2
	ldd R25,y+3
	cpi R24,48
	ldi R30,0
	cpc R25,R30
	brsh L2
	.dbline 25
; 			keyT=Bright_Dw;
	ldi R20,2
L2:
	.dbline 27
; 	
; 	if((0x00a0<ADBuf2)&&(ADBuf2<0x0120))
	ldd R2,y+2
	ldd R3,y+3
	ldi R24,160
	ldi R25,0
	cp R24,R2
	cpc R25,R3
	brsh L5
	movw R24,R2
	cpi R24,32
	ldi R30,1
	cpc R25,R30
	brsh L5
	.dbline 28
; 			keyT=Bright_Up;
	ldi R20,1
L5:
	.dbline 30
; 	
; 	if((0x01f0<ADBuf2)&&(ADBuf2<0x0230))
	ldd R2,y+2
	ldd R3,y+3
	ldi R24,496
	ldi R25,1
	cp R24,R2
	cpc R25,R3
	brsh L7
	movw R24,R2
	cpi R24,48
	ldi R30,2
	cpc R25,R30
	brsh L7
	.dbline 31
; 			keyT=Contrast_Up;
	ldi R20,3
L7:
	.dbline 33
; 	
; 	if((0x02d0<ADBuf2)&&(ADBuf2<0x0320))
	ldd R2,y+2
	ldd R3,y+3
	ldi R24,720
	ldi R25,2
	cp R24,R2
	cpc R25,R3
	brsh L9
	movw R24,R2
	cpi R24,32
	ldi R30,3
	cpc R25,R30
	brsh L9
	.dbline 34
; 			keyT=Contrast_Dw;
	ldi R20,4
L9:
	.dbline 36
; 	
; 	if((0x0350<ADBuf2)&&(ADBuf2<0x0380))
	ldd R2,y+2
	ldd R3,y+3
	ldi R24,848
	ldi R25,3
	cp R24,R2
	cpc R25,R3
	brsh L11
	movw R24,R2
	cpi R24,128
	ldi R30,3
	cpc R25,R30
	brsh L11
	.dbline 37
; 			keyT=ModeKey;
	ldi R20,8
L11:
	.dbline 39
; 	
; 	EEPROM_WRITE(0x20, ADBuf2);		// test ads value of key
	ldi R24,2
	ldi R25,0
	std y+1,R25
	std y+0,R24
	movw R18,R28
	subi R18,254  ; offset = 2
	sbci R19,255
	ldi R16,32
	ldi R17,0
	rcall _EEPROMWriteBytes_extIO
	.dbline 40
; 	return keyT;
	mov R16,R20
	.dbline -2
L1:
	adiw R28,4
	rcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r keyT 20 c
	.dbsym l ADBuf2 2 i
	.dbend
	.dbfunc e keyLog _keyLog fV
	.even
_keyLog::
	.dbline -1
	.dbline 46
; }
; 
; 
; 
; void keyLog(void)
; {
	.dbline 47
; 	KeyTemp=key();
	rcall _key
	sts _KeyTemp,R16
	.dbline 49
; 	
; 	if(KeyBuf==KeyTemp) 	// 去抖动，连续两次读键相同，认为按键状态有效
	lds R3,_KeyBuf
	cp R3,R16
	breq X2
	rjmp L14
X2:
	.dbline 50
; 		{
	.dbline 51
; 		KeyValue=KeyBuf;	//	取按键值
	mov R2,R3
	sts _KeyValue,R2
	.dbline 52
; 		KeyTemp=0;			//	清除本次扫描结果
	clr R2
	sts _KeyTemp,R2
	.dbline 53
; 		if(KeyValue==KeyValue2) 	// 读取的按键值和先前的值相同，是同一按键，为连击。 
	lds R2,_KeyValue2
	cp R3,R2
	brne L16
	.dbline 54
; 			{
	.dbline 55
; 			if(chk(Flag,bitKeyContinue))		// 允许连击
	lds R2,_Flag
	sbrs R2,2
	rjmp L18
	.dbline 56
; 				{
	.dbline 57
; 				if(KeepKeyCnt!=0)
	lds R2,_KeepKeyCnt
	tst R2
	breq L20
	.dbline 58
; 					{
	.dbline 59
; 					clr(Flag,keybhits);		// 连击计数未到
	.dbline 59
	lds R24,_Flag
	andi R24,247
	sts _Flag,R24
	.dbline 59
	.dbline 59
	.dbline 60
; 					KeepKeyCnt--; 		// 连击计数器
	mov R24,R2
	subi R24,1
	sts _KeepKeyCnt,R24
	.dbline 61
; 					}
	rjmp L15
L20:
	.dbline 63
; 				else //(KeepKeyCnt==0)//&&(bitADError==0)) // AD 异常下，锁定按键
; 					{ 
	.dbline 64
; 					chKeyVal=KeyValue;	//	连击按键有效
	lds R2,_KeyValue
	sts _chKeyVal,R2
	.dbline 65
; 					set(Flag,keybhits);		// 有按键闭合
	.dbline 65
	lds R24,_Flag
	ori R24,8
	sts _Flag,R24
	.dbline 65
	.dbline 65
	.dbline 66
; 					}
	.dbline 67
; 				}
	rjmp L15
L18:
	.dbline 69
; 			else
; 				{ 	// 不允许连击，认为没有按键闭合
	.dbline 70
; 				clr(Flag,keybhits);
	.dbline 70
	lds R24,_Flag
	andi R24,247
	sts _Flag,R24
	.dbline 70
	.dbline 70
	.dbline 71
; 				clr(Flag,bitKeepKeyL);
	.dbline 71
	andi R24,223
	sts _Flag,R24
	.dbline 71
	.dbline 71
	.dbline 72
; 				KeepKeyCnt=255;
	ldi R24,255
	sts _KeepKeyCnt,R24
	.dbline 73
; 				clr(Flag,bitKeepKeyKeep);	// 非连击中				
	.dbline 73
	lds R24,_Flag
	andi R24,239
	sts _Flag,R24
	.dbline 73
	.dbline 73
	.dbline 74
; 				}
	.dbline 75
; 			}
	rjmp L15
L16:
	.dbline 77
; 		else
; 			{	//	读取的按键与先前闭合的按键不同,有新的按键
	.dbline 78
; 			chKeyVal=KeyValue2=KeyValue; 		
	lds R2,_KeyValue
	sts _KeyValue2,R2
	sts _chKeyVal,R2
	.dbline 79
; 			KeepKeyCnt=255;
	ldi R24,255
	sts _KeepKeyCnt,R24
	.dbline 80
; 			clr(Flag,bitKeyContinue);
	.dbline 80
	lds R24,_Flag
	andi R24,251
	sts _Flag,R24
	.dbline 80
	.dbline 80
	.dbline 81
; 			set(Flag,keybhits);
	.dbline 81
	ori R24,8
	sts _Flag,R24
	.dbline 81
	.dbline 81
	.dbline 82
; 			clr(Flag,bitKeepKeyL);
	.dbline 82
	andi R24,223
	sts _Flag,R24
	.dbline 82
	.dbline 82
	.dbline 83
; 			}
	.dbline 84
; 		}
	rjmp L15
L14:
	.dbline 87
	.dbline 88
	lds R2,_KeyTemp
	sts _KeyBuf,R2
	.dbline 89
	ldi R24,255
	sts _chKeyVal,R24
	.dbline 90
	sts _KeepKeyCnt,R24
	.dbline 91
	.dbline 91
	lds R24,_Flag
	andi R24,247
	sts _Flag,R24
	.dbline 91
	.dbline 91
	.dbline 92
	.dbline 92
	andi R24,239
	sts _Flag,R24
	.dbline 92
	.dbline 92
	.dbline 93
	.dbline 93
	andi R24,223
	sts _Flag,R24
	.dbline 93
	.dbline 93
	.dbline 94
L15:
	.dbline -2
L13:
	.dbline 0 ; func end
	ret
	.dbend
