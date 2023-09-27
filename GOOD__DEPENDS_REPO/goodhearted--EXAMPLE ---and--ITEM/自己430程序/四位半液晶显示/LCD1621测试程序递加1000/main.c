#include <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned 
#define BIAS    0x52  //0b1000 0100 1010 1/3duty 3com
#define SYSDIS  0X00  //0b1000 0000 0000 ����ϵͳ������LCDƫѹ������
#define SYSEN   0X02  //0b1000 0000 0010 ��ϵͳ����
#define LCDOFF  0X04  //0b1000 0000 0100 ��LCDƫѹ
#define LCDON   0X06  //0b1000 0000 0110 ��LCDƫѹ
#define XTAL    0x28  //0b1000 0010 1000 �ⲿ��ʱ��
#define RC256   0X30  //0b1000 0011 0000 �ڲ�ʱ��
#define TONEON  0X12 //0b1000 0001 0010 ���������
#define TONEOFF 0X10 //0b1000 0001 0000 �ر��������
#define WDTDIS  0X0A  //0b1000 0000 1010 ��ֹ���Ź�
#define HT1621_DAT_H P1OUT|=BIT0
#define HT1621_DAT_L P1OUT&=~BIT0
#define HT1621_CS_H  P1OUT|=BIT1
#define HT1621_CS_L  P1OUT&=~BIT1
#define HT1621_WR_H  P1OUT|=BIT2
#define HT1621_WR_L  P1OUT&=~BIT2
volatile uchar qian=0,bai=0,shi=0,ge=0;//qian,bai,shi,ge �ĸ������ֱ��ʾҺ����ǧ��ʮ��λ
uchar Ht1621Tab[]={0x00};
uchar Ht1621Tab1[]={0x0c,0x0a,0x0c,  //0  ����С������
                    0x00,0x00,0x0c,  //1
                    0x04,0x0e,0x08,  //2
                    0x00,0x0e,0x0c,  //3
                    0x08,0x04,0x0c,  //4
                    0x08,0x0e,0x04,  //5
                    0x0c,0x0e,0x04,  //6
                    0x00,0x08,0x0c,  //7
                    0x0c,0x0e,0x0c,  //8
                    0x08,0x0e,0x0c}; //9
uchar Ht1621Tab2[]={0x0e,0x0a,0x0c,  //0    ��С������
                    0x02,0x00,0x0c,  //1
                    0x06,0x0e,0x08,  //2
                    0x02,0x0e,0x0c,  //3
                    0x0a,0x04,0x0c,  //4
                    0x0a,0x0e,0x04,  //5
                    0x0e,0x0e,0x04,  //6
                    0x02,0x08,0x0c,  //7
                    0x0e,0x0e,0x0c,  //8
                    0x0a,0x0e,0x0c}; //9
uchar Ht1621Tab3[]={0x0c,0x0a,0x0e,  //0
                    0x00,0x00,0x0e,  //1
                    0x04,0x0e,0x0a,  //2
                    0x00,0x0e,0x0c,  //3
                    0x08,0x04,0x0c,  //4
                    0x08,0x0e,0x04,  //5
                    0x0c,0x0e,0x04,  //6
                    0x00,0x08,0x0c,  //7
                    0x0c,0x0e,0x0c,  //8
                    0x08,0x0e,0x0c}; //9
void display (void);
void port_init(void)               //�˿ڳ�ʼ��
{
  P1DIR |=BIT0+BIT1+BIT2;  
}
void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  BCSCTL1 = CALBC1_1MHZ;           //DCO��������=1MHZ
  DCOCTL  = CALBC1_1MHZ;           //DCO��������=1MHZ 
}
void int_WDT(void)                 //���Ź�����
{
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void Delay_us(uint us)           // ��ʱus  ǰ����f=1MHZ
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(1);      
}
void Delay_ms(uint ms)           // ��ʱms  ǰ����f=1MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(1000); 
}
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
�������ƣ�void Ht1621WrOneData(uchar Addr,uchar Data)
��������: HT1621��ָ����ַд�����ݺ���
ȫ�ֱ�������
����˵����AddrΪд���ʼ��ַ��DataΪд������
˵������ΪHT1621������λ4λ������ʵ��д������Ϊ�����ĺ�4λ
********************************************************/
void Ht1621WrOneData(uchar Addr,uchar Data)
{
    HT1621_CS_L;
    Ht1621Wr_Data(0xa0,3);    //д�����ݱ�־101
    Ht1621Wr_Data(Addr<<2,6); //д���ַ����
    Ht1621Wr_Data(Data<<4,4); //д������
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
        Ht1621Wr_Data(0x00,8); //д������
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
    Delay_ms(200); //��ʱʹLCD������ѹ�ȶ�
    Ht1621WrCmd(BIAS);
    Ht1621WrCmd(RC256); //ʹ���ڲ�����
    Ht1621WrCmd(SYSDIS);
    Ht1621WrCmd(WDTDIS);
    Ht1621WrCmd(SYSEN);
    Ht1621WrCmd(LCDON);
}
void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;     //watchdog
}
void main( void )
{
   init_devices();               // ��ʼ������
   Ht1621_Init();                //�ϵ��ʼ��LCD
   Delay_ms(1);                  //��ʱһ��ʱ��
   Ht1621WrAllData(0,Ht1621Tab,16);
   while(1)
     {
       ge++;
       if(ge>9)
       {
         ge=0;
         shi++;
         if(shi>9)
         {
          shi=0;
          bai++;
          if(bai>9)
          {
            bai=0;
            qian++;
            if(qian>9)
            {
              qian=0;
            }
          }
         }
       }
       display ();
       Delay_ms(100);
       Ht1621WrAllData(0,Ht1621Tab,16);
      }
}
void display (void)
{
       Ht1621WrOneData(0x01,Ht1621Tab1[3*qian]);
       Ht1621WrOneData(0x03,Ht1621Tab1[3*qian+1]);
       Ht1621WrOneData(0x05,Ht1621Tab1[3*qian+2]);//���� Ht1621Tab3
                                                  //��ʾ ��ǰ��1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab1[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab1[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab1[3*shi+2]);//���� Ht1621Tab3 
                                                 //��ʾ��ĸLOW BATTERY
       Ht1621WrOneData(0x13,Ht1621Tab1[3*ge]);   
       Ht1621WrOneData(0x15,Ht1621Tab1[3*ge+1]);
       Ht1621WrOneData(0x17,Ht1621Tab1[3*ge+2]); //���� Ht1621Tab3  
                                                 //��ʾCONTINUITY
}
