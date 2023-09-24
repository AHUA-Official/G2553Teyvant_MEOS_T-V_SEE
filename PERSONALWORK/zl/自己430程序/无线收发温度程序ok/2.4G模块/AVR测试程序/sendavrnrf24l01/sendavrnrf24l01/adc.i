002D:adc.lis:010A2:1:14=7C526E00:16=47d21313:                        .module adc.c
                        .area text(rom, con, rel)
 0000                   .dbfile D:\OLED\adc.c
 0000                   .dbfunc e InitADC _InitADC fV
                        .even
 0000           _InitADC::
 0000                   .dbline -1
 0000                   .dbline 10
 0000           ; //ICC-AVR application builder : 2006-02-12 14:00:00
 0000           ; // Target : ATmega48
 0000           ; // Crystal: 1.000Mhz
 0000           ; // Author:  jackyan
 0000           ; 
 0000           ; #include "iom48v.h"
 0000           ; //ADC initialize
 0000           ; // Conversion time: 6uS
 0000           ; void InitADC( void)
 0000           ; {
 0000                   .dbline 11
 0000           ;       ADCSRA = 0x00; //disable adc
 0000 2224              clr R2
 0002 20927A00          sts 122,R2
 0006                   .dbline 12
 0006           ;       ADMUX = 0x43; //select adc input 3,AVCC
 0006 83E4              ldi R24,67
 0008 80937C00          sts 124,R24
 000C                   .dbline 13
 000C           ;       ACSR  = 0x80;   // 
 000C 80E8              ldi R24,128
 000E 80BF              out 0x30,R24
 0010                   .dbline 14
 0010           ;       ADCSRA = 0x80;  // ADC 功能开启，但是没有进行转换,单次转换，不中断,ADIF=0
 0010 80937A00          sts 122,R24
 0014                   .dbline 15
 0014           ;       ADCH=ADCL=0;
 0014 20927800          sts 120,R2
 0018 20927900          sts 121,R2
 001C                   .dbline -2
 001C           L1:
 001C                   .dbline 0 ; func end
 001C 0895              ret
 001E                   .dbend
 001E                   .dbfunc e ReadADC _ReadADC fI
 001E           ;              i -> R20,R21
 001E           ;        channel -> R16
                        .even
 001E           _ReadADC::
 001E 00D0              rcall push_gset1
 0020                   .dbline -1
 0020                   .dbline 19
 0020           ; }
 0020           ; 
 0020           ; int ReadADC( unsigned char channel)
 0020           ; {
 0020                   .dbline 22
 0020           ;     int i;
 0020           ; 
 0020           ;     ADMUX |= channel;                        // Select channel
 0020 20907C00          lds R2,124
 0024 202A              or R2,R16
 0026 20927C00          sts 124,R2
 002A                   .dbline 24
 002A           ;     
 002A           ;     ADCSRA |= 0x40;                          // Start conversion
 002A 80917A00          lds R24,122
 002E 8064              ori R24,64
 0030 80937A00          sts 122,R24
 0034           L3:
 0034                   .dbline 26
 0034           L4:
 0034                   .dbline 26
 0034           ;     
 0034           ;     while (!(ADCSRA & 0x10));                // Check if converstion is ready
 0034 20907A00          lds R2,122
 0038 24FE              sbrs R2,4
 003A FCCF              rjmp L3
 003C                   .dbline 28
 003C           ;     
 003C           ;     ADCSRA |= 