	.module DAC.c
	.area text(rom, con, rel)
	.dbfile D:\OLED\DAC.c
	.dbfunc e InitDAC _InitDAC fV
	.even
_InitDAC::
	.dbline -1
	.dbline 18
; //ICC-AVR application builder : 2006-02-12 14:00:00
; // Target : ATmega48
; // Crystal: 1.000Mhz
; // Author:  jackyan
; 
; #include "iom48v.h"
; #include "macros.h"
; #include "Defs.h"
; #include "var.h"
; 
; void WriteDA(ubyte bright);
; 
; #define DAsync 	BIT0
; #define DAsclk	BIT1
; #define DAout 	BIT2
; 
; void InitDAC(void)
; {
	.dbline 19
; 	WriteDA(Bright);
	lds R16,_Bright
	rcall _WriteDA
	.dbline -2
L1:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e WriteDA _WriteDA fV
;            cnt -> R20
;           temp -> R22
;         bright -> R16
	.even
_WriteDA::
	rcall push_gset2
	.dbline -1
	.dbline 23
; }
; 
; void WriteDA(ubyte bright)
; {
	.dbline 25
; 	ubyte cnt;
; 	ubyte temp=0;
	clr R22
	.dbline 27
; 
; 	set(DDRB,DAsync);
	.dbline 27
	sbi 0x4,0
	.dbline 27
	.dbline 27
	.dbline 28
; 	set(DDRB,DAsclk);
	.dbline 28
	sbi 0x4,1
	.dbline 28
	.dbline 28
	.dbline 29
; 	set(DDRB,DAout);
	.dbline 29
	sbi 0x4,2
	.dbline 29
	.dbline 29
	.dbline 30
; 	temp=bright>>4;
	mov R22,R16
	swap R22
	andi R22,#0x0F
	.dbline 31
; 	cnt=8;		
	ldi R20,8
	rjmp L4
L3:
	.dbline 33
; 	while(cnt>0)
; 		{
	.dbline 34
; 			clr(PORTB,DAsync);
	.dbline 34
	cbi 0x5,0
	.dbline 34
	.dbline 34
	.dbline 35
; 			set(PORTB,DAsclk);
	.dbline 35
	sbi 0x5,1
	.dbline 35
	.dbline 35
	.dbline 36
; 			clr(PORTB,DAout);
	.dbline 36
	cbi 0x5,2
	.dbline 36
	.dbline 36
	.dbline 37
; 			if (chk(temp,BIT7))
	sbrc R22,7
	.dbline 38
	.dbline 38
	sbi 0x5,2
	.dbline 38
L6:
	.dbline 38
	.dbline 39
	lsl R22
	.dbline 40
	dec R20
	.dbline 41
	.dbline 41
	cbi 0x5,1
	.dbline 41
	.dbline 41
	.dbline 42
L4:
	.dbline 32
	clr R2
	cp R2,R20
	brlo L3
	.dbline 44
; 				set(PORTB,DAout);
; 			temp<<=1;
; 			cnt--;
; 			clr(PORTB,DAsclk);	//0
; 		}
; 
; 	bright<<=4;
	mov R24,R16
	andi R24,#0x0F
	swap R24
	mov R16,R24
	.dbline 45
; 	temp=bright|0x0f;
	mov R22,R24
	ori R22,15
	.dbline 46
; 	cnt=8;		
	ldi R20,8
	rjmp L9
L8:
	.dbline 48
; 	while(cnt>0)
; 		{
	.dbline 49
; 			clr(PORTB,DAsync);
	.dbline 49
	cbi 0x5,0
	.dbline 49
	.dbline 49
	.dbline 50
; 			set(PORTB,DAsclk);
	.dbline 50
	sbi 0x5,1
	.dbline 50
	.dbline 50
	.dbline 51
; 			clr(PORTB,DAout);
	.dbline 51
	cbi 0x5,2
	.dbline 51
	.dbline 51
	.dbline 52
; 			if (chk(temp,BIT7))
	sbrc R22,7
	.dbline 53
	.dbline 53
	sbi 0x5,2
	.dbline 53
L11:
	.dbline 53
	.dbline 54
	lsl R22
	.dbline 55
	dec R20
	.dbline 56
	.dbline 56
	cbi 0x5,1
	.dbline 56
	.dbline 56
	.dbline 57
L9:
	.dbline 47
	clr R2
	cp R2,R20
	brlo L8
	.dbline 59
; 				set(PORTB,DAout);
; 			temp<<=1;
; 			cnt--;
; 			clr(PORTB,DAsclk);	//0
; 		}
; 
; 	set(PORTB,DAsync);
	.dbline 59
	sbi 0x5,0
	.dbline 59
	.dbline 59
	.dbline 60
; 	set(PORTB,DAsclk);
	.dbline 60
	sbi 0x5,1
	.dbline 60
	.dbline 60
	.dbline 61
; 	set(PORTB,DAout);
	.dbline 61
	sbi 0x5,2
	.dbline 61
	.dbline 61
	.dbline -2
L2:
	rcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r cnt 20 c
	.dbsym r temp 22 c
	.dbsym r bright 16 c
	.dbend
