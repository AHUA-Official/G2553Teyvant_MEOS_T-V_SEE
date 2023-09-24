/*
 * DAC8411.h
 *
 *  Created on: 2013-3-19
 *      Author: wangJingbing
 */

#ifndef DAC8411_H_
#define DAC8411_H_

/*
 * dac8411��������
 * dac8411Ϊ16λDAC����Ҫ3�߿��ƣ�STE;CLK;DOUT;
 * ÿ�ο��ƹ�24bit���ݡ�����λ�Ĵ�����PD0~PD1Ϊ��������;D22~D6ΪDAC����; D5~D0Ϊ��Чλ����Ҫ�ڷ�����6Bit����ʱ��STE���ߡ�
 */
// PD0~PD1������ƺ궨��
//#define		NORMAL_OPER_8411			0x00			//��������ģʽ
//#define		GNDto1K_OPER_8411		0x01			//����1Kŷķ����
//#define		GNDto100K_OPER_8411	0x02			//����100Kŷķ����
//#define		HIGHZ_OPER_8411				0x03			//����

extern void DAC8411_Init(void);
extern void write2DAC8411(unsigned int bit_16);
#endif /* DAC8411_H_ */
