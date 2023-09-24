	.module adc.c
	.area text(rom, con, rel)
	.dbfile D:\OLED\adc.c
	.dbfunc e InitADC _InitADC fV
	.even
_InitADC::
	.dbline -1
	.dbline 10
; //ICC-AVR application builder : 2006-02-12 14:00:00
; // Target : ATmega48
; // Crystal: 1.000Mhz
; // Author:  jackyan
; 
; #include "iom48v.h"
; //ADC initialize
; // Conversion time: 6uS
; void InitADC( void)
; {
	.dbline 11
;  	ADCSRA = 0x00; //disable adc
	clr R2
	sts 122,R2
	.dbline 12
;  	ADMUX = 0x43; //select adc input 3,AVCC
	ldi R24,67
	sts 124,R24
	.dbline 13
;  	ACSR  = 0x80;	// 
	ldi R24,128
	out 0x30,R24
	.dbline 14
;  	ADCSRA = 0x80;	// ADC 功能开启，但是没有进行转换,单次转换，不中断,ADIF=0
	sts 122,R24
	.dbline 15
;  	ADCH=ADCL=0;
	sts 120,R2
	sts 121,R2
	.dbline -2
L1:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e ReadADC _ReadADC fI
;              i -> R20,R21
;        channel -> R16
	.even
_ReadADC::
	rcall push_gset1
	.dbline -1
	.dbline 19
; }
; 
; int ReadADC( unsigned char channel)
; {
	.dbline 22
;     int i;
; 
;     ADMUX |= channel;                        // Select channel
	lds R2,124
	or R2,R16
	sts 124,R2
	.dbline 24
;     
;     ADCSRA |= 0x40;                          // Start conversion
	lds R24,122
	ori R24,64
	sts 122,R24
L3:
	.dbline 26
L4:
	.dbline 26
;     
;     while (!(ADCSRA & 0x10));                // Check if converstion is ready
	lds R2,122
	sbrs R2,4
	rjmp L3
	.dbline 28
;     
;     ADCSRA |= 0x10;                          // Clear Conversion ready flag by setting the bit
	lds R24,122
	ori R24,16
	sts 122,R24
	.dbline 30
; 
;     i = ADCL;                               // Read 8 low bits first (important)
	lds R20,120
	clr R21
	.dbline 31
;     i += (int)ADCH << 8;                    // Read 2 high bits and multiply with 256
	lds R2,121
	clr R3
	mov R3,R2
	clr R2
	add R20,R2
	adc R21,R3
	.dbline 33
; 
;     return i;
	movw R16,R20
	.dbline -2
L2:
	rcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r i 20 I
	.dbsym r channel 16 c
	.dbend
