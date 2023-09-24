/**************************************************

      ��ʵ�������¶ȴ�����DS18B20����ͨ��HT1621������λ��Һ��LCD
      ����ʾ��ǰ���¶ȣ���ȷ��0.6C����������������ֱ������DS18B20��
      ��MSP430����˯��ʱ�����Թر�DS18B20���ﵽʡ���Ŀ�ġ�

ע�⣺
     1.P1.0��P1.4����ȱ�����,��������P1.0��ACLK���ⲿ�ֱ�ʱ�Ӽ���Ƿ����LPM4 
       P1.4 (SMCLK)�ڲ�DCO,Ϊ�˲����Ƿ����͹���ģʽLPM3,Ҫ��û���������
       ˵���Ѿ�����LPM3��
     2.������͹��ĵ�ʱ��һ��Ҫ���ú�IO�ڣ�������,�ض�������P1.0���õ�IO�����������⣬
       ����һ��Ҫ���������P1.0һ���õͣ��ض�������9013��
       ���ر�DS18B20.�������������øߵ�ƽ��

///////////////////////Ӳ������/////////////////////
            
      Һ������HT1621D��7����   ����HT1621_DAT-------------------P1.5
      Һ������HT1621D��4����   ����HT1621_CS--------------------P1.1
      Һ������HT1621D��6����   ����HT1621_WR--------------------P1.2
      �¶ȴ�����DS18B20��2���� ����DS18B20----------------------P1.3
      �¶ȴ�����DS18B20��1���� ����DS18B20_GND-------������9013��3�ţ�C��
      ˯�߰���------------------------------------------------- P2.0 
      �����¶Ȱ���--------------------------------------------- P2.1
      ���ѷ��밴��--------------------------------------------- P2.2
      ʱ��ֵĵ��ڰ���----------------------------------------- P2.3
      ����ʱ����----------------------------------------------- P2.4
      ʱ��ʱ�ĵ��ڰ���----------------------------------------- P2.5

*************************************************/
#include <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned

//������HT1621 LCD��ʾ�궨��
#define BIAS    0x52  //0b1000 0100 1010 1/3duty 3com
#define SYSDIS  0X00  //0b1000 0000 0000 ����ϵͳ������LCDƫѹ������
#define SYSEN   0X02  //0b1000 0000 0010 ��ϵͳ����
#define LCDOFF  0X04  //0b1000 0000 0100 ��LCDƫѹ
#define LCDON   0X06  //0b1000 0000 0110 ��LCDƫѹ
#define TIMERDIS 0X08  //0b1000 0000 0110 ��ֹʱ�����
#define XTAL    0x28  //0b1000 0010 1000 �ⲿ��ʱ��
#define RC256   0X30  //0b1000 0011 0000 �ڲ�ʱ��
#define TONEON  0X12  //0b1000 0001 0010 ���������
#define TONEOFF 0X10  //0b1000 0001 0000 �ر��������
#define WDTDIS  0X0A  //0b1000 0000 1010 ��ֹ���Ź�
#define HT1621_DAT_H P1OUT|=BIT5
#define HT1621_DAT_L P1OUT&=~BIT5
#define HT1621_CS_H  P1OUT|=BIT1
#define HT1621_CS_L  P1OUT&=~BIT1
#define HT1621_WR_H  P1OUT|=BIT2
#define HT1621_WR_L  P1OUT&=~BIT2

//������DS18B20��ʾ�궨��

#define DS18B20      BIT3
#define DS18B20_H    P1OUT |= BIT3
#define DS18B20_L    P1OUT &= ~BIT3

volatile uchar qian=0,bai=0,shi=0,ge=0;//qian,bai,shi,ge �ĸ������ֱ��ʾҺ����ǧ��ʮ��λ
volatile uchar sleep=0, time_flag=0,temp_flag=1,shi_flag=0,huanxing=0; 
  //sleepΪ˯�߱�־λ�� temp_flagΪ�¶Ȼ��ѱ�־λ,time_flagΪʱ�份�ѱ�־λ
  //shi_flagΪ��ʾʱ���ѱ�־λ��huangxingΪ������ʾ��־λ
