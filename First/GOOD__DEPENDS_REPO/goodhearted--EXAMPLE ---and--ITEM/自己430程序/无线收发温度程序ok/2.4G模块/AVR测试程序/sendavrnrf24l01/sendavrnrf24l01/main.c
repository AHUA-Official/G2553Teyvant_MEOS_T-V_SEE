//ICC-AVR application builder : 2006-02-12 14:00:00
// Target : ATmega48
// Crystal: 8.000Mhz
// Author:  jackyan
// Oled Type : white
//#define fosc 8000000
//#define baud 9600
#include "iom48v.h"
#include "macros.h"
#include "defs.h"
/*-----------------------------------------------------------------------------
   Global Defines
------------------------------------------------------------------------------*/
unsigned char   key_debug;
unsigned char 	Buffer[]={
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
#define TX_ADR_WIDTH    5   // 5 bytes TX(RX) address width
#define TX_PLOAD_WIDTH  20  // 16 bytes TX payload
unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address
void init_CPU (void);
void delayms(unsigned short	dly);//当dly=1时，延时的时间是1ms  4MHz晶震
void INIT_io(void);
void RX_Mode(void);
void TX_Mode(void);
void delay(void);
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value);
unsigned char SPI_Read(unsigned char reg);
void clear_buf(unsigned char	*ptr,unsigned char number);
unsigned char accept_flag=0;
unsigned int accept_time=0;
//****************************************************************//
// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
//#define NOP             0xFF  // Define No Operation, might be used to read status register
//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define MAX_RT  	0x10  // Max #of TX retrans interrupt
#define TX_DS   	0x20  // TX data sent interrupt
#define RX_DR   	0x40  // RX data received
//-----------------------------------------------------------------------------
//------------------------------------------------------
#define BIT(x)	           (1 << (x))
#define SETBIT(x, y)   	   (x |= y)
#define CLEARBIT(x, y) 	   (x &= ~y)
#define CHECKBIT(x, y) 	   (x & y)
#define BIT7 0x80
#define BIT6 0x40
#define BIT5 0x20
#define BIT4 0x10
#define BIT3 0x08
#define BIT2 0x04
#define BIT1 0x02
#define BIT0 0x01
/*#define nRF24L01_CSN        BIT4
#define nRF24L01_SCK        BIT5
#define nRF24L01_MOSI       BIT6
#define nRF24L01_CE         BIT3
#define nRF24L01_MISO 	    BIT7
#define nRF24L01_IRQ  	    BIT0
#define nRF24L01_CSNH       SETBIT(PORTD, BIT4)     
#define nRF24L01_CSNL       CLEARBIT(PORTD, BIT4)   

#define nRF24L01_SCKH       SETBIT(PORTD,BIT5)      
#define nRF24L01_SCKL       CLEARBIT(PORTD,BIT5)    

#define nRF24L01_MOSIH      SETBIT(PORTD,BIT6)     
#define nRF24L01_MOSIL	    CLEARBIT(PORTD,BIT6)   

#define nRF24L01_CEH        SETBIT(PORTD,BIT3)     
#define nRF24L01_CEL        CLEARBIT(PORTD,BIT3) */ 
//-----------------------------------------------------------------------------
#define nRF24L01_CSN        BIT6
#define nRF24L01_SCK        BIT1
#define nRF24L01_MOSI       BIT0
#define nRF24L01_CE         BIT7
#define nRF24L01_MISO 	    BIT3
#define nRF24L01_IRQ  	    BIT2
#define nRF24L01_CSNH       SETBIT(PORTD, BIT6)     
#define nRF24L01_CSNL       CLEARBIT(PORTD, BIT6)   

#define nRF24L01_SCKH       SETBIT(PORTB,BIT1)      
#define nRF24L01_SCKL       CLEARBIT(PORTB,BIT1)    

#define nRF24L01_MOSIH      SETBIT(PORTB,BIT0)     
#define nRF24L01_MOSIL	    CLEARBIT(PORTB,BIT0)   

#define nRF24L01_CEH        SETBIT(PORTD,BIT7)     
#define nRF24L01_CEL        CLEARBIT(PORTD,BIT7)
//-----------------------------------------------------------------------------
void main(void)
{
 	unsigned char Get_SO=0;
    CLI();                   /* global interrupt disable */
    init_CPU ();
    delayms(100);	//延时10ms*10=100ms
    delayms(100);	//延时10ms*10=100ms
	RX_Mode();
	while(1)
	{		
		//	if(!(PINB& 0x01))
		if(!(PINB& 0x04))
		{//nRF24L01 接收数据
			key_debug=SPI_Read(STATUS);			// read register STATUS's value
			if(key_debug&RX_DR)					// if renRF24L01_CEive data ready (RX_DR) interrupt
				SPI_Read_Buf(RD_RX_PLOAD,Buffer,TX_PLOAD_WIDTH);// read renRF24L01_CEive payload from RX_FIFO buffer
			if(key_debug&MAX_RT) SPI_RW_Reg(FLUSH_TX,0);
			SPI_RW_Reg(WRITE_REG+STATUS,0xff);// clear RX_DR or TX_DS or MAX_RT interrupt flag
			RX_Mode();
			if((Buffer[0]==80)&&(Buffer[1]==01))//data accept
			{
			 	accept_flag=1;
			 	PORTC = 0x02;
				delayms(5000);	//延时10ms*10=100ms
				PORTC = 0x00;
			}
		}
		if(accept_flag==0)//data send
		{
		 		accept_flag=1;
				accept_time=0;
		 		Buffer[0]=80;
				Buffer[1]=02;
				TX_Mode();			// set TX Mode and transmitting
				Buffer[0]=80;
				Buffer[1]=02;
				TX_Mode();			// set TX Mode and transmitting
				delayms(100);
		}
		if(accept_flag=1)
		{
		   accept_time++;
		   delayms(100);
		   if(accept_time>1000)
		   {
		   	 accept_flag=0;
			 accept_time=0;
		   }
		}	
	}


}
/*-----------------------------------------------------------------------------
   Module:   init_CPU
   Function: Initialization of CPU
------------------------------------------------------------------------------*/
void init_CPU (void)
{
 	MCUCR = 0x00;	// 
	EICRA = 0x00; //extended ext ints
    EIMSK = 0x00;
	 
	TIMSK0 = 0x01; //timer 0 interrupt sources
	TIMSK1 = 0x00; //timer 1 interrupt sources
	TIMSK2 = 0x00; //timer 2 interrupt sources
	 CLI(); //disable all interrupts
	 NOP();
	PORTB = 0x81;
 	DDRB  = 0xf3;
	PORTD = 0x80;
	DDRD  = 0xff;
	PORTC = 0x80;
	DDRC  = 0x78;
    nRF24L01_CEL;
	delay();
	nRF24L01_CSNH;			// Spi disable
	delay();
	nRF24L01_SCKL;	
	delay();
}
void delay(void)//
{
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
}
void delayms(unsigned short	dly)//
{
	for(;dly>0;dly--) ;
}

