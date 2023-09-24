/*===========================================================
LaunchPad���߽��ճ���,315M
ʵ�������ֽڵĴ��ͣ��������޷��Զ�ˢ�µ����⡣
LaunchPad�����P1.6��LED���̵ƣ�ָʾˢ�µ��ٶȡ�

       0��   01   1ms�ͣ�1ms�ߣ�====>һ��2ms
       1��   0111 1ms�ͣ�3ms�ߣ�====>һ��4ms
  �����룺10ms�ͣ�10ms�ߡ�====>һ��20ms

�������к������ɫ��LEDû����˸����û��ˢ�£������޷�ͬ����
LED��˸�ڼ䡣����һֱ��ͬ�����¡�
����LED������˸��Ƶ���ȶ���

ע�ͣ�
�Ľ��ĵط���
1.�������飬������add[]={0};===>add[i];i����Ҫ�õ���������
2.��ISP���棨port1����i��ֵ�����������ݵĶ��١�
3.���Ҫ�޸Ľ������ݵĶ��٣���һ��Ҫ�޸�3����
  1>.�������鴦��
  2>.ISP���������iֵ�ж�
  3>.��irpro����;�������棬�޸���Ӧ�Ľ������ݡ�
����������ص㣺ISP��BCSCTL�����á�

    2012-7-16
By---����
===========================================================*/


#include"msp430g2553.h"
#include "msp430.h"
#include"12864.h"
#define uchar unsigned char
#define uint unsigned int

uchar flag,pro_ok,irok,i;

uint irtime;
uchar irdata[33];//�洢ʱ����
uchar irdat[4];//�洢���յ�����
uchar add[8];//������ʾ
//========//
  uint v;
  uint vv;
  uint ad;
uchar tab[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

void irpro();
void display();
//===========P1�ڵĳ�ʼ��==========//
void initport()
{
  P1DIR=0x41;
  P1OUT=0x18;
  P1REN=0x18;
  P1IE|=0x18;//P1.3��P1.4Ϊ�ж������
  P1IES|=BIT3+BIT4;//������ʽѡ��
  P1IFG=0x00;
}
//===========OVER==================//



//=========��ʱ���ĳ�ʼ��===========//
void init()
{
  TACTL=TASSEL_2+MC_1;//SMCLK  upmode
  CCTL0=CCIE;
  CCR0=796;//100us
}
//===============OVER=============//


//=============main==============================//
void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
    //======================ʱ������=====================//
    //====���õ�����ʱ�ӣ���SMCLKʱ��===================//
  BCSCTL2 = SELM_0 + DIVM_3 + DIVS_0;//��Ƶ��SMCLK==>8MHZ

    if (CALBC1_8MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        delay(100);

        // Follow recommended flow. First, clear all DCOx and MODx bits. Then
        // apply new RSELx values. Finally, apply new DCOx and MODx bit values.
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 8MHz */
        DCOCTL = CALDCO_8MHZ;
    }
    

    //=========================OVER=====================//
    initport();
    lcdinit();
    init();
    W_str(0,1,"���߽��ճ���");
    W_str(0,2,"���յ�ѹ��");
    W_str(0,3,"��ַ");
    W_str(4,3,"����");
    _EINT();
    while(1)
    {
      if(irok)
      {
        irpro();
        irok=0;       
      }
      if(pro_ok)
      {
        display();
        pro_ok=0;
      }
    }
}
//==========main OVER=================//



//=========100us��ʱ�жϷ�����=======//
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer(void)
{
  irtime++;
}


//===============���ն��жϷ���=================//

#pragma vector=PORT1_VECTOR
__interrupt void Port (void)
{
  if(P1IFG&BIT3)//ֻ�õ���P1.3��
  {
    if(flag)
    {
      if(irtime>150&&irtime<250)//����֮���̫С�������޷����롣
      {
        i=0;
      }
      irdata[i]=irtime;
      irtime=0;
      i++;
      if(i==33)
      { 
//        W_str(0,2,"ISP");//�����ã�������
        irok=1;
        i=0;
      }
    }
    else
    {
      irtime=0;
      flag=1;
    }
    P1IFG=0x00;
  }
  P1IFG&=~BIT3;
}



/*===========================
�Խ��յ����ݽ��д���
============================*/
void irpro()
{
  uchar j,k,i;
  uchar code,value;

  if(irdata[0]>150&&irdata[0]<250)//�����е�����
  {
    k=1;
    for(i=0;i<4;i++)
    {
      for(j=0;j<8;j++)
      {

        if(j<8)
        value=value>>1;//ò���������λ���൱��Ҫ��
        
        code=irdata[k];
        if(code>30)
          value|=0x80;      
        k++;
      }
      irdat[i]=value;
      value=0; 
   }
         
    add[0]=irdat[0]/16;
    add[1]=irdat[0]%16;
        
    add[2]=irdat[1]/16;
    add[3]=irdat[1]%16;

    
    add[4]=irdat[2]/16;
    add[5]=irdat[2]%16;
        
    add[6]=irdat[3]/16;
    add[7]=irdat[3]%16;
        
    pro_ok=1;    
//    W_str(3,2,"irpro");//���Ա��������ˡ�
  }
  P1OUT^=0x40;//ָʾ��������
}
void display()
{
  uchar a1,a2,a3,a4;
  
   v=add[5]*256+add[6]*16+add[7];//֤��v��������

   ad=v*3.56*1000/1024;//����ط��ܿӵ�����������

   vv=ad;
    
  a1=vv/1000+0x30;
  a2=vv%1000/100+0x30;
  a3=vv%100/10+0x30;
  a4=vv%10+0x30;

  
  write_com(0x95);
  write_data(a1);
  write_data('.');
  write_data(a2);
  write_data(a3);
  write_data(a4);
  write_data('V'); 
  
  write_com(0x98);
  write_data(tab[add[0]]);
  write_data(tab[add[1]]);
  write_data('-');
  write_data(tab[add[2]]);
  write_data(tab[add[3]]);
  
  write_com(0x9c);
  write_data(tab[add[4]]);
  write_data(tab[add[5]]);
  write_data('-');
  write_data(tab[add[6]]);
  write_data(tab[add[7]]);
  
//  W_str(7,2,"OK");//ָʾdisplay�Ƿ����У�

}
