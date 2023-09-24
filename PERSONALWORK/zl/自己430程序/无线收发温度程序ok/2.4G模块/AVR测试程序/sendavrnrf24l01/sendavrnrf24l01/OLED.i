002E:OLED.lis:04C65:1:14=7C526E00:16=47d21313:                        .module OLED.c
                        .area text(rom, con, rel)
 0000                   .dbfile D:\OLED\OLED.c
 0000                   .dbfunc e InitOLED _InitOLED fV
                        .even
 0000           _InitOLED::
 0000                   .dbline -1
 0000                   .dbline 20
 0000           ; //ICC-AVR application builder : 2006-02-12 14:00:00
 0000           ; // Target : ATmega48
 0000           ; // Crystal: 8.000Mhz
 0000           ; // Author:  jackyan
 0000           ; 
 0000           ; 
 0000           ; #include "iom48v.h"
 0000           ; #include "macros.h"
 0000           ; #include "Defs.h"
 0000           ; #include "OLED.h"
 0000           ; #include "var.h"
 0000           ; #include "eeprom.h"
 0000           ; 
 0000           ; extern void I2C_Write(unsigned char M_ADD, unsigned char Data);
 0000           ; extern unsigned char I2C_Read(unsigned char M_ADD);
 0000           ; 
 0000           ; 
 0000           ; 
 0000           ; void InitOLED(void)
 0000           ; {
 0000                   .dbline 21
 0000           ;       WriteMode();
 0000 28D0              rcall _WriteMode
 0002                   .dbline -2
 0002           L1:
 0002                   .dbline 0 ; func end
 0002 0895              ret
 0004                   .dbend
 0004                   .dbfunc e WriteOLED _WriteOLED fV
 0004           ;          temp2 -> R20
 0004           ;           temp -> R22
 0004           ;            val -> R10
 0004           ;        address -> R12
                        .even
 0004           _WriteOLED::
 0004 00D0              rcall push_gset4
 0006 A22E              mov R10,R18
 0008 C02E              mov R12,R16
 000A                   .dbline -1
 000A                   .dbline 25
 000A           ; }
 000A           ; 
 000A           ; void WriteOLED(ubyte address,ubyte val)
 000A           ; {
 000A                   .dbline 26
 000A           ;       ubyte temp=0;
 000A 6627              clr R22
 000C                   .dbline 27
 000C           ;       ubyte temp2=20;
 000C 44E1              ldi R20,20
 000E 0FC0              rjmp L4
 0010           L3:
 0010                   .dbline 29
 0010           ;       while(temp2!=0)
 0010           ;               {
 0010                   .dbline 30
 0010           ;               I2C_Write(address,val);
 0010 2A2D              mov R18,R10
 0012 0C2D              mov R16,R12
 0014 00D0              rcall _I2C_Write
 0016                   .dbline 31
 0016           ;               temp=ReadOLED(address);
 0016 0C2D              mov R16,R12
 0018 15D0              rcall _ReadOLED
 001A 602F              mov R22,R16
 001C                   .dbline 32
 001C           ;               if(temp!=val)
 001C 0A15              cp R16,R10
 001E 49F0              breq L5
 0020                   .dbline 33
 0020           ;                       {
 0020                   .dbline 34
 0020           ;                       temp2--;
 0020 4A95              dec R20
 0022                   .dbline 35
 0022           ;                       WDR();
 0022 A895              wdr
 0024                   .dbline 36
 0024           ;                       for(temp=0;temp<100;temp++)
 0024 6627              clr R22
 0026 01C0              rjmp L11
 0028           L8:
 0028                   .dbline 37
 0028           L9:
 0028                   .dbline 36
 0028 6395              inc R22
 002A           L11:
 002A                   .dbline 36
 002A 6436              cpi R22,100
 002C E8F3              brlo L8
 002E                   .dbline 38
 002E           ;                               ;
 002E           ;                       }
 002E                   .dbline 40
 002E           ;               else
 002E           ;                       break;
 002E           L7:
 002E                   .dbline 41
 002E           L4:
 002E                   .dbline 28
 002E 4423              tst R20
 0030 79F7              brne L3
 0032           L5:
 0032                   .dbline 42
 0032           ;               }
 0032           ;       if (temp2==0)   // ����10�ζ�û�гɹ�
 0032 4423              tst R20
 0034 29F4              brne L12
 0036                   .dbline 43
 0036                   .dbline 43
 0036 80910000          lds R24,_Flag
 003A 8260              ori R24,2
 003C 80930000          sts _Flag,R24
 0040                   .dbline 43
 0040           L12:
 0040                   .dbline 43
 0040           ;               set(Flag,bitOledError); 
 0040                   .dbline -2
 0040           L2:
 0040 00D0              rcall pop_gset4
 0042                   .dbline 0 ; func end
 0042 0895              ret
 0044                   .dbsym r temp2 20 c
 0044                   .dbsym r temp 22 c
 0044                   .dbsym r val 10 c
 0044                   .dbsym r address 12 c
 0044                   .dbend
 0044                   .dbfunc e ReadOLED _ReadOLED fc
 0044           ;           temp -> R20
 0044           ;        address -> R20
                        .even
 0044           _ReadOLED::
 0044 00D0              rcall push_gset1
 0046 402F              mov R20,R16
 0048                   .dbline -1
 0048                   .dbline 47
 0048           ; }
 0048           ; 
 0048           ; ubyte ReadOLED(ubyte address)
 0048           ; {
 0048                   .dbline 49
 0048           ;       ubyte temp;
 0048           ;       temp=I2C_Read(address);
 0048 042F              mov R16,R20
 004A 00D0              rcall _I2C_Read
 004C 402F              mov R20,R16
 004E                   .dbline 50
 004E           ;       return temp;
 004E                   .dbline -2
 004E           L14:
 004E 00D0              rcall pop_gset1
 0050                   .dbline 0 ; func end
 0050 0895              ret
 0052                   .dbsym r temp 20 c
 0052                   .dbsym r address 20 c
 0052                   .dbend
 0052                   .dbfunc e WriteMode _WriteMode fV
 0052           ;           temp -> <dead>
 0052           ;          temp2 -> R20
                        .even
 0052           _WriteMode::
 0052 00D0              rcall push_gset1
 0054                   .dbline -1
 0054                   .dbline 54
 0054           ; }
 0054           ; 
 0054           ; void WriteMode(void)
 0054           ; {
 0054                   .dbline 56
 0054           ;       ubyte temp,temp2;
 0054           ;       temp2=0;
 0054 4427              clr R20
 0056                   .dbline 57
 0056           ;       clr(Flag,bitOledError);
 0056                   .dbline 57
 0056 80910000          lds R24,_Flag
 005A 8D7F              andi R24,253
 005C 80930000          sts _Flag,R24
 0060                   .dbline 57
 0060                   .dbline 57
 0060                   .dbline 59
 0060           ;       
 0060           ;       if (OLEDMode==NTSC)
 0060 80910000          lds R24,_OLEDMode
 0064 8535              cpi R24,85
 0066 09F0              breq X0
 0068 51C0              rjmp L16
 006A           X0:
 006A                   .dbline 60
 006A           ;               {
 006A                   .dbline 61
 006A           ;               WriteOLED(0x01,0x78);
 006A 28E7              ldi R18,120
 006C 01E0              ldi R16,1
 006E CADF              rcall _WriteOLED
 0070                   .dbline 62
 0070           ;               WriteOLED(0x02,0x30);
 0070 20E3              ldi R18,48
 0072 02E0              ldi R16,2
 0074 C7DF              rcall _WriteOLED
 0076                   .dbline 63
 0076           ;               WriteOLED(0x03,0x78);
 0076 28E7              ldi R18,120
 0078 03E0              ldi R16,3
 007A C4DF              rcall _WriteOLED
 007C                   .dbline 64
 007C           ;               WriteOLED(0x04,0x30);
 007C 20E3              ldi R18,48
 007E 04E0              ldi R16,4
 0080 C1DF              rcall _WriteOLED
 0082                   .dbline 65
 0082           ;               WriteOLED(0x05,0x78);
 0082 28E7              ldi R18,120
 0084 05E0              ldi R16,5
 0086 BEDF              rcall _WriteOLED
 0088                   .dbline 67
 0088           ;               
 0088           ;               WriteOLED(0x06,0x30);
 0088 20E3              ldi R18,48
 008A 06E0              ldi R16,6
 008C BBDF              rcall _WriteOLED
 008E                   .dbline 68
 008E           ;               WriteOLED(0x07,Contrast);
 008E 20910000          lds R18,_Contrast
 0092 07E0              ldi R16,7
 0094 B7DF              rcall _WriteOLED
 0096                   .dbline 69
 0096           ;               WriteOLED(0x08,Bright);
 0096 20910000          lds R18,_Bright
 009A 08E0              ldi R16,8
 009C B3DF              rcall _WriteOLED
 009E                   .dbline 70
 009E           ;               WriteOLED(0x09,0x12);
 009E 22E1              ldi R18,18
 00A0 09E0              ldi R16,9
 00A2 B0DF              rcall _WriteOLED
 00A4                   .dbline 71
 00A4           ;               WriteOLED(0x0a,0x01);
 00A4 21E0              ldi R18,1
 00A6 0AE0              ldi R16,10
 00A8 ADDF              rcall _WriteOLED
 00AA                   .dbline 72
 00AA           ;               WriteOLED(0x0b,0x00);
 00AA 2227              clr R18
 00AC 0BE0              ldi R16,11
 00AE AADF              rcall _WriteOLED
 00B0                   .dbline 74
 00B0           ; 
 00B0           ;               WriteOLED(0x0c,0x02);
 00B0 22E0              ldi R18,2
 00B2 0CE0              ldi R16,12
 00B4 A7DF              rcall _WriteOLED
 00B6                   .dbline 75
 00B6           ;               WriteOLED(0x0d,0x00);
 00B6 2227              clr R18
 00B8 0DE0              ldi R16,13
 00BA A4DF              rcall _WriteOLED
 00BC                   .dbline 76
 00BC           ;               WriteOLED(0x0e,0x80);
 00BC 20E8              ldi R18,128
 00BE 0EE0              ldi R16,14
 00C0 A1DF              rcall _WriteOLED
 00C2                   .dbline 77
 00C2           ;               WriteOLED(0x0f,0x0a);
 00C2 2AE0              ldi R18,10
 00C4 0FE0              ldi R16,15
 00C6 9EDF              rcall _WriteOLED
 00C8                   .dbline 78
 00C8           ;               WriteOLED(0x10,0x1b);
 00C8 2BE1              ldi R18,27
 00CA 00E1              ldi R16,16
 00CC 9BDF              rcall _WriteOLED
 00CE                   .dbline 79
 00CE           ;               WriteOLED(0x11,0x07);
 00CE 27E0              ldi R18,7
 00D0 01E1              ldi R16,17
 00D2 98DF              rcall _WriteOLED
 00D4                   .dbline 81
 00D4           ; 
 00D4           ;               WriteOLED(0x12,0x08);
 00D4 28E0              ldi R18,8
 00D6 02E1              ldi R16,18
 00D8 95DF              rcall _WriteOLED
 00DA                   .dbline 82
 00DA           ;               WriteOLED(0x13,0x7b);
 00DA 2BE7              ldi R18,123
 00DC 03E1              ldi R16,19
 00DE 92DF              rcall _WriteOLED
 00E0                   .dbline 83
 00E0           ;               WriteOLED(0x14,0x0f);
 00E0 2FE0              ldi R18,15
 00E2 04E1              ldi R16,20
 00E4 8FDF              rcall _WriteOLED
 00E6                   .dbline 84
 00E6           ;               WriteOLED(0x15,0xd8);
 00E6 28ED              ldi R18,216
 00E8 05E1              ldi R16,21
 00EA 8CDF              rcall _WriteOLED
 00EC                   .dbline 85
 00EC           ;               WriteOLED(0x16,0x0c);
 00EC 2CE0              ldi R18,12
 00EE 06E1              ldi R16,22
 00F0 89DF              rcall _WriteOLED
 00F2                   .dbline 86
 00F2           ;               WriteOLED(0x17,0x01);
 00F2 21E0              ldi R18,1
 00F4 07E1              ldi R16,23
 00F6 86DF              rcall _WriteOLED
 00F8                   .dbline 88
 00F8           ; 
 00F8           ;               WriteOLED(0x18,0xc0);
 00F8 20EC              ldi R18,192
 00FA 08E1              ldi R16,24
 00FC 83DF              rcall _WriteOLED
 00FE                   .dbline 89
 00FE           ;               WriteOLED(0x19,0x00);
 00FE 2227              clr R18
 0100 09E1              ldi R16,25
 0102 80DF              rcall _WriteOLED
 0104                   .dbline 90
 0104           ;               WriteOLED(0x1a,0x00);
 0104 2227              clr R18
 0106 0AE1              ldi R16,26
 0108 7DDF              rcall _WriteOLED
 010A                   .dbline 91
 010A           ;               }
 010A 50C0              rjmp L17
 010C           L16:
 010C                   .dbline 93
 010C                   .dbline 94
 010C 28E7              ldi R18,120
 010E 01E0              ldi R16,1
 0110 79DF              rcall _WriteOLED
 0112                   .dbline 95
 0112 28E3              ldi R18,56
 0114 02E0              ldi R16,2
 0116 76DF              rcall _WriteOLED
 0118                   .dbline 96
 0118 28E7              ldi R18,120
 011A 03E0              ldi R16,3
 011C 73DF              rcall _WriteOLED
 011E                   .dbline 97
 011E 28E3              ldi R18,56
 0120 04E0              ldi R16,4
 0122 70DF              rcall _WriteOLED
 0124                   .dbline 98
 0124 28E7              ldi R18,120
 0126 05E0              ldi R16,5
 0128 6DDF              rcall _WriteOLED
 012A                   .dbline 100
 012A 28E3              ldi R18,56
 012C 06E0              ldi R16,6
 012E 6ADF              rcall _WriteOLED
 0130                   .dbline 101
 0130 20910000          lds R18,_Contrast
 0134 07E0              ldi R16,7
 0136 66DF              rcall _WriteOLED
 0138                   .dbline 102
 0138 20910000          lds R18,_Bright
 013C 08E0              ldi R16,8
 013E 62DF              rcall _WriteOLED
 0140                   .dbline 103
 0140 2AE4              ldi R18,74
 0142 09E0              ldi R16,9
 0144 5FDF              rcall _WriteOLED
 0146                   .dbline 104
 0146 20E1              ldi R18,16
 0148 0AE0              ldi R16,10
 014A 5CDF              rcall _WriteOLED
 014C                   .dbline 105
 014C 2227              clr R18
 014E 0BE0              ldi R16,11
 0150 59DF              rcall _WriteOLED
 0152                   .dbline 107
 0152 22E0              ldi R18,2
 0154 0CE0              ldi R16,12
 0156 56DF              rcall _WriteOLED
 0158                   .dbline 108
 0158 2227              clr R18
 015A 0DE0              ldi R16,13
 015C 53DF              rcall _WriteOLED
 015E                   .dbline 109
 015E 20E8              ldi R18,128
 0160 0EE0              ldi R16,14
 0162 50DF              rcall _WriteOLED
 0164                   .dbline 110
 0164 20EC              ldi R18,192
 0166 0FE0              ldi R16,15
 0168 4DDF              rcall _WriteOLED
 016A                   .dbline 111
 016A 2BE1              ldi R18,27
 016C 00E1              ldi R16,16
 016E 4ADF              rcall _WriteOLED
 0170                   .dbline 112
 0170 27E0              ldi R18,7
 0172 01E1              ldi R16,17
 0174 47DF              rcall _WriteOLED
 0176                   .dbline 114
 0176 2227              clr R18
 0178 02E1              ldi R16,18
 017A 44DF              rcall _WriteOLED
 017C                   .dbline 115
 017C 20E9              ldi R18,144
 017E 03E1              ldi R16,19
 0180 41DF              rcall _WriteOLED
 0182                   .dbline 116
 0182 2FE0              ldi R18,15
 0184 04E1              ldi R16,20
 0186 3EDF              rcall _WriteOLED
 0188                   .dbline 117
 0188 28ED              ldi R18,216
 018A 05E1              ldi R16,21
 018C 3BDF              rcall _WriteOLED
 018E                   .dbline 118
 018E 2CE0              ldi R18,12
 0190 06E1              ldi R16,22
 0192 38DF              rcall _WriteOLED
 0194                   .dbline 119
 0194 21E0              ldi R18,1
 0196 07E1              ldi R16,23
 0198 35DF              rcall _WriteOLED
 019A                   .dbline 121
 019A 20EC              ldi R18,192
 019C 08E1              ldi R16,24
 019E 32DF              rcall _WriteOLED
 01A0                   .dbline 122
 01A0 2227              clr R18
 01A2 09E1              ldi R16,25
 01A4 2FDF              rcall _WriteOLED
 01A6                   .dbline 123
 01A6 20EC              ldi R18,192
 01A8 0AE1              ldi R16,26
 01AA 2CDF              rcall _WriteOLED
 01AC                   .dbline 124
 01AC           L17:
 01AC                   .dbline -2
 01AC           L15:
 01AC 00D0              rcall pop_gset1
 01AE                   .dbline 0 ; func end
 01AE 0895              ret
 01B0                   .dbsym l temp 1 c
 01B0                   .dbsym r temp2 20 c
 01B0                   .dbend
 01B0                   .dbfunc e WriteBright _WriteBright fV
                        .even
 01B0           _WriteBright::
 01B0                   .dbline -1
 01B0                   .dbline 128
 01B0           ;       else    // if (OLEDMode==Pal)
 01B0           ;               {
 01B0           ;               WriteOLED(0x01,0x78);
 01B0           ;               WriteOLED(0x02,0x38);
 01B0           ;               WriteOLED(0x03,0x78);
 01B0           ;               WriteOLED(0x04,0x38);
 01B0           ;               WriteOLED(0x05,0x78);
 01B0           ;               
 01B0           ;               WriteOLED(0x06,0x38);
 01B0           ;               WriteOLED(0x07,Contrast);
 01B0           ;               WriteOLED(0x08,Bright);
 01B0           ;               WriteOLED(0x09,0x4a);
 01B0           ;               WriteOLED(0x0a,0x10);
 01B0           ;               WriteOLED(0x0b,0x00);
 01B0           ; 
 01B0           ;               WriteOLED(0x0c,0x02);
 01B0           ;               WriteOLED(0x0d,0x00);
 01B0           ;               WriteOLED(0x0e,0x80);
 01B0           ;               WriteOLED(0x0f,0xc0);
 01B0           ;               WriteOLED(0x10,0x1b);
 01B0           ;               WriteOLED(0x11,0x07);
 01B0           ; 
 01B0           ;               WriteOLED(0x12,0x00);
 01B0           ;               WriteOLED(0x13,0x90);
 01B0           ;               WriteOLED(0x14,0x0f);
 01B0           ;               WriteOLED(0x15,0xd8);
 01B0           ;               WriteOLED(0x16,0x0c);
 01B0           ;               WriteOLED(0x17,0x01);
 01B0           ; 
 01B0           ;               WriteOLED(0x18,0xc0);
 01B0           ;               WriteOLED(0x19,0x00);
 01B0           ;               WriteOLED(0x1a,0xc0);
 01B0           ;               }
 01B0           ; }
 01B0           ; 
 01B0           ; void WriteBright(void)
 01B0           ; {
 01B0                   .dbline 130
 01B0           ; //    WriteDA(Bright);
 01B0           ;       WriteOLED(0x08,Bright);
 01B0 20910000          lds R18,_Bright
 01B4 08E0              ldi R16,8
 01B6 26DF              rcall _WriteOLED
 01B8                   .dbline -2
 01B8           L18:
 01B8                   .dbline 0 ; func end
 01B8 0895              ret
 01BA                   .dbend
002A:oled.lk:09:1:14=7C526E00:16=47d21313:main.o 
