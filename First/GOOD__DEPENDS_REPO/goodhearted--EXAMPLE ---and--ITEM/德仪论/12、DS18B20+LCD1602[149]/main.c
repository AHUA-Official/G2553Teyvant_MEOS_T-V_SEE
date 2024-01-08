
#include <msp430x14x.h>
//#include  "cry1602.h"
#include  "cry1602.c"
//#include  "DS18B20.h"
#include  "DS18B20.c"


//Ҫ��ʾ��6λ�¶�����
uchar temp[6]; 
void Clock_Init();
void Disp_Numb(uint temper);

void main(void)
{   
    WDTCTL = WDTPW + WDTHOLD;           //�ع�
     /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    Clock_Init();
    //����ʱ��ѡ��SMLK=8MHz��1/8��Ƶ��Ϊ1MHz
    TACTL |= TASSEL_2 + ID_3; 
    //��ȫ���ж�
    _EINT();
        
    
    LcdReset();
    DispStr(0,0,"Temperature is:"); 
    Disp1Char(8,1,0X27);
    Disp1Char(9,1,'C');
  
     while(1)
     {
       Disp_Numb(Do1Convert());
       Disp1Char(1,1,temp[5]+0x30);
       Disp1Char(2,1,temp[4]+0x30);
       Disp1Char(3,1,0x2e);         //0x2e��С�����Ӧ��ASCII��ֵ
       Disp1Char(4,1,temp[3]+0x30);
       Disp1Char(5,1,temp[2]+0x30);
       Disp1Char(6,1,temp[1]+0x30);
       Disp1Char(7,1,temp[0]+0x30);         
     }
}


/*******************************************
�������ƣ�Disp_Numb
��    �ܣ�����DS18B20��ȡ��11bit�¶�����ת��
          ��Һ����ʾ���¶�����
��    ����temper--11bit�¶�����
����ֵ  ����
********************************************/
void Disp_Numb(uint temper)
{
    uchar i;
    
    for(i = 0;i < 6;i++) 
      temp[i] = 0; //��ʼ����ʾ����

    //��ֵת��
    if(temper & BIT0) 
    {
        temp[0] = 5;
        temp[1] = 2;
        temp[2] = 6;
    }
    if(temper&BIT1)     
    {
        temp[1] += 5;
        temp[2] += 2;
        temp[3] += 1;
    }
    if(temper & BIT2)     
    {
        temp[2] += 5;
        temp[3] += 2;
        if(temp[2] >= 10)
        {
            temp[2] -= 10;
            temp[3] += 1;
        }
    }
    if(temper&BIT3)     
    {
        temp[3] += 5;
    }
    if(temper & BIT4)
    {
        temp[4] += 1;
    }
    if(temper & BIT5)     
    {
        temp[4] += 2;
    }
    if(temper & BIT6)
    {
        temp[4] += 4;
    }
    if(temper & BIT7)     
    {
        temp[4] += 8;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
    }
    if(temper & BIT8)
    {
        temp[4] += 6;
        temp[5] += 1;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
    }
    if(temper & BIT9)
    {
        temp[4] += 2;
        temp[5] += 3;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
    }
    if(temper & BITA)
    {
        temp[4] += 4;
        temp[5] += 6;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
        if(temp[5] >= 10)
        {
            temp[5] -= 10;
        }
    }
}

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ⲿ8M����
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}
