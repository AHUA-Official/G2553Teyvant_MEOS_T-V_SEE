
#ifndef _UASRT_H_
#define _UASRT_H_


#include<msp430g2553.h>



extern void UARST_int();
extern void PutChar(unsigned char zifu);
extern void PutString(char *ptr);
extern void Delays(void);


#endif 