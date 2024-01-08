/*=========================================
���߷������
���뷽����
      �����룺  10ms�ͣ�10ms��
          0�� 01  1ms�ڣ�1ms��
          1�� 0111 1ms�ͣ�3ms�ߡ�
By----����
2012-7-16
=========================================*/
#include"msp430g2452.h"
#include"12864.h"
#define uchar unsigned char
#define uint unsigned int

uchar send[]={0x68, 0x30,0x18,0x7a,0x10,0x38,0x5a,0x42,0x4a,0x52,0xab};
uchar tab[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
uint t;//�����õļ�ʱ����
uchar n,i,dat;
uchar add=0x01;
//=======��ѹ������========//
uchar Vdat0,Vdat1;
uint ad,vv;
float v;
//============================//
void senddata(uchar x);//��������
void shuju();//ָʾ���͵�����
  
void init()
{
  P1DIR|=BIT0+BIT1;//P1.0���
  TACTL=TASSEL_2+MC_1;//SMCLK  upmode
  CCTL0=CCIE;
  CCR0=791;//100us��ʱ
}
void Vinit()
{
  ADC10CTL1 = INCH_3+CONSEQ_0; //���β���  ��û��CONSEQxѡ��ʱ��Ĭ��CONSEQ_0 P1.3����
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE+ ENC;//�������������ж� 
  ADC10AE0 |= 0x04; //�ر�P1.3��ͨIO���룬���ٸ��ţ�
}
void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 
    /*================����BCSCTL2==============*/
    BCSCTL2 = SELM_0 + DIVM_3 + DIVS_0;//DIVM==>MCLK=1MHZ��Ƶ��DIVS==>SMCLK=8MHZ��Ƶ

    if (CALBC1_8MHZ != 0xFF) {
        /* Follow recommended flow. First, clear all DCOx and MODx bits. Then
         * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
         */
        delay(100);
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 1MHz */
        DCOCTL = CALDCO_8MHZ;
    }
    /*===============OVER===================*/
    Vinit();
    init();
    lcdinit();
    W_str(0,1,"���߷������");
    W_str(0,2,"ʵ���ѹ��");
    W_str(0,3,"��ַ");
    W_str(3,3,"����");
    _EINT();
    while(1)
    {
        ADC10CTL0 |= ADC10SC; // ת����
          
//        dat=send[i];//add��ʾ�����͵�����
        shuju();
        n=10;
        while(n)
          {
            P1OUT=0x01;
            t=0;
            while(t<5);//�ṩ������
            //=======������========//
            P1OUT=0x00;
            t=0;
            while(t<100);
            
            P1OUT=0x01;
            t=0;
            while(t<100);
            
          //======������=====//  
            senddata(add);
            senddata(~add);
            senddata(Vdat1);
            senddata(Vdat0);
  
            P1OUT=0x00;
            t=0;
            while(t<10);
            --n;
          }

    }
}

void senddata(uchar x)
{
  uchar i,temp;
  temp=0x01;
  for(i=0;i<8;i++)
  {
    if(temp&x)//�����͵�λΪ1���������� 0111  һ��4ms
    {
        P1OUT=0x01;
        t=0;
        while(t<10);
        
        P1OUT=0x00;
        t=0;
        while(t<30);
    }
    else
    {
      P1OUT=0x01;
      t=0;
      while(t<10);
      
      P1OUT=0x00;
      t=0;
      while(t<10);
    }
    temp=temp<<1;
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer (void)
{
  t++;
  P1OUT^=0x02;
}

#pragma vector=ADC10_VECTOR //
  __interrupt void ADC()
 {
   ad=ADC10MEM;
   v=ad/1024.0*3.56;
   vv=v*1000;
    }
void shuju()
{
  uchar v1,v2,v3,v4;
  //===============dizhishuju==========//
  uchar n1,n2,n3,n4,n5,n6,n7,n8,add1;
  
    Vdat0=ad&0xff;//��λ
    Vdat1=ad>>8;  //��λ
    
  add1=~add;

  
  n1=add/16;
  n2=add%16;
  n3=add1/16;
  n4=add1%16;
  
  n5=Vdat0/16;
  n6=Vdat0%16;
  n7=Vdat1/16;
  n8=Vdat1%16;
  
  write_com(0x98);
  write_data(tab[n1]);
  write_data(tab[n2]);
  write_data('-');
  write_data(tab[n3]);
  write_data(tab[n4]);
  
  write_com(0x9b);
  write_data(tab[n7]);
  write_data(tab[n8]);
  write_data('-');  
  write_data(tab[n5]);
  write_data(tab[n6]);


//======================================//
    
  v1=vv/1000 + 0x30;
  v2=vv%1000/100 + 0x30;
  v3=vv%100/10 + 0x30;
  v4=vv%10 + 0x30;
  write_com(0x95);
  write_data(v1);
  write_data('.');
  write_data(v2);
  write_data(v3);
  write_data(v4);
  write_data('V');
//===============================//

}
