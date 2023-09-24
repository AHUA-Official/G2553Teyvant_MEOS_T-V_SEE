/*
 * softFIFO.h
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */

#ifndef SOFTFIFO_H_
#define SOFTFIFO_H_

#define FIFOBUFF_SIZE  256				/*��������С,���ݳ�����Ҫ���е���*/

extern unsigned char FIFOBuff[FIFOBUFF_SIZE]; 	//���建���������(FIFO)
extern unsigned int FIFO_Count;           					//FIFO�����ݸ���

extern unsigned char Read_FIFO();
extern void Write_FIFO(unsigned char value);


#endif /* SOFTFIFO_H_ */
