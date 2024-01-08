/****************************************************
  ��ʵ�����������ߴ���nRF2401�����Բ���������ͬ�ط����¶ȡ�
  
  �����ǣ�����������������ʱ��LED��˸��
          ���������޷���������ʱ��LEDһֱ����ͬʱLCD��ʾ�������¶ȡ�

  �������������ܵ������أ�������nRF2401��ͨ�ϣ�������MSP430˯��ʱ��
  �������ܹر�nRF2401���Խ��͹��ġ�
  
  ��MSP430˯��ʱ������LPM4�����Խ���0.4uA(HT1621B-0.3uA,MSP430-0.1uA)

�ر�ע�⣺������˯��ʱ��һ���ȹر�LCD,������IO��
          Ҫ��Ȼ�ز���LCD


************************Ӳ������*********************

                    nRF 2401������
        MISO -------------------------------P1.0
        MOSI -------------------------------P1.1
        SCK  -------------------------------P1.2
        CE   -------------------------------P1.3
        CSN  -------------------------------P1.4
        IRQ  -------------------------------P1.5
        nRF 2401 + -------------------------VCC
        nRF 2401 - -------------------------nRF 2401��������9013-C(3)

              nRF 2401��������9013������
        nRF 2401��������9013(2)-------------P2.3 
        nRF 2401��������9013(1)-------------GND

                    HT1621������
        HT1621_DAT--------------------------P2.0
        HT1621_CS --------------------------P2.1
        HT1621_WR --------------------------P2.2
                 ����������
        ˯��sleep��LPM4��-------------------P2.4
        ���ѣ�LPM4_EXIT) -------------------P2.5 

******************************************************/



#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

#define nRF2401_9013    BIT3
#define nRF2401_9013_H  P2OUT |= BIT3
#define nRF2401_9013_L  P2OUT &= ~BIT3

volatile uchar sleep=0;

#define BIAS    0x52  //0b1000 0100 1010 1/3duty 3com
#define SYSDIS  0X00  //0b1000 0000 0000 ����ϵͳ������LCDƫѹ������
#define SYSEN   0X02  //0b1000 0000 0010 ��ϵͳ����
#define LCDOFF  0X04  //0b1000 0000 0100 ��LCDƫѹ
#define LCDON   0X06  //0b1000 0000 0110 ��LCDƫѹ
#define TIMERDIS 0X08  //0b1000 0000 0110 ��ֹʱ�����
#define XTAL    0x28  //0b1000 0010 1000 �ⲿ��ʱ��
#define RC256   0X30  //0b1000 0011 0000 �ڲ�ʱ��
#define TONEON  0X12 //0b1000 0001 0010 ���������
#define TONEOFF 0X10 //0b1000 0001 0000 �ر��������
#define WDTDIS  0X0A  //0b1000 0000 1010 ��ֹ���Ź�
#define HT1621_DAT_H P2OUT|=BIT0
#define HT1621_DAT_L P2OUT&=~BIT0
#define HT1621_CS_H  P2OUT|=BIT1
#define HT1621_CS_L  P2OUT&=~BIT1
#define HT1621_WR_H  P2OUT|=BIT2
#define HT1621_WR_L  P2OUT&=~BIT2

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

#define TX_ADDR_WITDH 5//���͵�ַ�������Ϊ5���ֽ�
#define RX_ADDR_WITDH 5//���յ�ַ�������Ϊ5���ֽ�
#define TX_DATA_WITDH 8//
#define RX_DATA_WITDH 8


//nRF24L01��Ӧ����

#define MISO   BIT0                //input
#define MISO_H P1OUT |= BIT0
#define MISO_L P1OUT &= ~BIT0

#define MOSI   BIT1
#define MOSI_H P1OUT |= BIT1
#define MOSI_L P1OUT &= ~BIT1

#define SCK    BIT2
#define SCK_H  P1OUT |= BIT2
#define SCK_L  P1OUT &= ~BIT2

