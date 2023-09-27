	.module I2C.c
	.area text(rom, con, rel)
	.dbfile D:\OLED\I2C.c
	.dbfunc e Init_I2C _Init_I2C fV
	.even
_Init_I2C::
	.dbline -1
	.dbline 17
; //ICC-AVR application builder : 2006-02-12 14:00:00
; // Target : ATmega48
; // Crystal: 1.000Mhz
; // Author:  jackyan
; 
; #include "iom48v.h"
; #include "macros.h"
; #include "Defs.h"
; #include "I2C.h"
; 
; 
; // ******************************************** //
; // *** This routine will setup the I2C Port *** //
; // ******************************************** //
; 
; void Init_I2C(void)
; {
	.dbline 18
;  SETBIT(I2C_DIR,SCL);   	   // Set SCL to output	
	sbi 0x7,5
	.dbline 19
;  SETBIT(I2C_DIR,SDA );	   // Set SDA to output
	sbi 0x7,4
	.dbline 21
; 	
;  SETBIT(I2C_PORT,SCL);		   // Set SCL high
	sbi 0x8,5
	.dbline 22
;  SETBIT(I2C_PORT,SDA );		   // Set SDA high
	sbi 0x8,4
	.dbline -2
L1:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e I2C_Start _I2C_Start fV
	.even
_I2C_Start::
	.dbline -1
	.dbline 32
; }
; 
; 
; 
; // ************************************************ //
; // *** This routine will send the I2C Start Bit *** //
; // ************************************************ //
; 
; void I2C_Start (void)
; {
	.dbline 33
;  SETBIT(I2C_PORT,SCL);				// Set SCL High
	sbi 0x8,5
	.dbline 34
;  SETBIT(I2C_PORT,SDA);				// Set SDA High
	sbi 0x8,4
	.dbline 36
;  
;  SETBIT(I2C_DIR,SDA);   		   // Set SDA to output	
	sbi 0x7,4
	.dbline 37
;  CLEARBIT(I2C_PORT,SDA);		   // Clear SDA
	cbi 0x8,4
	.dbline -2
L2:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e I2C_Stop _I2C_Stop fV
	.even
_I2C_Stop::
	.dbline -1
	.dbline 46
; }
; 
; 
; // *********************************************** //
; // *** This routine will send the I2C Stop Bit *** //
; // *********************************************** //
; 
; void I2C_Stop (void)
; {
	.dbline 48
;  
;  SETBIT(I2C_DIR,SDA);   		   // Set SDA to output
	sbi 0x7,4
	.dbline 50
; 	
;  CLEARBIT(I2C_PORT,SDA);		   // Clear SDA
	cbi 0x8,4
	.dbline 51
;  CLEARBIT(I2C_PORT,SCL);		   // Clear SCL
	cbi 0x8,5
	.dbline 53
; 	
;  SETBIT(I2C_PORT,SCL);			   // Set SCL High
	sbi 0x8,5
	.dbline 54
;  SETBIT(I2C_PORT,SDA);			   // Set SDA High
	sbi 0x8,4
	.dbline -2
L3:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e I2C_Clock _I2C_Clock fV
	.even
_I2C_Clock::
	.dbline -1
	.dbline 66
; }
; 
; 
; 
; 
; 
; // ************************************************** //
; // *** This routine will send the I2C clock pulse *** //
; // ************************************************** //
; 
; void I2C_Clock (void)
; {
	.dbline 67
;  SETBIT(I2C_PORT,SCL);				// Set SCL high
	sbi 0x8,5
	.dbline 70
; // NOP();								// Small Delay
; // NOP();								// Small Delay
;  NOP();								// Small Delay
	nop
	.dbline 71
;  CLEARBIT(I2C_PORT,SCL);			// Set SCL low
	cbi 0x8,5
	.dbline -2
L4:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e Write_I2C_Byte _Write_I2C_Byte fV
;              i -> R20
;           byte -> R22
	.even
_Write_I2C_Byte::
	rcall push_gset2
	mov R22,R16
	.dbline -1
	.dbline 82
; }
; 
; 
; 
; 
; // ******************************************************** //
; // *** This routine will write a byte to the I2C device *** //
; // ******************************************************** //
; 
; void Write_I2C_Byte(unsigned char byte)
; {
	.dbline 85
;  unsigned char i;
; 
;  SETBIT(I2C_DIR,SDA);   		   // Set SDA to output	
	sbi 0x7,4
	.dbline 86
;  CLEARBIT(I2C_PORT,SCL);
	cbi 0x8,5
	.dbline 88
	clr R20
	rjmp L9
L6:
	.dbline 88
;  	
;  for (i = 0; i < 8; i++){  		 // Loop for our 8 bits
	.dbline 91
;  
; 	// Set or Clear SDA pin
; 	 if((byte & 0x80) == 0x80) SETBIT(I2C_PORT, SDA);	// Set I2C SDA PIN
	mov R24,R22
	andi R24,128
	cpi R24,128
	brne L10
	.dbline 91
	sbi 0x8,4
	rjmp L11
L10:
	.dbline 92
; 	 else CLEARBIT(I2C_PORT,SDA);		  				// Clear I2C SDA PIN
	cbi 0x8,4
L11:
	.dbline 94
	sbi 0x8,5
	.dbline 96
	lsl R22
	.dbline 103
	cbi 0x8,5
	.dbline 104
L7:
	.dbline 88
	inc R20
L9:
	.dbline 88
	cpi R20,8
	brlo L6
	.dbline 106
; 	 
; 	 SETBIT(I2C_PORT,SCL);			   // Set SCL High, Clock data
; 
; 	 byte = byte << 1;				   // Shift data in buffer right one
; 
; //	NOP();
; //	NOP();
; //	NOP();
; //	NOP();
; 
;      CLEARBIT(I2C_PORT,SCL);		   // Clear SCL
; 	}
;  
; i=255;
	ldi R20,255
	rjmp L13
L12:
	.dbline 108
;  while(I2C_Ackn() )				   // Check for acknowledge from I2C device	
;  	{
	.dbline 109
;  	i--;
	dec R20
	.dbline 110
; 	if(i==0)
	brne L15
	.dbline 111
; 		break;
	rjmp L14
L15:
	.dbline 112
L13:
	.dbline 107
	rcall _I2C_Ackn
	tst R16
	brne L12
L14:
	.dbline -2
L5:
	rcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r i 20 c
	.dbsym r byte 22 c
	.dbend
	.dbfunc e Read_I2C_Byte _Read_I2C_Byte fc
;              i -> R20
;           buff -> R22
	.even
_Read_I2C_Byte::
	rcall push_gset2
	.dbline -1
	.dbline 124
;  	}
; }
; 
; 
; 
; 
; 
; // ********************************************************* //
; // *** This routine will read a byte from the I2C device *** //
; // ********************************************************* //
; 
; unsigned char Read_I2C_Byte(void)
; {	
	.dbline 125
;  unsigned char i,buff = 0;
	clr R22
	.dbline 127
	clr R20
	rjmp L21
L18:
	.dbline 127
; 	
; 	for (i = 0; i < 8; i++){
	.dbline 129
; 	
; 		buff = buff << 1;
	lsl R22
	.dbline 131
; 		
;  		CLEARBIT(I2C_DIR,SDA);   		   // Set SDA to input
	cbi 0x7,4
	.dbline 133
; 				
; 		SETBIT(I2C_PORT,SCL);	  	  	   // Set SCL High,Clock bit out
	sbi 0x8,5
	.dbline 136
; //	NOP();
; //	NOP();
; 	NOP();
	nop
	.dbline 139
; 	
; 		// Read data on SDA pin
; 		if ( CHECKBIT( I2C_IN,SDA ) ) buff = buff | 0x01;	
	sbis 0x6,4
	rjmp L22
	.dbline 139
	ori R22,1
L22:
	.dbline 141
	cbi 0x8,5
	.dbline 145
	nop
	.dbline 146
L19:
	.dbline 127
	inc R20
L21:
	.dbline 127
	cpi R20,8
	brlo L18
	.dbline 150
; 		
; 		CLEARBIT(I2C_PORT,SCL);	  	  	   // Clear SCL 	
; 
; //	NOP();
; //	NOP();
; 	NOP();
;         }
; 
;  // *** No ACK *** //
;  
;  I2C_Clock();						       // Clock I2C bit
	rcall _I2C_Clock
	.dbline 152
; 	
;  return buff;							   // Return our I2C byte
	mov R16,R22
	.dbline -2
L17:
	rcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r i 20 c
	.dbsym r buff 22 c
	.dbend
	.dbfunc e I2C_Ackn _I2C_Ackn fc
;           Ackn -> R16
	.even
_I2C_Ackn::
	.dbline -1
	.dbline 163
; }
; 
; 
; 
; 
; // *********************************************************************** //
; // *** This routine returns a 0 if the I2C device sends an acknowledge *** //
; // *********************************************************************** //
; 
; unsigned char I2C_Ackn(void)
; {
	.dbline 164
;  unsigned char Ackn = 0;		   // Temp RAM for Ackn flag
	clr R16
	.dbline 166
; 	
;  CLEARBIT(I2C_PORT,SCL);
	cbi 0x8,5
	.dbline 167
;  CLEARBIT(I2C_DIR,SDA);   		   // Set SDA to input	
	cbi 0x7,4
	.dbline 169
;  
;  SETBIT(I2C_PORT,SCL);			   // Clock the ACK bit
	sbi 0x8,5
	.dbline 171
; 
;  if ( CHECKBIT( I2C_IN,SDA ) ) Ackn = 1;	  // Check the ACK bit on SDA	
	sbis 0x6,4
	rjmp L25
	.dbline 171
	ldi R16,1
L25:
	.dbline 173
;  
;  CLEARBIT(I2C_PORT,SCL);	  	   // Clear the clock
	cbi 0x8,5
	.dbline 175
; 
;  return Ackn;					   // Return our ACK bit
	.dbline -2
L24:
	.dbline 0 ; func end
	ret
	.dbsym r Ackn 16 c
	.dbend
	.dbfunc e I2C_Write _I2C_Write fV
;           Data -> R20
;          M_ADD -> R22
	.even
_I2C_Write::
	rcall push_gset2
	mov R20,R18
	mov R22,R16
	.dbline -1
	.dbline 182
; }	
; 
; 
; 
; 
; void I2C_Write(unsigned char M_ADD, unsigned char Data)
; {
	.dbline 183
;  I2C_Start();			   			   // Set I2C start condition
	rcall _I2C_Start
	.dbline 185
; 
;  Write_I2C_Byte(0x64);				   // Send the EEPROM internal Address
	ldi R16,100
	rcall _Write_I2C_Byte
	.dbline 187
; 
;  Write_I2C_Byte(M_ADD);				   // Send the EEPROM internal Address
	mov R16,R22
	rcall _Write_I2C_Byte
	.dbline 189
;   
;  Write_I2C_Byte(Data);				   // Send the EEPROM Data
	mov R16,R20
	rcall _Write_I2C_Byte
	.dbline 191
;  
;  I2C_Stop();	  					   // Set I2C Stop condition
	rcall _I2C_Stop
	.dbline -2
L27:
	rcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r Data 20 c
	.dbsym r M_ADD 22 c
	.dbend
	.dbfunc e I2C_Read _I2C_Read fc
;           Temp -> R20
;          M_ADD -> R20
	.even
_I2C_Read::
	rcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 198
; }
; 
; 
; 
; 
; ubyte I2C_Read(unsigned char M_ADD)
; {
	.dbline 201
; ubyte Temp;			   		// Temp RAM for EEPROM Read
;  
;  I2C_Start();			   			   // Set I2C start condition
	rcall _I2C_Start
	.dbline 203
; 
;  Write_I2C_Byte(0x64);				   // Send the EEPROM internal Address
	ldi R16,100
	rcall _Write_I2C_Byte
	.dbline 205
; 
;  Write_I2C_Byte(M_ADD);				   // Send the EEPROM internal Address
	mov R16,R20
	rcall _Write_I2C_Byte
	.dbline 207
;  
;  I2C_Start();			   			   // Set I2C start condition
	rcall _I2C_Start
	.dbline 209
;  
;   Write_I2C_Byte(0x65);				   // Send the EEPROM internal Address
	ldi R16,101
	rcall _Write_I2C_Byte
	.dbline 211
; 
;  Temp = Read_I2C_Byte();			   // Read data from EEPROM
	rcall _Read_I2C_Byte
	mov R20,R16
	.dbline 213
;  
;  I2C_Stop();	  					   // Set I2C Stop condition
	rcall _I2C_Stop
	.dbline 215
; 
;  return Temp;						   // Return data from EEPROM
	mov R16,R20
	.dbline -2
L28:
	rcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r Temp 20 c
	.dbsym r M_ADD 20 c
	.dbend
