	.module OLED.c
	.area text(rom, con, rel)
	.dbfile D:\OLED\OLED.c
	.dbfunc e InitOLED _InitOLED fV
	.even
_InitOLED::
	.dbline -1
	.dbline 20
; //ICC-AVR application builder : 2006-02-12 14:00:00
; // Target : ATmega48
; // Crystal: 8.000Mhz
; // Author:  jackyan
; 
; 
; #include "iom48v.h"
; #include "macros.h"
; #include "Defs.h"
; #include "OLED.h"
; #include "var.h"
; #include "eeprom.h"
; 
; extern void I2C_Write(unsigned char M_ADD, unsigned char Data);
; extern unsigned char I2C_Read(unsigned char M_ADD);
; 
; 
; 
; void InitOLED(void)
; {
	.dbline 21
; 	WriteMode();
	rcall _WriteMode
	.dbline -2
L1:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e WriteOLED _WriteOLED fV
;          temp2 -> R20
;           temp -> R22
;            val -> R10
;        address -> R12
	.even
_WriteOLED::
	rcall push_gset4
	mov R10,R18
	mov R12,R16
	.dbline -1
	.dbline 25
; }
; 
; void WriteOLED(ubyte address,ubyte val)
; {
	.dbline 26
; 	ubyte temp=0;
	clr R22
	.dbline 27
; 	ubyte temp2=20;
	ldi R20,20
	rjmp L4
L3:
	.dbline 29
; 	while(temp2!=0)
; 		{
	.dbline 30
; 		I2C_Write(address,val);
	mov R18,R10
	mov R16,R12
	rcall _I2C_Write
	.dbline 31
; 		temp=ReadOLED(address);
	mov R16,R12
	rcall _ReadOLED
	mov R22,R16
	.dbline 32
; 		if(temp!=val)
	cp R16,R10
	breq L5
	.dbline 33
; 			{
	.dbline 34
; 			temp2--;
	dec R20
	.dbline 35
; 			WDR();
	wdr
	.dbline 36
; 			for(temp=0;temp<100;temp++)
	clr R22
	rjmp L11
L8:
	.dbline 37
L9:
	.dbline 36
	inc R22
L11:
	.dbline 36
	cpi R22,100
	brlo L8
	.dbline 38
; 				;
; 			}
	.dbline 40
; 		else
; 			break;
L7:
	.dbline 41
L4:
	.dbline 28
	tst R20
	brne L3
L5:
	.dbline 42
; 		}
; 	if (temp2==0)	// 连续10次都没有成功
	tst R20
	brne L12
	.dbline 43
	.dbline 43
	lds R24,_Flag
	ori R24,2
	sts _Flag,R24
	.dbline 43
L12:
	.dbline 43
; 		set(Flag,bitOledError);	
	.dbline -2
L2:
	rcall pop_gset4
	.dbline 0 ; func end
	ret
	.dbsym r temp2 20 c
	.dbsym r temp 22 c
	.dbsym r val 10 c
	.dbsym r address 12 c
	.dbend
	.dbfunc e ReadOLED _ReadOLED fc
;           temp -> R20
;        address -> R20
	.even
_ReadOLED::
	rcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 47
; }
; 
; ubyte ReadOLED(ubyte address)
; {
	.dbline 49
; 	ubyte temp;
; 	temp=I2C_Read(address);
	mov R16,R20
	rcall _I2C_Read
	mov R20,R16
	.dbline 50
; 	return temp;
	.dbline -2
L14:
	rcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r temp 20 c
	.dbsym r address 20 c
	.dbend
	.dbfunc e WriteMode _WriteMode fV
;           temp -> <dead>
;          temp2 -> R20
	.even
_WriteMode::
	rcall push_gset1
	.dbline -1
	.dbline 54
; }
; 
; void WriteMode(void)
; {
	.dbline 56
; 	ubyte temp,temp2;
; 	temp2=0;
	clr R20
	.dbline 57
; 	clr(Flag,bitOledError);
	.dbline 57
	lds R24,_Flag
	andi R24,253
	sts _Flag,R24
	.dbline 57
	.dbline 57
	.dbline 59
; 	
; 	if (OLEDMode==NTSC)
	lds R24,_OLEDMode
	cpi R24,85
	breq X0
	rjmp L16
X0:
	.dbline 60
; 		{
	.dbline 61
; 		WriteOLED(0x01,0x78);
	ldi R18,120
	ldi R16,1
	rcall _WriteOLED
	.dbline 62
; 		WriteOLED(0x02,0x30);
	ldi R18,48
	ldi R16,2
	rcall _WriteOLED
	.dbline 63
; 		WriteOLED(0x03,0x78);
	ldi R18,120
	ldi R16,3
	rcall _WriteOLED
	.dbline 64
; 		WriteOLED(0x04,0x30);
	ldi R18,48
	ldi R16,4
	rcall _WriteOLED
	.dbline 65
; 		WriteOLED(0x05,0x78);
	ldi R18,120
	ldi R16,5
	rcall _WriteOLED
	.dbline 67
; 		
; 		WriteOLED(0x06,0x30);
	ldi R18,48
	ldi R16,6
	rcall _WriteOLED
	.dbline 68
; 		WriteOLED(0x07,Contrast);
	lds R18,_Contrast
	ldi R16,7
	rcall _WriteOLED
	.dbline 69
; 		WriteOLED(0x08,Bright);
	lds R18,_Bright
	ldi R16,8
	rcall _WriteOLED
	.dbline 70
; 		WriteOLED(0x09,0x12);
	ldi R18,18
	ldi R16,9
	rcall _WriteOLED
	.dbline 71
; 		WriteOLED(0x0a,0x01);
	ldi R18,1
	ldi R16,10
	rcall _WriteOLED
	.dbline 72
; 		WriteOLED(0x0b,0x00);
	clr R18
	ldi R16,11
	rcall _WriteOLED
	.dbline 74
; 
; 		WriteOLED(0x0c,0x02);
	ldi R18,2
	ldi R16,12
	rcall _WriteOLED
	.dbline 75
; 		WriteOLED(0x0d,0x00);
	clr R18
	ldi R16,13
	rcall _WriteOLED
	.dbline 76
; 		WriteOLED(0x0e,0x80);
	ldi R18,128
	ldi R16,14
	rcall _WriteOLED
	.dbline 77
; 		WriteOLED(0x0f,0x0a);
	ldi R18,10
	ldi R16,15
	rcall _WriteOLED
	.dbline 78
; 		WriteOLED(0x10,0x1b);
	ldi R18,27
	ldi R16,16
	rcall _WriteOLED
	.dbline 79
; 		WriteOLED(0x11,0x07);
	ldi R18,7
	ldi R16,17
	rcall _WriteOLED
	.dbline 81
; 
; 		WriteOLED(0x12,0x08);
	ldi R18,8
	ldi R16,18
	rcall _WriteOLED
	.dbline 82
; 		WriteOLED(0x13,0x7b);
	ldi R18,123
	ldi R16,19
	rcall _WriteOLED
	.dbline 83
; 		WriteOLED(0x14,0x0f);
	ldi R18,15
	ldi R16,20
	rcall _WriteOLED
	.dbline 84
; 		WriteOLED(0x15,0xd8);
	ldi R18,216
	ldi R16,21
	rcall _WriteOLED
	.dbline 85
; 		WriteOLED(0x16,0x0c);
	ldi R18,12
	ldi R16,22
	rcall _WriteOLED
	.dbline 86
; 		WriteOLED(0x17,0x01);
	ldi R18,1
	ldi R16,23
	rcall _WriteOLED
	.dbline 88
; 
; 		WriteOLED(0x18,0xc0);
	ldi R18,192
	ldi R16,24
	rcall _WriteOLED
	.dbline 89
; 		WriteOLED(0x19,0x00);
	clr R18
	ldi R16,25
	rcall _WriteOLED
	.dbline 90
; 		WriteOLED(0x1a,0x00);
	clr R18
	ldi R16,26
	rcall _WriteOLED
	.dbline 91
; 		}
	rjmp L17
L16:
	.dbline 93
	.dbline 94
	ldi R18,120
	ldi R16,1
	rcall _WriteOLED
	.dbline 95
	ldi R18,56
	ldi R16,2
	rcall _WriteOLED
	.dbline 96
	ldi R18,120
	ldi R16,3
	rcall _WriteOLED
	.dbline 97
	ldi R18,56
	ldi R16,4
	rcall _WriteOLED
	.dbline 98
	ldi R18,120
	ldi R16,5
	rcall _WriteOLED
	.dbline 100
	ldi R18,56
	ldi R16,6
	rcall _WriteOLED
	.dbline 101
	lds R18,_Contrast
	ldi R16,7
	rcall _WriteOLED
	.dbline 102
	lds R18,_Bright
	ldi R16,8
	rcall _WriteOLED
	.dbline 103
	ldi R18,74
	ldi R16,9
	rcall _WriteOLED
	.dbline 104
	ldi R18,16
	ldi R16,10
	rcall _WriteOLED
	.dbline 105
	clr R18
	ldi R16,11
	rcall _WriteOLED
	.dbline 107
	ldi R18,2
	ldi R16,12
	rcall _WriteOLED
	.dbline 108
	clr R18
	ldi R16,13
	rcall _WriteOLED
	.dbline 109
	ldi R18,128
	ldi R16,14
	rcall _WriteOLED
	.dbline 110
	ldi R18,192
	ldi R16,15
	rcall _WriteOLED
	.dbline 111
	ldi R18,27
	ldi R16,16
	rcall _WriteOLED
	.dbline 112
	ldi R18,7
	ldi R16,17
	rcall _WriteOLED
	.dbline 114
	clr R18
	ldi R16,18
	rcall _WriteOLED
	.dbline 115
	ldi R18,144
	ldi R16,19
	rcall _WriteOLED
	.dbline 116
	ldi R18,15
	ldi R16,20
	rcall _WriteOLED
	.dbline 117
	ldi R18,216
	ldi R16,21
	rcall _WriteOLED
	.dbline 118
	ldi R18,12
	ldi R16,22
	rcall _WriteOLED
	.dbline 119
	ldi R18,1
	ldi R16,23
	rcall _WriteOLED
	.dbline 121
	ldi R18,192
	ldi R16,24
	rcall _WriteOLED
	.dbline 122
	clr R18
	ldi R16,25
	rcall _WriteOLED
	.dbline 123
	ldi R18,192
	ldi R16,26
	rcall _WriteOLED
	.dbline 124
L17:
	.dbline -2
L15:
	rcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym l temp 1 c
	.dbsym r temp2 20 c
	.dbend
	.dbfunc e WriteBright _WriteBright fV
	.even
_WriteBright::
	.dbline -1
	.dbline 128
; 	else	// if (OLEDMode==Pal)
; 		{
; 		WriteOLED(0x01,0x78);
; 		WriteOLED(0x02,0x38);
; 		WriteOLED(0x03,0x78);
; 		WriteOLED(0x04,0x38);
; 		WriteOLED(0x05,0x78);
; 		
; 		WriteOLED(0x06,0x38);
; 		WriteOLED(0x07,Contrast);
; 		WriteOLED(0x08,Bright);
; 		WriteOLED(0x09,0x4a);
; 		WriteOLED(0x0a,0x10);
; 		WriteOLED(0x0b,0x00);
; 
; 		WriteOLED(0x0c,0x02);
; 		WriteOLED(0x0d,0x00);
; 		WriteOLED(0x0e,0x80);
; 		WriteOLED(0x0f,0xc0);
; 		WriteOLED(0x10,0x1b);
; 		WriteOLED(0x11,0x07);
; 
; 		WriteOLED(0x12,0x00);
; 		WriteOLED(0x13,0x90);
; 		WriteOLED(0x14,0x0f);
; 		WriteOLED(0x15,0xd8);
; 		WriteOLED(0x16,0x0c);
; 		WriteOLED(0x17,0x01);
; 
; 		WriteOLED(0x18,0xc0);
; 		WriteOLED(0x19,0x00);
; 		WriteOLED(0x1a,0xc0);
; 		}
; }
; 
; void WriteBright(void)
; {
	.dbline 130
; //	WriteDA(Bright);
; 	WriteOLED(0x08,Bright);
	lds R18,_Bright
	ldi R16,8
	rcall _WriteOLED
	.dbline -2
L18:
	.dbline 0 ; func end
	ret
	.dbend