#define CE     BIT3
#define CE_H   P1OUT |= BIT3
#define CE_L   P1OUT &= ~BIT3

#define CSN    BIT4
#define CSN_H  P1OUT |= BIT4
#define CSN_L  P1OUT &= ~BIT4

#define IRQ    BIT5          //input 
#define IRQ_H  P1OUT |=BIT5 
#define IRQ_L  P1OUT &= ~BIT5

#define  LED   BIT6
#define  LED_H P1OUT |= BIT6;
#define  LED_L P1OUT &= ~BIT6;

#define R_REGISTER    0x00  // ���Ĵ���
#define W_REGISTER    0x20  // д�Ĵ���
#define R_RX_PLOAD    0x61  // ��RX FIFO��Ч���ݣ�1-32�ֽڣ�����������ɺ����ݱ������Ӧ���ڽ���ģʽ
#define W_TX_PLOAD    0xA0  // дTX FIFO��Ч���ݣ�1-32�ֽڣ�д�������ֽ�0��ʼ��Ӧ���ڷ���ģʽ
#define FLUSH_TX    0xE1  // ���TX FIFO�Ĵ�����Ӧ���ڷ���ģʽ
#define FLUSH_RX    0xE2  // ���RX FIFO�Ĵ�����Ӧ���ڽ���ģʽ
#define REUSE_TX_PL 0xE3  // ����ʹ����һ����Ч���ݣ���CEΪ�߹����У����ݰ������ϵ����·���
#define NOP         0xFF  // �ղ���������������״̬�Ĵ���

#define CONFIG      0x00  // ���üĴ���
#define EN_AA       0x01  // ���Զ�Ӧ�𡱹��ܼĴ�
#define EN_RX_ADDR  0x02  // ����ͨ��ʹ�ܼĴ���
#define SETUP_AW    0x03  // ��ַ������üĴ���
#define SETUP_RETR  0x04  // �Զ��ط����üĴ���
#define RF_CH       0x05  // ��Ƶͨ��Ƶ�����üĴ���
#define RF_SETUP    0x06  // ��Ƶ���üĴ���
#define STATUS      0x07  // ״̬�Ĵ���
#define OBSERVE_TX  0x08  // ���ͼ��Ĵ���
#define CD          0x09  // �ز����Ĵ���
#define RX_ADDR_P0  0x0A  // ����ͨ��0���յ�ַ�Ĵ���
#define RX_ADDR_P1  0x0B  // ����ͨ��1���յ�ַ�Ĵ���
#define RX_ADDR_P2  0x0C  // ����ͨ��2���յ�ַ�Ĵ���
#define RX_ADDR_P3  0x0D  // ����ͨ��3���յ�ַ�Ĵ���
#define RX_ADDR_P4  0x0E  // ����ͨ��4���յ�ַ�Ĵ���
#define RX_ADDR_P5  0x0F  // ����ͨ��5���յ�ַ�Ĵ���
#define TX_ADDR     0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0    0x11  // ����ͨ��0��Ч���ݿ�����üĴ���
#define RX_PW_P1    0x12  // ����ͨ��1��Ч���ݿ�����üĴ���
#define RX_PW_P2    0x13  // ����ͨ��2��Ч���ݿ�����üĴ���
#define RX_PW_P3    0x14  // ����ͨ��3��Ч���ݿ�����üĴ���
#define RX_PW_P4    0x15  // ����ͨ��4��Ч���ݿ�����üĴ���
#define RX_PW_P5    0x16  // ����ͨ��5��Ч���ݿ�����üĴ���
#define FIFO_STATUS 0x17  // FIFO״̬�Ĵ���

uchar sta;                // ״̬����
#define RX_DR  (sta & 0x40)  // ���ճɹ��жϱ�־
#define TX_DS  (sta & 0x20)  // ����ɹ��жϱ�־
#define MAX_RT (sta & 0x10)  // �ط�����жϱ�־

