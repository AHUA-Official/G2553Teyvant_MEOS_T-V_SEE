/*
 * SD_SPI.h
 *
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#ifndef SD_SPI_H_
#define SD_SPI_H_

extern unsigned char SD_Init();
extern unsigned char SD_Reset();
extern unsigned char SD_Read_Sector(unsigned long Addr ,unsigned char *Ptr,unsigned int FirstNum,unsigned int Num);
extern unsigned char SD_Write_Sector(unsigned long Addr ,unsigned char *Ptr,unsigned int FirstNum,unsigned int Num);
extern unsigned char SD_Set_SPI();
extern unsigned char Write_Command_SD(unsigned char *CMD);

#endif /* SD_SPI_H_ */
