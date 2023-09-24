002D:I2C.lis:0527E:1:14=7C526E00:16=47d21313:                        .module I2C.c
                        .area text(rom, con, rel)
 0000                   .dbfile D:\OLED\I2C.c
 0000                   .dbfunc e Init_I2C _Init_I2C fV
                        .even
 0000           _Init_I2C::
 0000                   .dbline -1
 0000                   .dbline 17
 0000           ; //ICC-AVR application builder : 2006-02-12 14:00:00
 0000           ; // Target : ATmega48
 0000           ; // Crystal: 1.000Mhz
 0000           ; // Author:  jackyan
 0000           ; 
 0000           ; #include "iom48v.h"
 0000           ; #include "macros.h"
 0000           ; #include "Defs.h"
 0000           ; #include "I2C.h"
 0000           ; 
 0000           ; 
 0000           ; // ******************************************** //
 0000           ; // *** This routine will setup the I2C Port *** //
 0000           ; // ******************************************** //
 0000           ; 
 0000           ; void Init_I2C(void)
 0000           ; {
 0000                   .dbline 18
 0000           ;  SETBIT(I2C_DIR,SCL);            // Set SCL to output 
 0000 3D9A              sbi 0x7,5
 0002                   .dbline 19
 0002           ;  SETBIT(I2C_DIR,SDA );           // Set SDA to output
 0002 3C9A              sbi 0x7,4
 0004                   .dbline 21
 0004           ;       
 0004           ;  SETBIT(I2C_PORT,SCL);                   // Set SCL high
 0004 459A              sbi 0x8,5
 0006                   .dbline 22
 0006           ;  SETBIT(I2C_PORT,SDA );                  // Set SDA high
 0006 449A              sbi 0x8,4
 0008                   .dbline -2
 0008           L1:
 0008                   .dbline 0 ; func end
 0008 0895              ret
 000A                   .dbend
 000A                   .dbfunc e I2C_Start _I2C_Start fV
                        .even
 000A           _I2C_Start::
 000A                   .dbline -1
 000A                   .dbline 32
 000A           ; }
 000A           ; 
 000A           ; 
 000A           ; 
 000A           ; // ************************************************ //
 000A           ; // *** This routine will send the I2C Start Bit *** //
 000A           ; // ************************************************ //
 000A           ; 
 000A           ; void I2C_Start (void)
 000A           ; {
 000A                   .dbline 33
 000A           ;  SETBIT(I2C_PORT,SCL);                                // Set SCL High
 000A 459A              sbi 0x8,5
 000C                   .dbline 34
 000C           ;  SETBIT(I2C_PORT,SDA);                                // Set SDA High
 000C 449A              sbi 0x8,4
 000E                   .dbline 36
 000E           ;  
 000E           ;  SETBIT(I2C_DIR,SDA);                    // Set SDA to output 
 000E 3C9A              sbi 0x7,4
 0010                   .dbline 37
 0010           ;  CLEARBIT(I2C_PORT,SDA);                 // Clear SDA
 0010 4498              cbi 0x8,4
 0012                   .dbline -2
 0012           L2:
 0012                   .dbline 0 ; func end
 0012 0895              ret
 0014                   .dbend
 0014                   .dbfunc e I2C_Stop _I2C_Stop fV
                        .even
 0014           _I2C_Stop::
 0014                   .dbline -1
 0014                   .dbline 46
 0014           ; }
 0014           ; 
 0014           ; 
 0014           ; // *********************************************** //
 0014           ; // *** This routine will send the I2C Stop Bit *** //
 0014           ; // *********************************************** //
 0014           ; 
 0014           ; void I2C_Stop (void)
 0014           ; {
 0014                   .dbline 48
 0014           ;  
 0014           ;  SETBIT(I2C_DIR,SDA);                    // Set SDA to output
 0014 3C9A              sbi 0x7,4
 0016                   .dbline 50
 0016           ;       
 0016           ;  CLEARBIT(I2C_PORT,SDA);                 // Clear SDA
 0016 4498              cbi 0x8,4
 0018                   .dbline 51
 0018           ;  CLEARBIT(I2C_PORT,SCL);                 // Clear SCL
 0018 4598              cbi 0x8,5
 001A                   .dbline 53
 001A           ;       
 001A           ;  SETBIT(I2C_PORT,SCL);                           // Set SCL High
 001A 459A              sbi 0x8,5
 001C                   .dbline 54
 001C           ;  SETBIT(I2C_PORT,SDA);                           // Set SDA High
 001C 449A              sbi 0x8,4
 001E                   .dbline -2
 001E           L3:
 001E                   .dbline 0 ; func end
 001E 0895              ret
 0020                   .dbend
 0020                   .dbfunc e I2C_Clock _I2C_Clock fV
                        .even
 0020           _I2C_Clock::
 0020                   .dbline -1
 0020                   .dbline 66
 0020           ; }
 0020           ; 
 0020           ; 
 0020           ; 
 0020           ; 
 0020           ; 
 0020           ; // ************************************************** //
 0020           ; // *** This routine will send the I2C clock pulse *** //
 0020           ; // ************************************************** //
 0020           ; 
 0020           ; void I2C_Clock (void)
 0020           ; {
 0020                   .dbline 67
 0020           ;  SETBIT(I2C_PORT,SCL);                                // Set SCL high
 0020 459A              sbi 0x8,5
 0022                   .dbline 70
 0022           ; // NOP();                                                             // Small Delay
 0022           ; // NOP();                                                             // Small Delay
 0022           ;  NOP();                                                               // Small Delay
 0022 0000              nop
 0024                   .dbline 71
 0024           ;  CLEARBIT(I2C_PORT,SCL);                      // Set SCL low
 0024 4598              cbi 0x8,5
 0026                   .dbline -2
 0026           L4:
 0026                   .dbline 0 ; func end
 0026 0895              ret
 0028                   .dbend
 0028                   .dbfunc e Write_I2C_Byte _Write_I2C_Byte fV
 0028           ;              i -> R20
 0028           ;           byte -> R22
                        .even
 0028           _Write_I2C_Byte::
 0028 00D0              rcall push_gset2
 002A 602F              mov R22,R16
 002C                   .dbline -1
 002C                   .dbline 82
 002C           ; }
 002C           ; 
 002C           ; 
 002C           ; 
 002C           ; 
 002C           ; // ******************************************************** //
 002C           ; // *** This routine will write a byte to the I2C device *** //
 002C           ; // ******************************************************** //
 002C           ; 
 002C           ; void Write_I2C_Byte(unsigned char byte)
 002C           ; {
 002C                   .dbline 85
 002C           ;  unsigned char i;
 002C           ; 
 002C           ;  SETBIT(I2C_DIR,SDA);                    // Set SDA to output 
 002C 3C9A              sbi 0x7,4
 002E                   .dbline 86
 002E           ;  CLEARBIT(I2C_PORT,SCL);
 002E 4598              cbi 0x8,5
 0030                   .dbline 88
 0030 4427              clr R20
 0032 0BC0              rjmp L9
 0034           L6:
 0034                   .dbline 88
 0034           ;       
 0034           ;  for (i = 0; i < 8; i++){              // Loop for our 8 bits
 0034                   .dbline 91
 0034           ;  
 0034           ;       // Set or Clear SDA pin
 0034           ;        if((byte & 0x80) == 0x80) SETBIT(I2C_PORT, SDA);       // Set I2C SDA PIN
 0034 862F              mov R24,R22
 0036 8078              andi R24,128
 0038 8038              cpi R24,128
 003A 11F4              brne L10
 003C                   .dbline 91
 003C 449A              sbi 0x8,4
 003E 01C0              rjmp L11
 0040           L10:
 0040                   .dbline 92
 0040           ;        else CLEARBIT(I2C_PORT,SDA);                                           // Clear I2C SDA PIN
 0040 4498              cbi 0x8,4
 0042           L11:
 0042                   .dbline 94
 0042 459A              sbi 0x8,5
 0044                   .dbline 96
 0044 660F              lsl R22
 0046                   .dbline 103
 0046 4598              cbi 0x8,5
 0048                   .dbline 104
 0048           L7:
 0048                   .dbline 88
 0048 4395              inc R20
 004A           L9:
 004A                   .dbline 88
 004A 4830              cpi R20,8
 004C 98F3              brlo L6
 004E                   .dbline 106
 004E           ;        
 004E           ;        SETBIT(I2C_PORT,SCL);                     // Set SCL High, Clock data
 004E           ; 
 004E           ;        byte = byte << 1;                                 // Shift data in buffer right one
 004E           ; 
 004E           ; //    NOP();
 004E           ; //    NOP();
 004E           ; //    NOP();
 004E           ; //    NOP();
 004E           ; 
 004E           ;      CLEARBIT(I2C_PORT,SCL);             // Clear SCL
 004E           ;       }
 004E           ;  
 004E           ; i=255;
 004E 4FEF              ldi R20,255
 0050 03C0              rjmp L13
 0052           L12:
 0052                   .dbline 108
 0052           ;  while(I2C_Ackn() )                              // Check for acknowledge from I2C device     
 0052           ;       {
 0052                   .dbline 109
 0052           ;       i--;
 0052 4A95              dec R20
 0054                   .dbline 110
 0054           ;       if(i==0)
 0054 09F4              brne L15
 0056                   .dbline 111
 0056           ;               break;
 0056 03C0              rjmp L14
 0058           L15:
 0058                   .dbline 112
 0058           L13:
 0058                   .dbline 107
 0058 18D0              rcall _I2C_Ackn
 005A 0023              tst R16
 005C D1F7              brne L12
 005E           L14:
 005E                   .dbline -2
 005E           L5:
 005E 00D0              rcall pop_gset2
 0060                   .dbline 0 ; func end
 0060 0895              ret
 0062                   .dbsym r i 20 c
 0062                   .dbsym r byte 22 c
 0062                   .dbend
 0062                   .dbfunc e Read_I2C_Byte _Read_I2C_Byte fc
 0062           ;              i -> R20
 0062           ;           buff -> R22
                        .even
 0062           _Read_I2C_Byte::
 0062 00D0              rcall push_gset2
 0064                   .dbline -1
 0064                   .dbline 124
 0064           ;       }
 0064           ; }
 0064           ; 
 0064           ; 
 0064           ; 
 0064           ; 
 0064           ; 
 0064           ; // ********************************************************* //
 0064           ; // *** This routine will read a byte from the I2C device *** //
 0064           ; // ********************************************************* //
 0064           ; 
 0064           ; unsigned char Read_I2C_Byte(void)
 0064           ; {     
 0064                   .dbline 125
 0064           ;  unsigned char i,buff = 0;
 0064 6627              clr R22
 0066                   .dbline 127
 0066 4427              clr R20
 0068 0AC0              rjmp L21
 006A           L18:
 006A                   .dbline 127
 006A           ;       
 006A           ;       for (i = 0; i < 8; i++){
 006A                   .dbline 129
 006A           ;       
 006A           ;               buff = buff << 1;
 006A 660F              lsl R22
 006C                   .dbline 131
 006C           ;               
 006C           ;               CLEARBIT(I2C_DIR,SDA);                     // Set SDA to input
 006C 3C98              cbi 0x7,4
 006E                   .dbline 133
 006E           ;                               
 006E           ;               SETBIT(I2C_PORT,SCL);                      // Set SCL High,Clock bit out
 006E 459A              sbi 0x8,5
 0070                   .dbline 136
 0070           ; //    NOP();
 0070           ; //    NOP();
 0070           ;       NOP();
 0070 0000              nop
 0072                   .dbline 139
 0072           ;       
 0072           ;               // Read data on SDA pin
 0072           ;               if ( CHECKBIT( I2C_IN,SDA ) ) buff = buff | 0x01;       
 0072 349B              sbis 0x6,4
 0074 01C0              rjmp L22
 0076                   .dbline 139
 0076 6160              ori R22,1
 0078           L22:
 0078                   .dbline 141
 0078 4598              cbi 0x8,5
 007A                   .dbline 145
 007A 0000              nop
 007C                   .dbline 146
 007C           L19:
 007C                   .dbline 127
 007C 4395              inc R20
 007E           L21:
 007E                   .dbline 127
 007E 4830              cpi R20,8
 0080 A0F3              brlo L18
 0082                   .dbline 150
 0082           ;               
 0082           ;               CLEARBIT(I2C_PORT,SCL);                    // Clear SCL         
 0082           ; 
 0082           ; //    NOP();
 0082           ; //    NOP();
 0082           ;       NOP();
 0082           ;         }
 0082           ; 
 0082           ;  // *** No ACK *** //
 0082           ;  
 0082           ;  I2C_Clock();                                                // Clock I2C bit
 0082 CEDF              rcall _I2C_Clock
 0084                   .dbline 152
 0084           ;       
 0084           ;  return buff;                                                    // Return our I2C byte
 0084 062F              mov R16,R22
 0086                   .dbline -2
 0086           L17:
 0086 00D0              rcall pop_gset2
 0088                   .dbline 0 ; func end
 0088 0895              ret
 008A                   .dbsym r i 20 c
 008A                   .dbsym r buff 22 c
 008A                   .dbend
 008A                   .dbfunc e I2C_Ackn _I2C_Ackn fc
 008A           ;           Ackn -> R16
                        .even
 008A           _I2C_Ackn::
 008A                   .dbline -1
 008A                   .dbline 163
 008A           ; }
 008A           ; 
 008A           ; 
 008A           ; 
 008A           ; 
 008A           ; // *********************************************************************** //
 008A           ; // *** This routine returns a 0 if the I2C device sends an acknowledge *** //
 008A           ; // *********************************************************************** //
 008A           ; 
 008A           ; unsigned char I2C_Ackn(void)
 008A           ; {
 008A                   .dbline 164
 008A           ;  unsigned char Ackn = 0;                 // Temp RAM for Ackn flag
 008A 0027              clr R16
 008C                   .dbline 166
 008C           ;       
 008C           ;  CLEARBIT(I2C_PORT,SCL);
 008C 4598              cbi 0x8,5
 008E                   .dbline 167
 008E           ;  CLEARBIT(I2C_DIR,SDA);                  // Set SDA to input  
 008E 3C98              cbi 0x7,4
 0090                   .dbline 169
 0090           ;  
 0090           ;  SETBIT(I2C_PORT,SCL);                           // Clock the ACK bit
 0090 459A              sbi 0x8,5
 0092                   .dbline 171
 0092           ; 
 0092           ;  if ( CHECKBIT( I2C_IN,SDA ) ) Ackn = 1;        // Check the ACK bit on SDA   
 0092 349B              sbis 0x6,4
 0094 01C0              rjmp L25
 0096                   .dbline 171
 0096 01E0              ldi R16,1
 0098           L25:
 0098                   .dbline 173
 0098           ;  
 0098           ;  CLEARBIT(I2C_PORT,SCL);                 // Clear the clock
 0098 4598              cbi 0x8,5
 009A                   .dbline 175
 009A           ; 
 009A           ;  return Ackn;                                    // Return our ACK bit
 009A                   .dbline -2
 009A           L24:
 009A                   .dbline 0 ; func end
 009A 0895              ret
 009C                   .dbsym r Ackn 16 c
 009C                   .dbend
 009C                   .dbfunc e I2C_Write _I2C_Write fV
 009C           ;           Data -> R20
 009C           ;          M_ADD -> R22
                        .even
 009C           _I2C_Write::
 009C 00D0              rcall push_gset2
 009E 422F              mov R20,R18
 00A0 602F              mov R22,R16
 00A2                   .dbline -1
 00A2                   .dbline 182
 00A2           ; }     
 00A2           ; 
 00A2           ; 
 00A2           ; 
 00A2           ; 
 00A2           ; void I2C_Write(unsigned char M_ADD, unsigned char Data)
 00A2           ; {
 00A2                   .dbline 183
 00A2           ;  I2C_Start();                                            // Set I2C start condition
 00A2 B3DF              rcall _I2C_Start
 00A4                   .dbline 185
 00A4           ; 
 00A4           ;  Write_I2C_Byte(0x64);                                   // Send the EEPROM internal Address
 00A4 04E6              ldi R16,100
 00A6 C0DF              rcall _Write_I2C_Byte
 00A8                   .dbline 187
 00A8           ; 
 00A8           ;  Write_I2C_Byte(M_ADD);                                  // Send the EEPROM internal Address
 00A8 062F              mov R16,R22
 00AA BEDF              rcall _Write_I2C_Byte
 00AC                   .dbline 189
 00AC           ;   
 00AC           ;  Write_I2C_Byte(Data);                                   // Send the EEPROM Data
 00AC 042F              mov R16,R20
 00AE BCDF              rcall _Write_I2C_Byte
 00B0                   .dbline 191
 00B0           ;  
 00B0           ;  I2C_Stop();                                             // Set I2C Stop condition
 00B0 B1DF              rcall _I2C_Stop
 00B2                   .dbline -2
 00B2           L27:
 00B2 00D0              rcall pop_gset2
 00B4                   .dbline 0 ; func end
 00B4 0895              ret
 00B6                   .dbsym r Data 20 c
 00B6                   .dbsym r M_ADD 22 c
 00B6                   .dbend
 00B6                   .dbfunc e I2C_Read _I2C_Read fc
 00B6           ;           Temp -> R20
 00B6           ;          M_ADD -> R20
                        .even
 00B6           _I2C_Read::
 00B6 00D0              rcall push_gset1
 00B8 402F              mov R20,R16
 00BA                   .dbline -1
 00BA                   .dbline 198
 00BA           ; }
 00BA           ; 
 00BA           ; 
 00BA           ; 
 00BA           ; 
 00BA           ; ubyte I2C_Read(unsigned char M_ADD)
 00BA           ; {
 00BA                   .dbline 201
 00BA           ; ubyte Temp;                                   // Temp RAM for EEPROM Read
 00BA           ;  
 00BA           ;  I2C_Start();                                            // Set I2C start condition
 00BA A7DF              rcall _I2C_Start
 00BC                   .dbline 203
 00BC           ; 
 00BC           ;  Write_I2C_Byte(0x64);                                   // Send the EEPROM internal Address
 00BC 04E6              ldi R16,100
 00BE B4DF           