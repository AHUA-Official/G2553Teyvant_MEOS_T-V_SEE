/*
 * Slope.h
 *
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#ifndef SLOPE_H_
#define SLOPE_H_

extern unsigned char Slope_Measure_Flag;
extern unsigned int R_REF;
extern unsigned int R_SENS;
extern void Slope_Measure_Init();
extern void  Slope_Measure_REF();
extern void Slope_Measure_SENS();
extern void Slope_TA_CCI1B();
extern void Slope_Port_Charge();

#endif /* SLOPE_H_ */