volatile uint jishu=0;               //���ӱ�������
volatile uchar hour=0,min=0,sec=0;   //���ӱ��ʱ���֣���
uchar Ht1621Tab[]={0x00};            //LCD��������
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
uchar Ht1621Tab3[]={0x0c,0x0a,0x0e,  //0  ��ʾ��ǰ��1��LOW BATTERY CONTINUITY
                    0x00,0x00,0x0e,  //1
                    0x04,0x0e,0x0a,  //2
                    0x00,0x0e,0x0e,  //3
                    0x08,0x04,0x0e,  //4
                    0x08,0x0e,0x06,  //5
                    0x0c,0x0e,0x06,  //6
                    0x00,0x08,0x0e,  //7
                    0x0c,0x0e,0x0e,  //8
                    0x08,0x0e,0x0e}; //9
/******************************************************

                   ��������

****************************************************/
void display (void);            //Һ����ʾ
void display_shi(void);         //Һ����ʾ�м���λ
void temp_dsp(void);            //�¶ȴ�����
void time_dsp(void);            //���봦����
void shi_dsp(void);             //ʱ������
void stop_lcd(void);

/////////////  DS18B20��������   //////
void B20_init(void);
uchar B20_readB(void);
void B20_writeB(uchar wrd);
uint Read_temp(void);

void port_init(void)            //�˿ڳ�ʼ��
{
  P1DIR |= BIT0+BIT1+BIT2+BIT3+BIT5; //P1.1 P1.2 P1.3 P1.5 OUT
  P1OUT |= BIT0;                     //P1.0���ڿ��������ܵĹض�
                                     //�ߵ�ƽʱ�������ܵ�ͨ����DS18B20����
                                     //�͵�ƽʱ��
  P2OUT |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5����������
  P2IE  |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5�ж�ʹ��
  P2IES |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5�ж��½��ش����ж�
  P2REN |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5��������ʹ��
  P2IFG &=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);//P2.0-5�жϱ�־λ����
}
void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO��������=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO��������=8MHZ
}
void int_WDT(void)                 //���Ź�����
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void int_Time_A(void)              //��ʱ��A��ʼ��
{
  TACTL = TASSEL_1 +ID_3+ MC_1;    // ACLK��8div,upmode
  CCTL0 = CCIE;                    //CCR0 interrupt enabled
  CCR0 = 4096;                     //��ʱ1s t=1S
}
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
    Delay_ms(20);       //��ʱʹLCD������ѹ�ȶ�
    Ht1621WrCmd(BIAS);  //1/3duty 3com
    Ht1621WrCmd(RC256); //ʹ���ڲ�����
    Ht1621WrCmd(SYSDIS);//����ϵͳ������LCDƫѹ������
    Ht1621WrCmd(WDTDIS);//��ֹ���Ź�
    Ht1621WrCmd(TONEOFF);//�ر��������
    Ht1621WrCmd(SYSEN); //��ϵͳ����
    Ht1621WrCmd(LCDON); //��LCDƫѹ
}

//������HT1621 LCD��ʾ�ӳ���

void init_devices(void)
{
   port_init();                     //IO������
   int_Clock();                     //ʱ������
   int_WDT();                       //���Ź�����
}
void main( void )
{  
   init_devices();                   // ��ʼ������
   int_Time_A();                     //��ʱ��A ������
   Ht1621_Init();                    //�ϵ��ʼ��LCD
   Ht1621WrAllData(0,Ht1621Tab,16);  //LCD����
   _EINT();                          //�����ж�
  while(1)
  {   
       if(temp_flag)                 //�Ƿ���ʾ�¶��ж�
        {
           temp_dsp();               //�¶Ȳɼ������ݴ���
           display();                //����LCD��λ��ʾ
        }
       if(time_flag)                 //�Ƿ���ʾ�ֺ����ж�
        {
         time_dsp();
         display();                  //����LCD��λ��ʾ
        }
       if(shi_flag)                 //�Ƿ���ʾʱ�ж�
       {
         shi_dsp();
         display_shi();
       }
       if(sleep)                    //˯�߱�־λ
       {
         stop_lcd();                //�ر�LCD�Լ�����IO�����
         LPM3;                      //����˯��LPM4
        }
       if(huanxing)                 //�����¶ȳ�ʼ��
       {
         init_devices();            // ��ʼ������
         Ht1621_Init();             //�ϵ��ʼ��LCD
         huanxing=0;
        } 
  }
}
void display (void)                               //��ʾ��λ�ӳ���
{
       Ht1621WrOneData(0x01,Ht1621Tab1[3*qian]);
       Ht1621WrOneData(0x03,Ht1621Tab1[3*qian+1]);
       Ht1621WrOneData(0x05,Ht1621Tab1[3*qian+2]);//�ڴ˵�ַ���� Ht1621Tab3
                                                  //��ʾ ��ǰ��1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab2[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab2[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab3[3*shi+2]);//�ڴ˵�ַ���� Ht1621Tab3 
                                                 //��ʾ��ĸLOW BATTERY
       Ht1621WrOneData(0x13,Ht1621Tab1[3*ge]);   
       Ht1621WrOneData(0x15,Ht1621Tab1[3*ge+1]);
       Ht1621WrOneData(0x17,Ht1621Tab3[3*ge+2]); //�ڴ˵�ַ���� Ht1621Tab3  
                                                 //��ʾCONTINUITY
}

