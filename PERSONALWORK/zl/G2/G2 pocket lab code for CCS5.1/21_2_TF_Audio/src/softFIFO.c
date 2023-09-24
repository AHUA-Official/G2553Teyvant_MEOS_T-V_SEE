/*
 * softFIFO.c
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */

#include "msp430g2553.h"
#include "softFIFO.h"
#define DISABLE_INT				_DINT()		// 关中断
#define RESTORE_INT			_EINT()			// 开中断

/***********************以下宏定义与FIFO缓冲器有关******************************/
unsigned char FIFOBuff[FIFOBUFF_SIZE]={0}; 	//定义缓冲队列数组(FIFO)
static unsigned int 	FIFO_IndexW;          	//缓冲队列写入指针(头指针)
static unsigned int 	FIFO_IndexR;          		//缓冲队列读取指针(尾指针
unsigned int 	FIFO_Count;           					//FIFO中未读取数据的个数
/****************************************************************************
* 名    称：Write_FIFO()
* 功    能：将一次值压入相应键盘缓冲队列
* 入口参数：value:数据
* 出口参数：无
****************************************************************************/
void Write_FIFO(unsigned char value)
{
  if(FIFO_Count>=FIFOBUFF_SIZE) return;//若缓冲区已满，放弃本次值
  DISABLE_INT;
  FIFO_Count++;                       //次数计数增加
  FIFOBuff[FIFO_IndexW] = value;   //从队列头部追加新的数据
  if (++FIFO_IndexW >=  FIFOBUFF_SIZE) //循环队列，如果队列头指针越界
   {
	  FIFO_IndexW = 0;                  //队列头指针回到数组起始位置
   }
  RESTORE_INT;
}
/****************************************************************************
* 名    称：Read_FIFO()
* 功    能：从缓冲队列内读取一次值
* 入口参数：无
* 出口参数：value：数据
* 说    明: 调用一次该函数，会自动删除缓冲队列里一个数据。
****************************************************************************/
unsigned char Read_FIFO()
{
	unsigned char value;
  if(FIFO_Count==0)  return(0);     	//若无数据，返回0
  //DISABLE_INT;								//队列操作过程中不允许被定时中断执行ScanIO函数，保护全局变量正确
  FIFO_Count--;                     			//按键次数计数减1
  value=FIFOBuff[FIFO_IndexR];    		//从缓冲区尾部读取一个数据
  if (++FIFO_IndexR >= FIFOBUFF_SIZE)	//循环队列，如果队列尾指针越界
     {
	  FIFO_IndexR = 0;             			//队列尾指针回到数组起始位置
     }
  //RESTORE_INT;                     				//恢复中断允许
  return(value);
}



