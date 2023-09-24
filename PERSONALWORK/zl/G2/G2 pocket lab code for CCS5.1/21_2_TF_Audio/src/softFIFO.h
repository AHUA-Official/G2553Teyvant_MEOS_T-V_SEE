/*
 * softFIFO.h
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */

#ifndef SOFTFIFO_H_
#define SOFTFIFO_H_

#define FIFOBUFF_SIZE  256				/*缓冲区大小,根据程序需要自行调整*/

extern unsigned char FIFOBuff[FIFOBUFF_SIZE]; 	//定义缓冲队列数组(FIFO)
extern unsigned int FIFO_Count;           					//FIFO中数据个数

extern unsigned char Read_FIFO();
extern void Write_FIFO(unsigned char value);


#endif /* SOFTFIFO_H_ */
