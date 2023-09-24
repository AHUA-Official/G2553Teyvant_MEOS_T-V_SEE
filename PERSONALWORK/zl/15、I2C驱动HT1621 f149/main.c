/*******************************************************************

            ������:  HT1621оƬ���Գ��� ��6��LED�������ʾ���ӱ�
            ����CPU ��MSP430G2553 	  8MHZ
            ����˵��: �����Գ����ܹ�6��LED�������ʾ��ʾ���ӱ�

Ӳ������ͼ��
            HT1621_DAT.................BIT0  //HT1621��������
            HT1621_WR..................BIT1  //HT1621ʱ������(CLK) 
            HT1621_CS..................BIT2  //HT1621ʹ������ 
********************************************************************/
#include <msp430x14x.h>
#define uchar unsigned char 
#define uint unsigned int


//HT1621����λ��Һ��ģ��ӿڶ��壬�������ѵ���Ҫ���ģ� 
#define HT1621_DAT   BIT0  //HT1621��������
#define HT1621_WR    BIT1  //HT1621ʱ������ 
#define HT1621_CS    BIT2  //HT1621ʹ������ 

#define HT1621_DAT_H P1OUT |= BIT0
#define HT1621_DAT_L P1OUT &= ~BIT0
#define HT1621_WR_H  P1OUT |= BIT1
#define HT1621_WR_L  P1OUT &= ~BIT1
#define HT1621_CS_H  P1OUT |= BIT2
#define HT1621_CS_L  P1OUT &= ~BIT2

void Delay_us(uint us)             // ��ʱus  ǰ����f=8MHZ���������ø���ʱ�����Ǻܾ�ȷ��
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(8);      
}
void Delay_ms(uint ms)             // ��ʱms  ǰ����f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
}
#define BIAS       0x52 //0b1000 0101 0010  1/3duty 4com 
#define SYSDIS     0X00 //0b1000 0000 0000  ����ϵͳ������LCDƫѹ������ 
#define SYSEN      0X02 //0b1000 0000 0010  ��ϵͳ���� 
#define LCDOFF     0X04 //0b1000 0000 0100  ��LCDƫѹ 
#define LCDON      0X06 //0b1000 0000 0110  ��LCDƫѹ 
#define TIMERDIS   0X08 //0b1000 0000 0110  ��ֹʱ�����
#define XTAL       0x28 //0b1000 0010 1000  �ⲿ��ʱ�� 
#define RC256      0X30 //0b1000 0011 0000  �ڲ�ʱ�� 
#define TONEON     0X12 //0b1000 0001 0010  ��������� 
#define TONEOFF    0X10 //0b1000 0001 0000  �ر�������� 
#define WDTDIS     0X0A //0b1000 0000 1010  ��ֹ���Ź� 

//LCD��λ��ʾ(R1���λ�������ұ�һλ)��ͬ��R2��R3.....
volatile uchar R1=0,R2=0,R3=0,R4=0,R5=0,R6=0;

uchar  Ht1621Tab[]={0x00,0x00,0x00,0x00,0x00,0x00, //32����ַ������
                    0x00,0x00,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00};         

//����0-9���ֲ���С����Ķ�ά����
uchar SEG_7[10][2]=   
 { 
  0xc4,0xc8,//00 
  0x04,0x40,//01  
  0x4c,0x88,//02 
  0x4c,0x48,//03
  0x8c,0x40,//04
  0xc8,0x48,//05
  0xc8,0xc8,//06
  0x44,0x40,//07
  0xcc,0xc8,//08
  0xcc,0x48,//09
 }; 
//����0-9���ִ�С����Ķ�ά���� 
//ע�⣺1.���ұ����ֵ�С���㲻���ڣ�ֻ��COL1= ��������ʾʱ���������ð�ţ���
uchar SEG_8[10][2]=   
 { 
  0xc4,0xcc,//00 
  0x04,0x44,//01  
  0x4c,0x8c,//02 
  0x4c,0x4c,//03
  0x8c,0x44,//04
  0xc8,0x4c,//05
  0xc8,0xcc,//06
  0x44,0x44,//07
  0xcc,0xcc,//08
  0xcc,0x4c,//09
 };

//������HT1621 LCD��ʾ�ӳ���
/******************************************************
д���ݺ���,cntΪ��������λ��,���ݴ���Ϊ��λ��ǰ
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
    uchar i;
    for (i=0;i<cnt;i++)
     {
        HT1621_WR_L;
        Delay_us(1);
        if(Data&0x80)
          HT1621_DAT_H;
        else
          HT1621_DAT_L;
        Delay_us(1);
        HT1621_WR_H;
        Delay_us(1);
        Data<<=1;
 }
}
/********************************************************
�������ƣ�void Ht1621WrCmd(uchar Cmd)
��������: HT1621����д�뺯��
ȫ�ֱ�������
����˵����CmdΪд����������
˵����д�������ʶλ100
********************************************************/
void Ht1621WrCmd(uchar Cmd)
{
HT1621_CS_L;
Delay_us(1);
Ht1621Wr_Data(0x80,4); //д�������־100
Ht1621Wr_Data(Cmd,8); //д����������
HT1621_CS_H;
Delay_us(1);
}
/********************************************************
�������ƣ�void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
��������: HT1621����д�뷽ʽ����
ȫ�ֱ�������
����˵����AddrΪд���ʼ��ַ��*pΪ����д������ָ�룬
cntΪд����������
˵����HT1621������λ4λ���˴�ÿ������Ϊ8λ��д������
������8λ����
********************************************************/
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
{
    uchar i;
    HT1621_CS_L;
    Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
    Ht1621Wr_Data(Addr<<2,6); //д���ַ����
    for (i=0;i<cnt;i++)
      {
        Ht1621Wr_Data(*p,8); //д������
        p++;
      }
    HT1621_CS_H;
    Delay_us(1);
}
/********************************************************
�������ƣ�void Ht1621_Init(void)
��������: HT1621��ʼ��
ȫ�ֱ�������
����˵������
˵������ʼ����Һ���������ֶξ���ʾ
********************************************************/
void Ht1621_Init(void)
{
    HT1621_CS_H;
    HT1621_WR_H;
    HT1621_DAT_H;
    Delay_ms(10);       //��ʱʹLCD������ѹ�ȶ�
    Ht1621WrCmd(BIAS);  //1/3duty 4com
    Ht1621WrCmd(RC256); //ʹ���ڲ�����
    Ht1621WrCmd(SYSDIS);//����ϵͳ������LCDƫѹ������
    Ht1621WrCmd(WDTDIS);//��ֹ���Ź�
    Ht1621WrCmd(TONEOFF);//�ر��������
    Ht1621WrCmd(SYSEN); //��ϵͳ����
    Ht1621WrCmd(LCDON); //��LCDƫѹ
}

