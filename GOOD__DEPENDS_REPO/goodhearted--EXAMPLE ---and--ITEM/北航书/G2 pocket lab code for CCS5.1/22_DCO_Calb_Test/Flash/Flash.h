/*
 * Flash.h
 *
 *  Created on: 2013-2-20
 *      Author: Administrator
 */

#ifndef FLASH_H_
#define FLASH_H_

extern unsigned char Flash_Init(unsigned char Div,unsigned char Seg );
extern void Flash_Erase();
extern unsigned char Flash_ReadChar (unsigned int Addr);
extern unsigned int Flash_ReadWord (unsigned int Addr);
extern char Flash_ReadSeg(unsigned int Addr, unsigned int SegSize,unsigned char * Array);
extern char Flash_Direct_WriteChar (unsigned int Addr,unsigned char Data);
extern char Flash_Direct_WriteWord (unsigned int Addr,unsigned int Data);
extern char Flash_Bak_WriteChar (unsigned int Addr,unsigned char Data);
extern char Flash_Bak_WriteWord (unsigned int Addr,unsigned int Data);
extern unsigned char Flash_SegA_ReadChar (unsigned int Addr);


#endif /* FLASH_H_ */