uchar  TX_Addr[]={0x34,0x43,0x10,0x10,0x01};
uchar  TX_Buffer[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar RX_Buffer[RX_DATA_WITDH];


void port_init(void)               //�˿ڳ�ʼ��
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI+LED;
  P1DIR &= ~MISO;
  P2DIR |=BIT0+BIT1+BIT2+nRF2401_9013;
  nRF2401_9013_H; 
  
  P2OUT |=BIT4+BIT5;   //P2.4-5����������
  P2IE  |=BIT4+BIT5;   //P2.4-5�ж�ʹ��
  P2IES |=BIT4+BIT5;   //P2.4-5�ж��½��ش����ж�
  P2REN |=BIT4+BIT5;   //P2.4-5��������ʹ��
  P2IFG &=~(BIT4+BIT5);//P2.4-5�жϱ�־λ����
}
void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO��������=1MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO��������=1MHZ 
}
void int_WDT(void)                 //���Ź�����
{
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void Delay_us(uint us)           // ��ʱus  ǰ����f=8MHZ
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(1);      
}
void Delay_ms(uint ms)           // ��ʱms  ǰ����f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
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

void display (void)
{
       Ht1621WrOneData(0x01,Ht1621Tab1[3*qian]);
       Ht1621WrOneData(0x03,Ht1621Tab1[3*qian+1]);
       Ht1621WrOneData(0x05,Ht1621Tab1[3*qian+2]);//���� Ht1621Tab3
                                                  //��ʾ ��ǰ��1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab2[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab2[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab2[3*shi+2]);//���� Ht1621Tab3 
                                                 //��ʾ��ĸLOW BATTERY
       Ht1621WrOneData(0x13,Ht1621Tab1[3*ge]);   
       Ht1621WrOneData(0x15,Ht1621Tab1[3*ge+1]);
       Ht1621WrOneData(0x17,Ht1621Tab1[3*ge+2]); //���� Ht1621Tab3  
                                                 //��ʾCONTINUITY
}

//nRF24L01��ʼ��
void nRF24L01_Init(void)
{
 Delay_ms(2);
 CE_L;//����ģʽ��
 CSN_H;
 SCK_L;
 IRQ_H;
}

//SPIʱ����
uchar SPI_RW(uchar byte)
{
 uchar i;
 for(i=0;i<8;i++)//һ�ֽ�8λѭ��8��д��
 {
  if(byte&0x80)//����������λ��1
   MOSI_H;//��NRF24L01д1
  else //����д0
   MOSI_L;
  byte<<=1;//��һλ�Ƶ����λ
  SCK_H;//SCK���ߣ�д��һλ���ݣ�ͬʱ��ȡһλ����
  if(P1IN & MISO)
   byte|=0x01;
  SCK_L;//SCK����
 }
 return byte;//���ض�ȡһ�ֽ�
}

//SPIд�Ĵ���һ�ֽں���
//reg:�Ĵ�����ַ
//value:һ�ֽڣ�ֵ��
uchar SPI_W_Reg(uchar reg,uchar value)
{
 uchar status;//����״̬
 CSN_L;//SPIƬѡ
 status=SPI_RW(reg);//д��Ĵ�����ַ��ͬʱ��ȡ״̬
 SPI_RW(value);//д��һ�ֽ�
 CSN_H;//
 return status;//����״̬
}

//SPI��һ�ֽ�
uchar SPI_R_byte(uchar reg)
{
 uchar status;
 CSN_L;
 SPI_RW(reg);
 status=SPI_RW(0);
 CSN_H;
 return status;
}

//SPI��ȡRXFIFO�Ĵ�������
//reg:�Ĵ�����ַ
//Dat_Buffer:�������ȡ������
//DLen:���ݳ���
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen)
{
 uchar reg_value,i;
 CSN_L;
 reg_value=SPI_RW(reg);
 for(i=0;i<Dlen;i++)
 {
  Dat_Buffer[i]=SPI_RW(0);
 }
 CSN_H;
 return reg_value;
}