//��λLCD��ʾ�Ӻ���
void LCD_Display(void)
{
      Ht1621Tab[2]=SEG_7[R6][0];       //����ߵ�һλ�����
      Ht1621Tab[3]=SEG_7[R6][1];       //����ʾС���㣬�ڴ˵���SEG_8[R6][1]

      Ht1621Tab[4]=SEG_7[R5][0];       //����ߵڶ�λ�����
      Ht1621Tab[5]=SEG_7[R5][1];       //����ʾС���㣬�ڴ˵���SEG_8[R5][1]
      
      Ht1621Tab[6]=SEG_7[R4][0];       //����ߵ���λ�����
      Ht1621Tab[7]=SEG_7[R4][1];       //����ʾС���㣬�ڴ˵���SEG_8[R4][1]
      
      Ht1621Tab[8]=SEG_7[R3][0];       //����ߵ���λ�����
      Ht1621Tab[9]=SEG_7[R3][1];       //����ʾС���㣬�ڴ˵���SEG_8[R3][1]
      
      Ht1621Tab[10]=SEG_7[R2][0];      //����ߵ���λ�����
      Ht1621Tab[11]=SEG_7[R2][1];       //����ʾС���㣬�ڴ˵���SEG_8[R2][1]
      
      Ht1621Tab[12]=SEG_7[R1][0];      //����ߵ���λ�����
      Ht1621Tab[13]=SEG_7[R1][1];     

      Ht1621Tab[13]=SEG_8[R1][1];     //��ʾʱ����С���� ��(:)COL1
//      Ht1621Tab[14]=0xcc;             //��ʾR1,R2,R3,R4
      Ht1621Tab[15]=0x40;             //��ʾʱ���С���� ��(:)COL2
//      Ht1621Tab[15]=0x0c;             //��ʾL1,L2  
      Ht1621WrAllData(0,Ht1621Tab,16);//д1621�Ĵ������ݣ�����ʾ    
}
//������HT1621 LCD��ʾ�����Ӻ���

void port_init(void)                       //�˿ڳ�ʼ��
{
  P1DIR |= HT1621_DAT+HT1621_WR+HT1621_CS; //����HT1621���
}

void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  DCOCTL = DCO0 + DCO1 + DCO2;              // Max DCO
  BCSCTL1 = RSEL0 + RSEL1 + RSEL2;          // XT2on, max RSEL
}
void int_WDT(void)                 //���Ź�����
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}

void init_devices(void)
{
   port_init();                     //IO������
   int_Clock();                     //ʱ������
   int_WDT();                       //���Ź�����
}
void stop_lcd(void)        //�ر�LCD�ӳ���
{
    HT1621_CS_H;
    HT1621_WR_H;
    HT1621_DAT_H;
    Ht1621WrCmd(LCDOFF);   //��LCDƫѹ
    Ht1621WrCmd(TONEOFF);  //�ر��������
                           //��ʵ���û���Ҫ�أ���Ϊ��ʼ��û�д�
    Ht1621WrCmd(TIMERDIS); //��ֹʱ�����
    Ht1621WrCmd(SYSDIS);   //����ϵͳ������LCDƫѹ������
    P1DIR =0XFF;           //����IO
    P1OUT =0XFF;     
    P2DIR =0xff;       
    P2OUT =0xff;     
}

/******************************************************** 
�������ƣ�main() 
��������: ������
********************************************************/
void main() 
{ 
  uint i=0;
  init_devices();                       // ��ʼ������
  Ht1621_Init();                        //�ϵ��ʼ��LCD 
  //Delay_ms(10);                      //��ʱһ��ʱ�� 
  Ht1621WrAllData(0,Ht1621Tab,16);      //���1621�Ĵ������ݣ������� 
  _EINT();                              //�����ж�
  while(1)
  { 
    for(i=0;i<10;i++)
    {
      R1++;
      if(R1>9)
      {
        R1=0;
        R2++;
        if(R2>5)
        {
          R2=0;
          R3++;
          if(R3>9)
          {
           R3=0;
           R4++;
           if(R4>5)
           {
             R4=0;
             R5++;
           }
          }
        }
      }
      LCD_Display();
      Delay_ms(1000);                 //һ�����ʱ
    }
  }    
} 