/*
 * USCI_I2C.h
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */

#ifndef USCI_I2C_H_
#define USCI_I2C_H_

extern void USCI_I2C_Init();
extern void USCI_I2C_Tx_Init();
extern void USCI_I2C_Rx_Init();
extern unsigned char PreSet_Tx_Frame(unsigned char *p_Tx,unsigned char num);
extern unsigned char PreSet_Rx_Frame(unsigned char *p_Tx,unsigned char num);

#endif /* USCI_I2C_H_ */
