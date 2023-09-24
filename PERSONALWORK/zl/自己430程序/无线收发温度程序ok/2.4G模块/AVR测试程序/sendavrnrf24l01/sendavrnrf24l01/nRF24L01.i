002F:nRF24L01.s:06D9:1:14=7C526E00:16=47d21313:	.module nRF24L01.c
	.area data(ram, con, rel)
_TX_ADDRESS::
	.blkb 2
	.area idata
	.byte 52,'C
	.area data(ram, con, rel)
	.blkb 2
	.area idata
	.byte 16,16
	.area data(ram, con, rel)
	.blkb 1
	.area idata
	.byte 1
	.area data(ram, con, rel)
	.dbfile E:\avrnrf24l01\nRF24L01.c
	.dbsym e TX_ADDRESS _TX_ADDRESS A[5:5]c
	.area text(rom, con, rel)
	.dbfile E:\avrnrf24l01\nRF24L01.c
	.dbfunc e nRF24L01_Initial _nRF24L01_Initial fV
	.even
_nRF24L01_Initial::
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e nRF24L01ioConfig _nRF24L01ioConfig fV
	.even
_nRF24L01ioConfig::
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e SPI_RW _SPI_RW fc
;           byte -> R0
	.even
_SPI_RW::
	.dbline 0 ; func end
	ret
	.dbsym l byte 0 c
	.dbend
	.dbfunc e SPI_RW_Reg _SPI_RW_Reg fc
;          value -> R1
;            reg -> R0
	.even
_SPI_RW_Reg::
	.dbline 0 ; func end
	ret
	.dbsym l value 2 c
	.dbsym l reg 0 c
	.dbend
	.dbfunc e SPI_Read _SPI_Read fc
;            reg -> R0
	.even
_SPI_Read::
	.dbline 0 ; func end
	ret
	.dbsym l reg 0 c
	.dbend
	.dbfunc e SPI_Read_Buf _SPI_Read_Buf fc
;          bytes -> R2
;           pBuf -> R1,R2
;            reg -> R0
	.even
_SPI_Read_Buf::
	.dbline 0 ; func end
	ret
	.dbsym l bytes 4 c
	.dbsym l pBuf 2 pc
	.dbsym l reg 0 c
	.dbend
	.dbfunc e SPI_Write_Buf _SPI_Write_Buf fc
;          bytes -> R2
;           pBuf -> R1,R2
;            reg -> R0
	.even
_SPI_Write_Buf::
	.dbline 0 ; func end
	ret
	.dbsym l bytes 4 c
	.dbsym l pBuf 2 pc
	.dbsym l reg 0 c
	.dbend
	.dbfunc e RX_Mode _RX_Mode fV
	.even
_RX_Mode::
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e TX_Mode _TX_Mode fV
	.even
_TX_Mode::
	.dbline 0 ; func end
	ret
	.dbend
0031:nRF24L01._c:01CBB:1:14=7C526E00:16=47d21313:#include "iom48v.h"
#include "macros.h"
#include "Defs.h"
#include "nRF24L01.h"  



extern unsigned char Buffer[16]  ; // Define a static TX address


unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address


void nRF24L01_Initial(void)
{
	nRF24L01_CE=0;			// chip enable
	nRF24L01_CSN=1;			// Spi disable
	nRF24L01_SCK=0;			// Spi clock line init high
	
	
}
void nRF24L01ioConfig(void)//LCD initial IO.
{
	PM2	&=0X0F1;		//端口模式设置	1 (输入)  P2.1~P2.3都设置为输出
	PU2	&=0X0F1;		//端口上拉设置	1 (上拉)  P2.1~P2.3都设置为无上拉
//	PM13	&=0X0FE;		//端口模式设置	1 (输入)  P13.0设置为输出
//	PU13	&=0X0FE;		//端口上拉设置	1 (上拉)  P13.0设置为无上拉

	PM4	|=0X05;			//端口模式设置	1 (输入)  P4.2都设置为输入
	PU4	|=0X05;			//端口上拉设置	1 (上拉)  P4.2都设置为有上拉	
	nRF24L01_Initial();
}
/**************************************************
Function: SPI_RW();

Description:
  Writes one byte to nRF24L01, and return the byte read
  from nRF24L01 during write, according to SPI protocol  */
/**************************************************/
unsigned char SPI_RW(unsigned char byte)
{
	unsigned char  bit_ctr;	
	DI();
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   	// output 8-bit
   	{
   		if(byte & 0x80) 
   			nRF24L01_MOSI = 1;
   		else 
   			nRF24L01_MOSI = 0;
   		byte = (byte << 1);           		// shift next bit into MSB..
   		nRF24L01_SCK = 1;                   // Set nRF24L01_SCK high..
   		if(nRF24L01_MISO) byte |= 1;
   		nRF24L01_SCK = 0;            		// ..then set nRF24L01_SCK low again
   	}
   	EI();
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
//	DI();
	nRF24L01_CSN = 0;          // nRF24L01_CSN low, init SPI transaction.
	status = SPI_RW(reg);      // select register
  	SPI_RW(value);             // ..and write value to it..
  	nRF24L01_CSN = 1;                   // nRF24L01_CSN high again
//  EI();
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
//	DI();
	nRF24L01_CSN = 0;       	// nRF24L01_CSN low, initialize SPI communication...
  	SPI_RW(reg);            // Select register to read from..
  	reg_val = SPI_RW(0);    // ..then read registervalue
    nRF24L01_CSN = 1;       	// nRF24L01_CSN high, terminate SPI communication
 // 	EI();
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
//	DI();
	nRF24L01_CSN = 0;                   // Set nRF24L01_CSN low, init SPI tranaction
  	status = SPI_RW(reg);       		// Select register to write to and read status byte
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0);    // Perform SPI_RW to read byte from nRF24L01
	nRF24L01_CSN = 1;                  // Set nRF24L01_CSN high again
//  EI();
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
//	DI();
	nRF24L01_CSN = 0;                    		// Set nRF24L01_CSN low, init SPI tranaction
  	status = SPI_RW(reg);    					// Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	status = SPI_RW(*pBuf++);
	nRF24L01_CSN = 1;                           // Set nRF24L01_CSN high again
//  	EI();
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
	nRF24L01_CE=0;
//	DI();
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device

  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
//	EI();
  	nRF24L01_CE=1;
	//  This device is now ready to receive one packet of 16 bytes payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.

}
/**************************************************/

/**************************************************
Function: TX_Mode();

Description:
  This function initializes one nRF24L01 device to
