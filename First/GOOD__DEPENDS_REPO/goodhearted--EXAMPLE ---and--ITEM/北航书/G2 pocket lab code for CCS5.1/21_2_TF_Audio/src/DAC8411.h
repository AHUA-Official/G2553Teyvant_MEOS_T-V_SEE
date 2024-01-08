/*
 * DAC8411.h
 *
 *  Created on: 2013-3-19
 *      Author: wangJingbing
 */

#ifndef DAC8411_H_
#define DAC8411_H_

/*
 * dac8411操作方法
 * dac8411为16位DAC，需要3线控制：STE;CLK;DOUT;
 * 每次控制共24bit数据。高两位寄存器的PD0~PD1为控制命令;D22~D6为DAC数据; D5~D0为无效位。需要在发送这6Bit数据时将STE拉高。
 */
// PD0~PD1命令控制宏定义
//#define		NORMAL_OPER_8411			0x00			//正常操作模式
//#define		GNDto1K_OPER_8411		0x01			//下拉1K欧姆电阻
//#define		GNDto100K_OPER_8411	0x02			//下拉100K欧姆电阻
//#define		HIGHZ_OPER_8411				0x03			//高阻

extern void DAC8411_Init(void);
extern void write2DAC8411(unsigned int bit_16);
#endif /* DAC8411_H_ */
