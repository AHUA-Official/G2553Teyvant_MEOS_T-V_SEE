002D:DAC.lis:020E1:1:14=7C526E00:16=47d21313:                        .module DAC.c
                        .area text(rom, con, rel)
 0000                   .dbfile D:\OLED\DAC.c
 0000                   .dbfunc e InitDAC _InitDAC fV
                        .even
 0000           _InitDAC::
 0000                   .dbline -1
 0000                   .dbline 18
 0000           ; //ICC-AVR application builder : 2006-02-12 14:00:00
 0000           ; // Target : ATmega48
 0000           ; // Crystal: 1.000Mhz
 0000           ; // Author:  jackyan
 0000           ; 
 0000           ; #include "iom48v.h"
 0000           ; #include "macros.h"
 0000           ; #include "Defs.h"
 0000           ; #include "var.h"
 0000           ; 
 0000           ; void WriteDA(ubyte bright);
 0000           ; 
 0000           ; #define DAsync        BIT0
 0000           ; #define DAsclk        BIT1
 0000           ; #define DAout         BIT2
 0000           ; 
 0000           ; void InitDAC(void)
 0000           ; {
 0000                   .dbline 19
 0000           ;       WriteDA(Bright);
 0000 00910000          lds R16,_Bright
 0004 01D0              rcall _WriteDA
 0006                   .dbline -2
 0006           L1:
 0006                   .dbline 0 ; func end
 0006 0895              ret
 0008                   .dbend
 0008                   .dbfunc e WriteDA _WriteDA fV
 0008           ;            cnt -> R20
 0008           ;           temp -> R22
 0008           ;         bright -> R16
                        .even
 0008           _WriteDA::
 0008 00D0              rcall push_gset2
 000A                   .dbline -1
 000A                   .dbline 23
 000A           ; }
 000A           ; 
 000A           ; void WriteDA(ubyte bright)
 000A           ; {
 000A                   .dbline 25
 000A           ;       ubyte cnt;
 000A           ;       ubyte temp=0;
 000A 6627              clr R22
 000C                   .dbline 27
 000C           ; 
 000C           ;       set(DDRB,DAsync);
 000C                   .dbline 27
 000C 209A              sbi 0x4,0
 000E                   .dbline 27
 000E                   .dbline 27
 000E                   .dbline 28
 000E           ;       set(DDRB,DAsclk);
 000E                   .dbline 28
 000E 219A              sbi 0x4,1
 0010                   .dbline 28
 0010                   .dbline 28
 0010                   .dbline 29
 0010           ;       set(DDRB,DAout);
 0010                   .dbline 29
 0010 229A              sbi 0x4,2
 0012                   .dbline 29
 0012                   .dbline 29
 0012                   .dbline 30
 0012           ;       temp=bright>>4;
 0012 602F              mov R22,R16
 0014 6295              swap R22
 0016 6F70              andi R22,#0x0F
 0018                   .dbline 31
 0018           ;       cnt=8;          
 0018 48E0              ldi R20,8
 001A 08C0              rjmp L4
 001C           L3:
 001C                   .dbline 33
 001C           ;       while(cnt>0)
 001C           ;               {
 001C                   .dbline 34
 001C           ;                       clr(PORTB,DAsync);
 001C                   .dbline 34
 001C 2898              cbi 0x5,0
 001E                   .dbline 34
 001E                   .dbline 34
 001E                   .dbline 35
 001E           ;                       set(PORTB,DAsclk);
 001E                   .dbline 35
 001E 299A              sbi 0x5,1
 0020                   .dbline 35
 00