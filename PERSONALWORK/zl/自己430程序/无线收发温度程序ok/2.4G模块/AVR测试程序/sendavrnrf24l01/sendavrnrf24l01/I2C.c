//ICC-AVR application builder : 2006-02-12 14:00:00
// Target : ATmega48
// Crystal: 1.000Mhz
// Author:  jackyan

#include "iom48v.h"
#include "macros.h"
#include "Defs.h"
#include "I2C.h"


// ******************************************** //
// *** This routine will setup the I2C Port *** //
// ******************************************** //

void Init_I2C(void)
{
 SETBIT(I2C_DIR,SCL);   	   // Set SCL to output	
 SETBIT(I2C_DIR,SDA );	   // Set SDA to output
	
 SETBIT(I2C_PORT,SCL);		   // Set SCL high
 SETBIT(I2C_PORT,SDA );		   // Set SDA high
}



// ************************************************ //
// *** This routine will send the I2C Start Bit *** //
// ************************************************ //

void I2C_Start (void)
{
 SETBIT(I2C_PORT,SCL);				// Set SCL High
 SETBIT(I2C_PORT,SDA);				// Set SDA High
 
 SETBIT(I2C_DIR,SDA);   		   // Set SDA to output	
 CLEARBIT(I2C_PORT,SDA);		   // Clear SDA
}


// *********************************************** //
// *** This routine will send the I2C Stop Bit *** //
// *********************************************** //

void I2C_Stop (void)
{
 
 SETBIT(I2C_DIR,SDA);   		   // Set SDA to output
	
 CLEARBIT(I2C_PORT,SDA);		   // Clear SDA
 CLEARBIT(I2C_PORT,SCL);		   // Clear SCL
	
 SETBIT(I2C_PORT,SCL);			   // Set SCL High
 SETBIT(I2C_PORT,SDA);			   // Set SDA High
}





// ************************************************** //
// *** This routine will send the I2C clock pulse *** //
// ************************************************** //

void I2C_Clock (void)
{
 SETBIT(I2C_PORT,SCL);				// Set SCL high
// NOP();								// Small Delay
// NOP();								// Small Delay
 NOP();								// Small Delay
 CLEARBIT(I2C_PORT,SCL);			// Set SCL low
}




// ******************************************************** //
// *** This routine will write a byte to the I2C device *** //
// ******************************************************** //

void Write_I2C_Byte(unsigned char byte)
{
 unsigned char i;

 SETBIT(I2C_DIR,SDA);   		   // Set SDA to output	
 CLEARBIT(I2C_PORT,SCL);
 	
 for (i = 0; i < 8; i++){  		 // Loop for our 8 bits
 
	// Set or Clear SDA pin
	 if((byte & 0x80) == 0x80) SETBIT(I2C_PORT, SDA);	// Set I2C SDA PIN
	 else CLEARBIT(I2C_PORT,SDA);		  				// Clear I2C SDA PIN
	 
	 SETBIT(I2C_PORT,SCL);			   // Set SCL High, Clock data

	 byte = byte << 1;				   // Shift data in buffer right one

//	NOP();
//	NOP();
//	NOP();
//	NOP();

     CLEARBIT(I2C_PORT,SCL);		   // Clear SCL
	}
 
i=255;
 while(I2C_Ackn() )				   // Check for acknowledge from I2C device	
 	{
 	i--;
	if(i==0)
		break;
 	}
}





// ********************************************************* //
// *** This routine will read a byte from the I2C device *** //
// ********************************************************* //

unsigned char Read_I2C_Byte(void)
{	
 unsigned char i,buff = 0;
	
	for (i = 0; i < 8; i++){
	
		buff = buff << 1;
		
 		CLEARBIT(I2C_DIR,SDA);   		   // Set SDA to input
				
		SETBIT(I2C_PORT,SCL);	  	  	   // Set SCL High,Clock bit out
//	NOP();
//	NOP();
	NOP();
	
		// Read data on SDA pin
		if ( CHECKBIT( I2C_IN,SDA ) ) buff = buff | 0x01;	
		
		CLEARBIT(I2C_PORT,SCL);	  	  	   // Clear SCL 	

//	NOP();
//	NOP();
	NOP();
        }

 // *** No ACK *** //
 
 I2C_Clock();						       // Clock I2C bit
	
 return buff;							   // Return our I2C byte
}




// *********************************************************************** //
// *** This routine returns a 0 if the I2C device sends an acknowledge *** //
// *********************************************************************** //

unsigned char I2C_Ackn(void)
{
 unsigned char Ackn = 0;		   // Temp RAM for Ackn flag
	
 CLEARBIT(I2C_PORT,SCL);
 CLEARBIT(I2C_DIR,SDA);   		   // Set SDA to input	
 
 SETBIT(I2C_PORT,SCL);			   // Clock the ACK bit

 if ( CHECKBIT( I2C_IN,SDA ) ) Ackn = 1;	  // Check the ACK bit on SDA	
 
 CLEARBIT(I2C_PORT,SCL);	  	   // Clear the clock

 return Ackn;					   // Return our ACK bit
}	




void I2C_Write(unsigned char M_ADD, unsigned char Data)
{
 I2C_Start();			   			   // Set I2C start condition

 Write_I2C_Byte(0x64);				   // Send the EEPROM internal Address

 Write_I2C_Byte(M_ADD);				   // Send the EEPROM internal Address
  
 Write_I2C_Byte(Data);				   // Send the EEPROM Data
 
 I2C_Stop();	  					   // Set I2C Stop condition
}




ubyte I2C_Read(unsigned char M_ADD)
{
ubyte Temp;			   		// Temp RAM for EEPROM Read
 
 I2C_Start();			   			   // Set I2C start condition

 Write_I2C_Byte(0x64);				   // Send the EEPROM internal Address

 Write_I2C_Byte(M_ADD);				   // Send the EEPROM internal Address
 
 I2C_Start();			   			   // Set I2C start condition
 
  Write_I2C_Byte(0x65);				   // Send the EEPROM internal Address

 Temp = Read_I2C_Byte();			   // Read data from EEPROM
 
 I2C_Stop();	  					   // Set I2C Stop condition

 return Temp;						   // Return data from EEPROM
}




