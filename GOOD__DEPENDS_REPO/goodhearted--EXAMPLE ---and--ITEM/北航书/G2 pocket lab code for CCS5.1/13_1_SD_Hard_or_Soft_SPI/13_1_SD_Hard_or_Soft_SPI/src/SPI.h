/*
 * SPI.h
 *
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#ifndef SPI_H_
#define SPI_H_

//#define	HARD_SPI	//条件编译，HARD_SPI启用硬件SPI代码
#define	SOFT_SPI			//条件编译，SOFT_SPI启用软件SPI代码
extern void SPI_CS_High(void);
extern void SPI_CS_Low(void);
extern void SPI_HighSpeed(void);
extern void SPI_LowSpeed(void);
extern void SPI_init(void);
extern unsigned char SPI_TxFrame(unsigned char  *pBuffer, unsigned int   size);
extern unsigned char SPI_RxFrame(unsigned char  *pBuffer, unsigned int size);

#endif /* SPI_H_ */