void clear_buf(unsigned char	*ptr,unsigned char number)//清零buffer,指定字节个数
{
	for (;number>0;number--) *ptr++=0;
}
//------------------------------------------------------
/**************************************************
Function: SPI_RW();

Description:
  Writes one byte to nRF24L01, and return the byte read
  from nRF24L01 during write, according to SPI protocol  */
/**************************************************/
unsigned char SPI_RW(unsigned char byte)
{
	unsigned char  bit_ctr;	
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   	// output 8-bit
   	{
   		if(byte & 0x80) 
   			nRF24L01_MOSIH;
   		else 
   			nRF24L01_MOSIL;
		delay();
   		byte = (byte << 1);           		// shift next bit into MSB..
   		nRF24L01_SCKH;
		delay();
 		if(PINB&0x08) byte |= 1;
   		nRF24L01_SCKL;
		delay();
   	}
    return(byte);           		  		// return read byte
}
/**************************************************
Function: SPI_RW_Reg();

Description:
  Writes value 'value' to register 'reg' */
/**************************************************/
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
	nRF24L01_CSNL;
	delay();
	status = SPI_RW(reg);      // select register
  	SPI_RW(value);             // ..and write value to it..
	nRF24L01_CSNH;
	delay();
  	return(status);            // return nRF24L01 status byte
}
/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'  */
/**************************************************/
unsigned char SPI_Read(unsigned char reg)
{
	unsigned char reg_val;
	nRF24L01_CSNL;
	delay();
  	SPI_RW(reg);            // Select register to read from..
  	reg_val = SPI_RW(0);    // ..then read registervalue
	nRF24L01_CSNH;
	delay();
  	return(reg_val);        // return register value
}
/**************************************************
Function: SPI_Read_Buf();

Description:
  Reads 'bytes' #of bytes from register 'reg'
  Typically used to read RX payload, Rx/Tx address */
/**************************************************/
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
	unsigned char status,byte_ctr;
	nRF24L01_CSNL;
	delay();
  	status = SPI_RW(reg);       		// Select register to write to and read status byte
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0);    // Perform SPI_RW to read byte from nRF24L01
	nRF24L01_CSNH;
	delay();
  	return(status);                    // return nRF24L01 status byte
}
/**************************************************
Function: SPI_Write_Buf();

Description:
  Writes contents of buffer '*pBuf' to nRF24L01
  Typically used to write TX payload, Rx/Tx address */
/**************************************************/
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
	unsigned char status,byte_ctr;
	nRF24L01_CSNL;                   		// Set nRF24L01_CSN low, init SPI tranaction
  	delay();
	status = SPI_RW(reg);    					// Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	status = SPI_RW(*pBuf++);            
	nRF24L01_CSNH; 								// Set nRF24L01_CSN high again
	delay();
  	return(status);          					// return nRF24L01 status byte
}
/**************************************************
Function: RX_Mode();

Description:
  This function initializes one nRF24L01 device to
  RX Mode, set RX address, writes RX payload width,
  select RF channel, datarate & LNA HCURR.
  After init, CE is toggled high, which means that
  this device is now ready to receive a datapacket. */
/**************************************************/
void RX_Mode(void)
{
	nRF24L01_CEL;
	delay();
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device

  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
  	nRF24L01_CEH;
	delay();
	//  This device is now ready to receive one packet of 16 bytes payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.

}
/**************************************************/

/**************************************************
Function: TX_Mode();

Description:
  This function initializes one nRF24L01 device to
  TX mode, set TX address, set RX address for auto.ack,
  fill TX payload, select RF channel, datarate & TX pwr.
  PWR_UP is set, CRC(2 bytes) is enabled, & PRIM:TX.

  ToDo: One high pulse(>10us) on CE will now send this
  packet and expext an acknowledgment from the RX device. */
/**************************************************/
void TX_Mode(void)
{
	nRF24L01_CEL;
	delay();
  	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
  	SPI_Write_Buf(WR_TX_PLOAD, Buffer, TX_PLOAD_WIDTH); // Writes data to TX payload

  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
	nRF24L01_CEH;
	delay();
	
}
//------------------------------------------------------
