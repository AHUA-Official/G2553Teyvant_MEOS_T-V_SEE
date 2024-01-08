//ICC-AVR application builder : 2006-02-12 14:00:00
// Target : ATmega48
// Crystal: 8.000Mhz
// Author:  jackyan


#include "iom48v.h"
#include "macros.h"
#include "Defs.h"
#include "OLED.h"
#include "var.h"
#include "eeprom.h"

extern void I2C_Write(unsigned char M_ADD, unsigned char Data);
extern unsigned char I2C_Read(unsigned char M_ADD);



void InitOLED(void)
{
	WriteMode();
}

void WriteOLED(ubyte address,ubyte val)
{
	ubyte temp=0;
	ubyte temp2=20;
	while(temp2!=0)
		{
		I2C_Write(address,val);
		temp=ReadOLED(address);
		if(temp!=val)
			{
			temp2--;
			WDR();
			for(temp=0;temp<100;temp++)
				;
			}
		else
			break;
		}
	if (temp2==0)	// ����10�ζ�û�гɹ�
		set(Flag,bitOledError);	
}

ubyte ReadOLED(ubyte address)
{
	ubyte temp;
	temp=I2C_Read(address);
	return temp;
}

void WriteMode(void)
{
	ubyte temp,temp2;
	temp2=0;
	clr(Flag,bitOledError);
	
	if (OLEDMode==NTSC)
		{
		WriteOLED(0x01,0x78);
		WriteOLED(0x02,0x30);
		WriteOLED(0x03,0x78);
		WriteOLED(0x04,0x30);
		WriteOLED(0x05,0x78);
		
		WriteOLED(0x06,0x30);
		WriteOLED(0x07,Contrast);
		WriteOLED(0x08,Bright);
		WriteOLED(0x09,0x12);
		WriteOLED(0x0a,0x01);
		WriteOLED(0x0b,0x00);

		WriteOLED(0x0c,0x02);
		WriteOLED(0x0d,0x00);
		WriteOLED(0x0e,0x80);
		WriteOLED(0x0f,0x0a);
		WriteOLED(0x10,0x1b);
		WriteOLED(0x11,0x07);

		WriteOLED(0x12,0x08);
		WriteOLED(0x13,0x7b);
		WriteOLED(0x14,0x0f);
		WriteOLED(0x15,0xd8);
		WriteOLED(0x16,0x0c);
		WriteOLED(0x17,0x01);

		WriteOLED(0x18,0xc0);
		WriteOLED(0x19,0x00);
		WriteOLED(0x1a,0x00);
		}
	else	// if (OLEDMode==Pal)
		{
		WriteOLED(0x01,0x78);
		WriteOLED(0x02,0x38);
		WriteOLED(0x03,0x78);
		WriteOLED(0x04,0x38);
		WriteOLED(0x05,0x78);
		
		WriteOLED(0x06,0x38);
		WriteOLED(0x07,Contrast);
		WriteOLED(0x08,Bright);
		WriteOLED(0x09,0x4a);
		WriteOLED(0x0a,0x10);
		WriteOLED(0x0b,0x00);

		WriteOLED(0x0c,0x02);
		WriteOLED(0x0d,0x00);
		WriteOLED(0x0e,0x80);
		WriteOLED(0x0f,0xc0);
		WriteOLED(0x10,0x1b);
		WriteOLED(0x11,0x07);

		WriteOLED(0x12,0x00);
		WriteOLED(0x13,0x90);
		WriteOLED(0x14,0x0f);
		WriteOLED(0x15,0xd8);
		WriteOLED(0x16,0x0c);
		WriteOLED(0x17,0x01);

		WriteOLED(0x18,0xc0);
		WriteOLED(0x19,0x00);
		WriteOLED(0x1a,0xc0);
		}
}

void WriteBright(void)
{
//	WriteDA(Bright);
	WriteOLED(0x08,Bright);
}