//SPI��TXFIFO�Ĵ���д������
//reg:д��Ĵ�����ַ
//TX_Dat_Buffer:�����Ҫ���͵�����
//Dlen:���ݳ���
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen)
{
 uchar reg_value,i;
 CSN_L;
 reg_value=SPI_RW(reg);
 for(i=0;i<Dlen;i++)
 {
  SPI_RW(TX_Dat_Buffer[i]);
 }
 CSN_H;
 return reg_value; 
}

//���÷���ģʽ
void nRF24L01_Set_RX_Mode(void)
{
 CE_L;//����
 SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);
 SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,TX_Addr,TX_ADDR_WITDH);
 SPI_W_Reg(W_REGISTER+EN_AA,0x01);//auot ack
 SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);
 SPI_W_Reg(W_REGISTER+SETUP_RETR,0x0a);
 SPI_W_Reg(W_REGISTER+RX_PW_P0,RX_DATA_WITDH);
 SPI_W_Reg(W_REGISTER+RF_CH,0);
 SPI_W_Reg(W_REGISTER+RF_SETUP,0x07);//0db,lna
 SPI_W_Reg(W_REGISTER+CONFIG,0x0f);
 CE_H;
 Delay_ms(5);
}

//���Ӧ���ź�
uchar nRF24L01_RX_Data(void)
{
 sta=SPI_R_byte(R_REGISTER+STATUS);
 if(RX_DR)
 {
  CE_L;
  SPI_R_DBuffer(R_RX_PLOAD,RX_Buffer,RX_DATA_WITDH);
  SPI_W_Reg(W_REGISTER+STATUS,0xff);
  CSN_L;
  SPI_RW(FLUSH_RX);
  CSN_H;
  return 1;
 }
 else
  return 0;
 
}

void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;     //watchdog
}

void stop_IO(void)
{
  P1DIR  = 0xff;      
  P1OUT  = 0xff;       
  P2DIR |= 0xcf;          //1100 1111 ���˰��������඼Ϊ���
  P2OUT |= 0xc7;          //1100 0111 ���˰�����P2.3(������)����Ϊ��,���඼�����
  P2OUT &= ~nRF2401_9013; 
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
}

void main(void)
{
   //uchar i;
   init_devices();
   
   Ht1621_Init();                //�ϵ��ʼ��LCD
   Delay_ms(1);                  //��ʱһ��ʱ��
   Ht1621WrAllData(0,Ht1621Tab,16);
   
   Delay_ms(5);
   nRF24L01_Init();//NRF24L01��ʼ��
   _EINT();                          //�����ж�
   while(1)
 {
  nRF24L01_Set_RX_Mode();
  Delay_ms(1);
  if(nRF24L01_RX_Data())
  {
    LED_L;                       //����������յ�����
    ge=RX_Buffer[0];
    shi=RX_Buffer[1];
    bai=RX_Buffer[2];
    qian=RX_Buffer[3];
    display();
   // Delay_ms(50);
    //Ht1621WrAllData(0,Ht1621Tab,16);
  }
  else                          //�����Ϩ 
  LED_H;
  
  if(sleep)
  {
    stop_lcd();                 //�ر�ע�⣺һ���ȹر�LCD
                                //          ������IO�ڣ�Ҫ��Ȼ�ز���LCD
    stop_IO();  
    LPM4;
    
    init_devices();
    Ht1621_Init();               //�ϵ��ʼ��LCD
    sleep=0;
  }
 }   
}

//P2�жϷ������

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{ 
    Delay_ms(10);            //����10ms
    if((P2IN & BIT4)==0)
    {
      sleep=1;               //��˯�߱�־λ��λ��׼������
    }
    if((P2IN & BIT5)==0)
    {
      sleep=0; 
      LPM4_EXIT;             //�˳�˯��LPM4
    }
    P2IFG =0;                //P2�жϱ�־λ����
}  
