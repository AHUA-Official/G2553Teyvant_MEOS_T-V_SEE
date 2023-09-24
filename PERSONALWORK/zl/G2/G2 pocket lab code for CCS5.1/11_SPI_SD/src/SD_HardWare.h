/*
 * SD_HardWare.h
 *
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#ifndef SD_HARDWARE_H_
#define SD_HARDWARE_H_

extern void SD_Write_Byte(unsigned char Value);
extern void SD_Write_Frame(unsigned char  *pBuffer,unsigned char size);
extern void SD_CS_High();
extern void SD_CS_Low();
extern unsigned char SD_Read_Byte();
extern void SD_Read_Frame(unsigned char  *pBuffer, unsigned int size);
extern void SD_High_Speed();
extern void SD_Low_Speed();

#endif /* SD_HARDWARE_H_ */
