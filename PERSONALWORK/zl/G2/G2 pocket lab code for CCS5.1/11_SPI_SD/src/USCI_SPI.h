/*
 * USCI_SPI.h
 *
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#ifndef USCI_SPI_H_
#define USCI_SPI_H_

extern void SPI_CS_High(void);
extern void SPI_CS_Low(void);
extern void USCI_A0_init(void);
extern unsigned char SPI_SendFrame(unsigned char  *pBuffer, unsigned int   size);
extern unsigned char SPI_RecieveFrame(unsigned char  *pBuffer, unsigned int size);
extern unsigned char Empty_SD_Oper;

#endif /* USCI_SPI_H_ */