void display_shi(void)                           //��ʾ�м���λ�ӳ���
{
       Ht1621WrOneData(0x01,0x00);
       Ht1621WrOneData(0x03,0x00);
       Ht1621WrOneData(0x05,0x00);                //�ڴ˵�ַ���� Ht1621Tab3
                                                  //��ʾ ��ǰ��1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab1[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab1[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab1[3*shi+2]);//�ڴ˵�ַ���� Ht1621Tab3 
                                                 //��ʾ��ĸLOW BATTERY
       Ht1621WrOneData(0x13,0x00);   
       Ht1621WrOneData(0x15,0x00);
       Ht1621WrOneData(0x17,0x00);               //�ڴ˵�ַ���� Ht1621Tab3  
                                                 //��ʾCONTINUITY
}

//������DS18B20�¶ȼ���ӳ���
/********************************
��������: B20_init
��    ��: ��λDS18B20
��    ��: ��
����ֵ  : ��
********************************/
void B20_init(void)
{
 P1DIR |= DS18B20;		     //����Ϊ���			  
 DS18B20_L;			     //����
 Delay_us(600);			     //�ȴ�600΢��
 DS18B20_H;			     //�ͷ�����
 Delay_us(60);			     //�ȴ�60΢��
 P1DIR &= ~DS18B20;		     //����Ϊ���� 
 while((P1IN &(DS18B20)));	     //�ȴ�DS18B20����
 while(!(P1IN &(DS18B20)));          //�ȴ�DS18B20�ͷ�����
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
  P1DIR |=DS18B20;		//����Ϊ���
  DS18B20_L;		        //���ͣ�����������λ
  DS18B20_H;			//�ͷ�����
  Delay_us(5);			//�ȴ�5΢��
  P1DIR &=~DS18B20;		//����Ϊ���룬��ʼ��ȡ����λ
  if(P1IN&DS18B20)	        //��λ�Ƿ�Ϊ��
  {
   retd|=0x80;			//�Ǿͽ���λ�ø�
  }
  Delay_us(50);			//�ȴ�50΢��
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
  P1DIR |=DS18B20;		//����Ϊ���
  DS18B20_L;		        //���ͣ�����д����λ
  Delay_us(1);			//�ȴ�1΢��
  if(wrd&0x01)			//��λ�����Ƿ�Ϊ��
  {
   DS18B20_H;		        //�Ǹ��򽫵���������
  }
  else
  {
   DS18B20_L;		        //�ǵ��򽫵���������
  }
  Delay_us(50);		        //�ȴ�50΢��
  DS18B20_H;		        //�ͷ�����
  wrd>>=1;			//���ƣ�Ϊд���µ�����λ��׼��
 }
  Delay_us(50);			//�ȴ�50΢��
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
 B20_init();	   //��ʼ����ÿ��д����ӳ�ʼ����ʼ
 B20_writeB(0xcc); //����ROM
 B20_writeB(0x44); //�����¶�ת��
 B20_init();	   //��ʼ����ÿ��д����ӳ�ʼ����ʼ
 B20_writeB(0xcc); //����ROM
 B20_writeB(0xbe); //���Ĵ���
 templ=B20_readB();//���¶ȵ��ֽ�
 temph=B20_readB();//���¶ȸ��ֽ�
 temp=templ+temph*256;//���¶������16λ����
 
  return temp;	   //����16λ����

}
//������DS18B20�¶ȼ���ӳ���

