// ***************     I2C driver V 1.0    ***************
// ***           Written By P. Fletcher-Jones          ***
// ***                Written on 06/6/01			   ***
// ***                 Last MOD 15/11/01			   ***
// ***       Compiled using ImageCraft C Comiler       ***
// *******************************************************


// ****************************** //
// *** I2C Hardware Interface *** //
// ****************************** //
#ifndef i2c_h
#define i2c_h

#define SDA BIT4
#define SCL BIT5

#define I2C_PORT	PORTC
#define I2C_DIR		DDRC
#define I2C_IN		PINC

void Init_I2C(void);
void I2C_Start (void);
void I2C_Stop (void);
void I2C_Clock (void);
unsigned char I2C_Ackn(void);
void Write_I2C_Control (unsigned char,unsigned char,unsigned char);
void Write_I2C_Byte(unsigned char);
unsigned char Read_I2C_Byte(void);

void I2C_Write(unsigned char M_ADD, unsigned char Data);
ubyte I2C_Read(unsigned char M_ADD);

#endif
