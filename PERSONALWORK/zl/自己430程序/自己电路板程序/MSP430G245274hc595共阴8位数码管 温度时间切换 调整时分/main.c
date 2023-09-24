/**************************************************************
    
    ��ʵ��74HC595Ҳ��һ�ֶ�̬��ʾ�����Ա�������ѭ�����С�
    ����74HC595���벢���ķ�ʽ��ʾ8λ���������,��ʵ������P2.0,P2.1,P2.2

    ���ÿ��Ź���ʱ����ʵ�ֵ��ӱ���������ܵ�1.2��ʾʱ����3.4λ��ʾ��
    ���7.8��ʾ�롣
   
    ��������DS18B20�¶ȴ�������ʾ�¶ȣ�ͨ������P1.0 �л��¶�
    
    �л��¶Ⱥ�ʱ�䣬����ʱ����䣺
    ͨ��P1.1�л�ʱ�䣨ǰ���ǰѿ������ϵ�LED1��ñ�γ�����
    ͨ��P1.2���ڷּ�һ����
    ͨ��P1.3���ڷּ�һ����
    ͨ��P1.2����ʱ��һСʱ
    ͨ��P1.3����ʱ��һСʱ

   //////       Ӳ������     //////////////

    P2.0��74HC595��12 �ţ��洢ʱ��(STCP)
    P2.1��74HC595��14 �ţ��������������(DS)
    P2.2��74HC595��11 �ţ���λʱ��(SHCP)
    P2.3��DS18B20��������ǵý�һ����������4.7K��10K��
    P1.0�л��¶�
    P1.1�л��¶�
    P1.2ʱ��ֵĵ��ڣ�������һ����
    P1.3ʱ��ֵĵ��ڣ�������һ����
    P1.4ʱ��ֵĵ��ڣ�������һСʱ
    P1.5ʱ��ֵĵ��ڣ�������һСʱ
*************************************************************/
#include <msp430g2452.h>
#define uint unsigned int 
#define uchar unsigned char

//������74HC595����ĺ궨��
#define HC595_Store_H P2OUT |= BIT0
#define HC595_Store_L P2OUT &= ~ BIT0
#define HC595_A_H     P2OUT |= BIT1
#define HC595_A_L     P2OUT &= ~ BIT1
#define HC595_Shift_H P2OUT |= BIT2
#define HC595_Shift_L P2OUT &= ~ BIT2
#define DS18B20       BIT3
#define DS18B20_H     P2OUT |= BIT3
#define DS18B20_L     P2OUT &= ~BIT3
//������74HC595����ĺ궨��
unsigned char const LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                                0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71,//��������ܶ�ѡ����(0-F��
                                0xBf,0x86,0xDb,0xCf,0xE6,0xEd,0xFd,0x87,    
                                0xff,0xef,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,};//���������(��С����)��ѡ����(0-F��
unsigned char const LedPos[] = {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}; //�����λѡ���飬�ֱ�ѡͨÿһλ�����
volatile uint shijian=0;                     //ʱ�����
volatile uchar sec=0,minter=0,hour=0;
volatile uchar temp_flag=1,time_flag=0;     //�����ֵȫ�ֱ���

/**********************************************

                ��������

**********************************************/
void init_devices(void);
void port_init(void);
void int_WDT(void);
void int_Clock();
volatile uchar key_value=0;           //�����ֵȫ�ֱ���

//������DS18B20����ĺ�������

void B20_init(void);
uchar B20_readB(void);
void B20_writeB(uchar wrd);
uint Read_temp(void);
void B20_display(void);
//������DS18B205����ĺ�������

void miaobiao(void); 
void Delay_ms(uint ms);           // ��ʱms  ǰ����f=8MHZ