void temp_dsp(void)   //
{
   uint t;
   t=Read_temp();                    //��ȡ�¶�ֵ
   t=t*6.25;
   qian=t/1000;
   bai=(t-qian*1000)/100;
   shi=((t-qian*1000)-bai*100)/10;
   ge=t%10;   
}
void time_dsp(void)
{
  if(jishu>43200)
  {
     jishu=0;
  }
      min=jishu%3600/60;
      sec=jishu%60;
      
      qian=min/10;
      bai=min%10;
      shi=sec/10;
      ge=sec%10;
}

void shi_dsp(void)
{
  if(jishu>43200)
  {
     jishu=0;
  }
    hour=jishu/3600;
    bai=hour/10;
    shi=hour%10;   
}

void stop_lcd(void)      //�ر�LCD�ӳ���
{
    HT1621_CS_H;
    HT1621_WR_H;
    HT1621_DAT_H;
    Ht1621WrCmd(LCDOFF); //��LCDƫѹ
    Ht1621WrCmd(TONEOFF);//�ر��������
                         //��ʵ���û���Ҫ�أ���Ϊ��ʼ��û�д�
    Ht1621WrCmd(TIMERDIS);
    Ht1621WrCmd(SYSDIS); //����ϵͳ������LCDƫѹ������
      
    P1DIR=0XFF;        //����IO
    P1OUT =0XFE;       //P1.0һ��Ҫ�õͣ��ض������ܣ����ر�DS18B20
    P2DIR=0xC0;        //
    P2OUT |=0xC0;      //
}
//timeA��ʱ�жϷ������
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  jishu++;
}

//P2�жϷ������

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{ 
  if ((P2IN&BIT0)==0)
  {
    Delay_ms(10);            //����10ms
    if((P2IN&BIT0)==0)
    {
      sleep=1;               //����˯��LPM3
    }
  }
  if((P2IN&BIT1)==0)
  {
    Delay_ms(10);            //����10ms
    if((P2IN&BIT1)==0)
    {
      huanxing=1;            //����LPM3
      temp_flag=1;           //�¶ȱ�־λ��1��       ��ʾ�¶�
      time_flag=0;           //ʱ������־λ��0��   ͬʱ�رշ�����ʾ
      shi_flag=0;            //ʱ��־λ��0��         ͬʱ�ر�ʱ��ʾ
      sleep=0;               //ͬʱ��˯�߱�־λ���㣬��ʼ��ʾ�¶�
      LPM3_EXIT;             //�˳�˯��LPM3
     }
   }
  if((P2IN&BIT2)==0)
  {
    Delay_ms(10);            //����10ms
    if((P2IN&BIT2)==0)
    {
      huanxing=1;            //����LPM3
      time_flag=1;           //ʱ������־λ��1��   ��ʾ����
      temp_flag=0;           //�¶ȱ�־λ��0��       ͬʱ�ر��¶���ʾ
      shi_flag=0;            //ʱ��־λ��0��         ͬʱ�ر�ʱ����ʾ
      sleep=0;               //ͬʱ��˯�߱�־λ���㣬��ʼ��ʾ�¶�
      LPM3_EXIT;             //�˳�˯��LPM3
    }
  }
  if((P2IN & BIT3)==0)
  {
    Delay_ms(10);            //����10ms
    if((P2IN & BIT3)==0)
    {
      jishu=jishu+60;        //ʱ��ֵļ�һ��(ǰ���ǻ���ʱ)  
      Delay_ms(10);          //����10ms
    }
  }
  
  if((P2IN & BIT4)==0)
  {
    Delay_ms(10);            //����10ms
    if((P2IN & BIT4)==0)
    {
      huanxing=1;            //����LPM3
      shi_flag=1;            //ʱ��ʱ��־λ��1��     ��ʾʱ
      time_flag=0;           //ʱ������־λ��0��   ͬʱ�رշ������ʾ
      temp_flag=0;           //�¶ȱ�־λ��0��       ͬʱ�رշ������ʾ
      sleep=0;               //ͬʱ��˯�߱�־λ���㣬��ʼ��ʾʱ
      LPM3_EXIT;             //�˳�˯��LPM3
    }
  }
  
  if((P2IN & BIT5)==0)
  {
    Delay_ms(10);            //����10ms
    if((P2IN & BIT5)==0)
    {
      jishu=jishu+3600;      //ʱ��ʱ�ļ�һ��(ǰ���ǻ���ʱ) 
      Delay_ms(10);          //����10ms
    }
  }
    P2IFG =0;                //P2�жϱ�־λ����
}  
