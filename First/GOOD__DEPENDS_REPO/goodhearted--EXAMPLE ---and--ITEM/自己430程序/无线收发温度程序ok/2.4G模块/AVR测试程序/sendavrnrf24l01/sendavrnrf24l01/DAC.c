//ICC-AVR application builder : 2006-02-12 14:00:00
// Target : ATmega48
// Crystal: 1.000Mhz
// Author:  jackyan

#include "iom48v.h"
#include "macros.h"
#include "Defs.h"
#include "var.h"

void WriteDA(ubyte bright);

#define DAsync 	BIT0
#define DAsclk	BIT1
#define DAout 	BIT2

void InitDAC(void)
{
	WriteDA(Bright);
}

void WriteDA(ubyte bright)
{
	ubyte cnt;
	ubyte temp=0;

	set(DDRB,DAsync);
	set(DDRB,DAsclk);
	set(DDRB,DAout);
	temp=bright>>4;
	cnt=8;		
	while(cnt>0)
		{
			clr(PORTB,DAsync);
			set(PORTB,DAsclk);
			clr(PORTB,DAout);
			if (chk(temp,BIT7))
				set(PORTB,DAout);
			temp<<=1;
			cnt--;
			clr(PORTB,DAsclk);	//0
		}

	bright<<=4;
	temp=bright|0x0f;
	cnt=8;		
	while(cnt>0)
		{
			clr(PORTB,DAsync);
			set(PORTB,DAsclk);
			clr(PORTB,DAout);
			if (chk(temp,BIT7))
				set(PORTB,DAout);
			temp<<=1;
			cnt--;
			clr(PORTB,DAsclk);	//0
		}

	set(PORTB,DAsync);
	set(PORTB,DAsclk);
	set(PORTB,DAout);
}



