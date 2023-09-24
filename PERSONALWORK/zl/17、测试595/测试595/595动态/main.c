
#include "msp430g2553.h"
//#include "intrins.h"

#define uchar unsigned char 
#define uint  unsigned int

#define HC595_Shift_H  P2OUT |= BIT2    //��λʱ��     ��595��11��
#define HC595_Shift_L  P2OUT &= ~BIT2
#define HC595_Store_H  P2OUT |= BIT1    //����ת����ʱ�ӣ�595��12��
#define HC595_Store_L  P2OUT &= ~BIT1
#define SDI_H          P2OUT |= BIT0
#define SDI_L          P2OUT &= ~BIT0


//������
unsigned char DIG_CODE[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                            0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
unsigned char segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

unsigned char a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;

void HC595_Send(uchar dat0,uchar dat1);
void HC595store(void);
void HC595shift(void);
void P1_IODect();//P1�ڵ��ⲿ�ж��¼���⺯��
void GPIO_Init();//GPIO��ʼ������



void main(void)
{    
      

      WDTCTL = WDTPW +WDTHOLD;  // Stop Watchdog Timer
      GPIO_Init();//��ʼ��GPIO
      _BIS_SR(GIE);//��ͬ_EINT��ʹ�����ж�

      P2DIR = BIT0+BIT1+BIT2;
      P2OUT = BIT0+BIT1+BIT2;
      
      while(1)
      {
         
         HC595_Send(~DIG_CODE[a],segbit[0]);
         HC595_Send(~DIG_CODE[b],segbit[1]);
         HC595_Send(~DIG_CODE[c],segbit[2]);
         HC595_Send(~DIG_CODE[d],segbit[3]);
         HC595_Send(~DIG_CODE[e],segbit[4]);
         HC595_Send(~DIG_CODE[f],segbit[5]);
         HC595_Send(~DIG_CODE[g],segbit[6]);
         HC595_Send(~DIG_CODE[h],segbit[7]);
         
       
      }
}

//595�Ӻ���
void HC595_Send(uchar dat0,uchar dat1)
{
	uchar i,temp;
	HC595_Store_H;
	HC595_Shift_H;
	for(i=0;i<8;i++)
	{
                temp = dat0 & 0x80;
		if(temp != 0)  SDI_H;	//�Ӹ�λ��diλ                
                else           SDI_L;
                HC595shift();
		dat0 <<= 1;               
	}
	for(i=0;i<8;i++)
	{
                temp = dat1 & 0x80;
		if(temp != 0)  SDI_H;	//�Ӹ�λ��diλ                  
                else           SDI_L;
                HC595shift();
		dat1 <<= 1;               
	}      
        _NOP();
        _NOP();
        HC595store(); 
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
//����Ϊ595ȫ���Ӻ���

//zhongduan�Ӻ���
void GPIO_Init()
{
	
	//-----��ϻ�е�����������ڲ���������-----
	P1REN = 0xff;//����P1.3�ڲ�����������
	P1OUT = 0xff;//����������Ϊ����
	//-----����P1.3�жϲ���-----
	//P1DIR &= ~BIT3;// P1.3��Ϊ����(��ʡ��)
	P1IES = 0xff; // P1.3��Ϊ�½����ж�
	P1IE  = 0xff; // ����P1.3�ж�
        
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	//-----����Port1�¼���⺯��-----
	P1_IODect();//���ͨ�����������¼�������
   	P1IFG=0;//�˳��ж�ǰ�����ֶ����IO���жϱ�־
}

/******************************************************************************************************
 * ��       �ƣ�P1_IODect()
 * ��       �ܣ��жϾ��������жϵ�IO����������ӦIO���ж��¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����ú�����������8��IO�ļ�⣬�����ʵ������IO��������롱��
 * 					 �����У�����P1.3����������IO����������7��IO�ġ������롱û�б��������
 * ��       ������
 ******************************************************************************************************/
void P1_IODect()
{
	unsigned int Push_Key=0,pp=0;
        pp = ~P1DIR;
	//-----�ų����IO�ĸ��ź�����Ψһ���������жϱ�־λ-----
	Push_Key=P1IFG&(pp);//??????
	//-----��ʱһ��ʱ�䣬�ܿ���е��������-----
	__delay_cycles(10000);	//������ʱ
	//----�жϰ���״̬�Ƿ�����ʱǰһ��-----
	if((P1IN&Push_Key)==0)//����ôΰ���ȷʵ��Ч
        {
	 //----�жϾ����ĸ�IO�����£����ø�IO���¼�������-----
	 switch(Push_Key)
         {
	 case BIT0:	a++;		break;
	 case BIT1: 	b++;		break;
	 case BIT2: 	c++;		break;
	 case BIT3: 	d++;		break;
	 case BIT4: 	e++;		break;
	 case BIT5: 	f++;		break;
	 case BIT6: 	g++;		break;
	 case BIT7: 	h++;		break;
	 default:			break;		//�κ�����¾�����default
	 }
     }
}