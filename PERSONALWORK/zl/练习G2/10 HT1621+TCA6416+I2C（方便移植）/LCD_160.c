

//自己做LCD160子程序

#include <msp430x44x.h>
//#include <math.h>



void LCD_init(void)
{
     LCDCTL = LCD4MUX + LCDSG0_2 + LCDON;  //4MUX
     BTCTL =  BT_fLCD_DIV128;              // STK LCD freq 
     P5SEL =  0xFC;                        // Common and Rxx all selected
   
}

void LCD_Clear(void)
{
     uchar i=0;
     for(i=0;i<20;i++)
        LCDMEM[i]=0;
}
