//
// nRF24L01.c

#include "nRF24L01.h"
#include "..\\def.h"

uchar const TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address
uchar const RX_ADDRESS[RX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static RX address


uchar 	bdata sta;
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;

void inerDelay_us(unsigned char n)
{
	for(;n>0;n--)
		_nop_();
}

void init_io(void)
{
    inerDelay_us(100);
 	CE=0;   // chip enable
 	CSN=1;   // Spi disable 
 	SCK=0;   // Spi clock line init high
}

/**************************************************
Function: SPI_RW();

  Description:
  Writes one byte to nRF24L01, and return the byte read
  from nRF24L01 during write, according to SPI protocol
/**************************************************/
uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (byte & 0x80);         // output 'byte', MSB to MOSI
		byte = (byte << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		byte |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(byte);           		  // return read byte
}
/**************************************************/

/**************************************************
Function: SPI_RW_Reg();

  Description:
  Writes value 'value' to register 'reg'
/**************************************************/
uchar SPI_RW_Reg(BYTE reg, BYTE value)
{
	uchar status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Read();

  Description:
  Read one byte from nRF24L01 register, 'reg'
/**************************************************/
BYTE SPI_Read(BYTE reg)
{
	BYTE reg_val;
	
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}
/**************************************************/

/**************************************************
Function: SPI_Read_Buf();

  Description:
  
/**************************************************/
uchar SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	uchar status,byte_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg);       		// Select register to write to and read status byte
	
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr] = SPI_RW(0);    // 
	
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Write_Buf();

  Description:
  Writes contents of buffer '*pBuf' to nRF24L01
  Typically used to write TX payload, Rx/Tx address
/**************************************************/
uchar SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	uchar status,byte_ctr;
	
	CSN = 0;                   
	status = SPI_RW(reg);   
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) //
		SPI_RW(*pBuf++);
	CSN = 1;                 // Set CSN high again
	return(status);          // 
}
/**************************************************/

/**************************************************
Function: RX_Mode();

  Description:
  
/**************************************************/
void SetRX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // Use the same address on the RX device as the TX device
	
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // 
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // 
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        // 
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); 
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     
	
	CE = 1; 
	inerDelay_us(130);

}
/**************************************************/
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
{
    unsigned char revale=0;

	//SetRX_Mode();

	sta=SPI_Read(STATUS);	// read register STATUS's value
	if(RX_DR)				// if receive data ready (RX_DR) interrupt
	{
	    CE = 0; 
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;//we have receive data
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);// clear RX_DR or TX_DS or MAX_RT interrupt flag
	
	return revale;
}

/**************************************************
Function: nRF24L01_TxPacket();

  Description:
  This function initializes one nRF24L01 device to
  TX mode, set TX address, set RX address for auto.ack,
  fill TX payload, select RF channel, datarate & TX pwr.
  PWR_UP is set, CRC(2 bytes) is enabled, & PRIM:TX.
  
	ToDo: One high pulse(>10us) on CE will now send this
	packet and expext an acknowledgment from the RX device.
/**************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE=0;
	
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); 
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 
	
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // 
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // 
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); //
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        // 
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // 
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);     // 
	CE=1;
	inerDelay_us(10);
	//sta=SPI_Read(STATUS);	// read register STATUS's value
	//SPI_RW_Reg(WRITE_REG+STATUS,SPI_Read(READ_REG+STATUS));	// clear interrupt flag(TX_DS)
	
}
/**************************************************/