//������74HC595����ĺ�������
void HC595shift(void);
void HC595store(void);
void HC595send(uchar x);
void display(uchar pos,uchar dat); //pos���Ǳ�ʾѡͨ��һλ��dat���Ǳ�ʾ��ʾʲô����
//������74HC595����ĺ�������
void main( void )         //������
{
   
   init_devices();        //��ʼ������
   _EINT();               //�����ж�
   while(1)
     {  
       while(temp_flag)
       {
         B20_display();     //��ʾ�¶�
       }
       while(time_flag)
       {
          miaobiao();        //��ʾ���ӱ�
       }
     }
}
void init_devices(void)
{
   port_init();          //IO����
   int_WDT();            //���Ź�����
   int_Clock();          //ʱ������
}
void port_init(void)           //�˿ڳ�ʼ��
{
   P2DIR |= BIT0+BIT1+BIT2+BIT3;
    
   P1OUT |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     //P2.0-P2.5��������
   P1REN |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     //ʹ��P2.0-P2.5����������
   P1IE  |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     // P2.0-P2.5 IO���ж�ʹ��
   P1IES |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     //P2.0-P2.5 IO�ڱ��ش����жϷ�ʽ����
   P1IFG &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);  // P2.0-P2.5 IO���жϱ�־λ���
}
void int_WDT(void)                 //���Ź�����
{   
  WDTCTL = WDT_ADLY_1000;          //  �򿪿��Ź����ڸ���ϵͳʱ���¶�ʱ1s��
  IE1 |=WDTIE;                     // �����Ź��ж�
}
void int_Clock(void)               //ʱ�ӳ�ʼ�� ����������ʱ��Ϊ8MHZ��
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO��������=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO��������=8MHZ
}
void Delay_ms(uint ms)             // ��ʱms  ǰ����f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
}
/********************************
///////////////////////////////

�������¶ȴ�����DS18B02����
��ʱ

/////////////////////////////////

��������: B20_init
��    ��: ��λDS18B20
��    ��: ��
����ֵ  : ��
********************************/
void B20_init(void)
{
 P2DIR |= DS18B20;		             //����Ϊ���			  
 DS18B20_L;			             //����
 //Delay_us(600);			     //�ȴ�600΢��
 __delay_cycles(4800); 
 DS18B20_H;			             //�ͷ�����
 //Delay_us(60);			     //�ȴ�60΢��
 __delay_cycles(480); 
 P2DIR &= ~DS18B20;		             //����Ϊ���� 
 while((P2IN &(DS18B20)));	             //�ȴ�DS18B20����
 while(!(P2IN &(DS18B20)));                  //�ȴ�DS18B20�ͷ�����
}
/*******************************************
��������: B20_readB
��    ��: ��ȡһ���ֽڵ�����
��    ��: ��
����ֵ  : retd--���ص�һ���ֽ�����
********************************************/
uchar B20_readB(void)
{
 uchar i,retd=0;
 for(i=0;i<8;i++)	 	//λ����ֵ
 {
  retd>>=1;			//���ƣ�׼�������µ�����λ
  P2DIR |=DS18B20;		//����Ϊ���
  DS18B20_L;		        //���ͣ�����������λ
  DS18B20_H;			//�ͷ�����
  //Delay_us(5);		//�ȴ�5΢��
  __delay_cycles(40); 
  P2DIR &=~DS18B20;		//����Ϊ���룬��ʼ��ȡ����λ
  if(P2IN&DS18B20)		//��λ�Ƿ�Ϊ��
  {
   retd|=0x80;			//�Ǿͽ���λ�ø�
  }
  //Delay_us(50);		//�ȴ�50΢��
  __delay_cycles(400); 
 }
 return retd;			//��������һ���ֽڷ���
}
/*******************************************
��������: B20_writeB
��    ��: д��һ���ֽڵ�����
��    ��: wrd--Ҫд�������
����ֵ  : ��
********************************************/
void B20_writeB(uchar wrd)
{
 uchar i;
 for(i=0;i<8;i++)	  	//λ����ֵ
 {
  P2DIR |=DS18B20;		//����Ϊ���
  DS18B20_L;		        //���ͣ�����д����λ
 // Delay_us(1);		//�ȴ�1΢��
  __delay_cycles(8); 
  if(wrd&0x01)			//��λ�����Ƿ�Ϊ��
  {
   DS18B20_H;		        //�Ǹ��򽫵���������
  }
  else
  {
   DS18B20_L;		        //�ǵ��򽫵���������
  }
  //Delay_us(50);	        //�ȴ�50΢��
  __delay_cycles(400); 
  DS18B20_H;		        //�ͷ�����
  wrd>>=1;			//���ƣ�Ϊд���µ�����λ��׼��
 }
 // Delay_us(50);		//�ȴ�50΢��
 __delay_cycles(400); 
}
/*******************************************
��������: Read_temp
��    ��: ��ȡ�¶�ֵ
��    ��: ��
����ֵ  : rettemp--���ص��¶�ֵ
********************************************/
uint Read_temp(void)
{
 uchar templ,temph;
 uint temp;
 B20_init();	        //��ʼ����ÿ��д����ӳ�ʼ����ʼ
 B20_writeB(0xcc);      //����ROM
 B20_writeB(0x44);      //�����¶�ת��
 B20_init();	        //��ʼ����ÿ��д����ӳ�ʼ����ʼ
 B20_writeB(0xcc);      //����ROM
 B20_writeB(0xbe);      //���Ĵ���
 templ=B20_readB();     //���¶ȵ��ֽ�
 temph=B20_readB();     //���¶ȸ��ֽ�
 temp=templ+temph*256;  //���¶������16λ����
  return temp;	        //����16λ����
}
void B20_display(void)
{
   uint t;
   t=Read_temp();          //��ȡ�¶�ֵ
   t=t*6.25;              //��ʾ���¶ȱ���С�����һλ��
                           //Ҫ�뱣����λ���Գ���0.0625
   display(2,t/1000);       //��ʾ�¶ȵ�ʮλ
   Delay_ms(5);
   display(3,t%1000/100+16); //��ʾ�¶ȵĸ�λ����λ��ʾС���㼴���ʱ��16��
   Delay_ms(5);
   display(4,t%100/10);        //��ʾ�¶ȵ�С��λ
   Delay_ms(5);
   display(5,t%10);        //��ʾ�¶ȵ�С��λ
   Delay_ms(4);
}
/*******************************************
///////////////////////////////

�������¶ȴ�����DS18B02����

////////////////////////////
�����ʾ�Ӻ���������������ܵĵ�3λ��ʾʱ��
��4,5λ��ʾ�֣���6,7λ��ʾ�롣

********************************************/
void miaobiao(void)
{
      hour=shijian/3600;
      minter=shijian%3600/60;
      sec=shijian%60;
        if(shijian>43199)
        {
          shijian=0;
        }
        display(6,sec/10);
        Delay_ms(3);
        display(7,sec%10);
        Delay_ms(3);
        display(3,minter/10);
        Delay_ms(3);
        display(4,minter%10+16);//��ʾ���ӱ�ķֵĺ�һλ��Ҫ��ʾС���㼴���ʱ��16��
        Delay_ms(3);
        display(0,hour/10);     //��ʾ���ӱ��ʱλ��Ҫ��ʾС���㼴���ʱ��16��
        Delay_ms(3);
        display(1,hour%10+16); 
        Delay_ms(2);
     }
