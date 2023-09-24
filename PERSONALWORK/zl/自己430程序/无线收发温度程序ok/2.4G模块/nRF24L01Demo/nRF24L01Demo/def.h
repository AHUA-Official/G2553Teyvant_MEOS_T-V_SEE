#ifndef _DEF_
#define _DEF_

#include <reg52.h>
#include <intrins.h>


 //hard pin mapping
sbit 	MISO	=P1^3;
sbit 	MOSI	=P1^4;
sbit	SCK	    =P1^5;
sbit	CE	    =P1^6;
sbit	CSN		=P3^7;
sbit	IRQ		=P1^2;

sbit    LED2    =P3^5;
sbit    LED1    =P3^4;
sbit    KEY1    =P3^0;
sbit    KEY2    =P3^1;


/*

sbit 	GDO0	=P1^2;
sbit 	GDO2	=P1^4;
sbit	MISO	=P1^5;
sbit	MOSI	=P1^6;
sbit	SCK		=P3^7;
sbit	CSN		=P1^3;

sbit    LED2    =P3^5;
sbit    LED1    =P3^4;
sbit    KEY1    =P3^0;
sbit    KEY2    =P3^1;

*/


#endif //_DEF_