/*******************************************
///////////////////////////////

������74HC595��������ܵĺ���

////////////////////////////
�����ʾ�Ӻ���������������ܵĵ�3λ��ʾʱ��
��4,5λ��ʾ�֣���6,7λ��ʾ�롣

********************************************/

void HC595send(uchar x)
{
   uchar n,temp;
   for(n=0;n<8;n++)	//һ���ֽ�Ҫ��8��д��
   {
      temp=x&0x80;	//��סһ��Ҫ��д���λ
      if(temp!=0)	//�ж��Ƿ񲻵���0
      {
         HC595_A_H;     //P1.1��Ϊ1
         HC595shift();	//ʹ�Ĵ�����λ
      }
      else		//����0
      {
         HC595_A_L;     //P1.1��Ϊ0
         HC595shift();  //ʹ�Ĵ�����λ
      }	 
      x<<=1;		//��������һλ����ô��һ��ѭ�����ǻ�ȡ
                        //��7λ�����ݣ������ǵ�6λ,��5λ,,,��1λ
                        //�����������ݶ�����HC595
    }
}
/***************************************
 �ֽ�д��洢�Ĵ�����������������˿ڣ�����ˢ�����
***************************************/
void HC595store(void) 
{
   HC595_Store_L;   //�Ƚ��洢ʱ���õ�                  
   HC595_Store_H;   //���洢ʱ���ø߲���һ�������أ��ͻ��һ���ֽ�
                    //д��洢�Ĵ���������74HC595 ������ˢ�������
} 
/**************************************
           �Ĵ�����λ����
**************************************/
void HC595shift(void) 
{
   HC595_Shift_L;  //Ҫ��ʱ�����õ�                 
   HC595_Shift_H;  //��ʱ�������ߣ����������أ��ͻ�ѽ��мĴ�����λ����
}
/*************************************
        ��ʾ��������ĳλ��ʾĳ����
*************************************/
void display(uchar pos,uchar dat) 
{
   HC595send(LedPos[pos]);   //һ��Ҫ�Ȱ�λѡ���������ϼ�HC595 
                             //���Ѷ�ѡ���������ϼ�HC595ʱ��
                             //λѡ��������Ƶ��¼�HC595��
   HC595send(LedData[dat]);  //�����������¼�HC595ʱ��
                             //λѡ��������Ƶ��ϼ�HC595��
   HC595store(); 	     //��������ʾ����
   
}
/******************************************************

                  �жϷ�����

*******************************************************/
#pragma vector=WDT_VECTOR             //���Ź��жϷ������
__interrupt void watchdog_time (void)
{
 shijian++;
}

//P1�жϷ������

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(10000);
    {
      if((P1IN & BIT0)==0)
      {
         temp_flag=1;
         time_flag=0;
      }
       if((P1IN & BIT1)==0)
      {
         temp_flag=0;
         time_flag=1;
      }
       if((P1IN & BIT2)==0)
      {
         shijian=shijian+60;
         __delay_cycles(20000);
      }
       if((P1IN & BIT3)==0)
      { 
         shijian=shijian-60;
         __delay_cycles(20000);
       }
      if((P1IN & BIT4)==0)
      {
         shijian=shijian+3600;
         __delay_cycles(20000);
      }
      if((P1IN & BIT5)==0)
      {
         shijian=shijian-3600;
         __delay_cycles(20000);
      }
    }
        P1IFG=0;  // P2.0-P2.2 IO���жϱ�־λ���
}