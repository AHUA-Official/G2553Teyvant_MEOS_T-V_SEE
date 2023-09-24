#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

//������74HC595����ĺ궨��
#define HC595_Store_H P2OUT |= BIT0
#define HC595_Store_L P2OUT &= ~ BIT0
#define HC595_A_H     P2OUT |= BIT1
#define HC595_A_L     P2OUT &= ~ BIT1
#define HC595_Shift_H P2OUT |= BIT2
#define HC595_Shift_L P2OUT &= ~ BIT2
//������74HC595����ĺ궨��

unsigned char  LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                                0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71,//��������ܶ�ѡ����(0-F��
                                0xBf,0x86,0xDb,0xCf,0xE6,0xEd,0xFd,0x87,    
                                0xff,0xef,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,};//���������(��С����)��ѡ����(0-F��
unsigned char  LedPos[] = {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}; //�����λѡ���飬�ֱ�ѡͨÿһλ�����

void HC595shift(void);

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
  P2DIR |= (BIT0+BIT1+BIT2);
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
  __delay_cycles(8000); 
}

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

//���ý���ģʽ
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
void main(void)
{
   //uchar i;
   init_devices();
   Delay_ms(5);
   nRF24L01_Init();//NRF24L01��ʼ��
   
   while(1)
 {
  nRF24L01_Set_RX_Mode();
  Delay_ms(1);
  if(nRF24L01_RX_Data())
  {
    LED_H;                       //����������յ�����
    //Delay_ms(10);
//    if(RX_Buffer[0]==0x3f)
    display(0,RX_Buffer[0]);
    Delay_ms(2);
    display(1,RX_Buffer[1]);
    Delay_ms(2);
    display(2,RX_Buffer[2]);
    Delay_ms(2);
    display(3,RX_Buffer[3]);
    Delay_ms(2);
    display(4,RX_Buffer[4]);
    Delay_ms(2);
    display(5,RX_Buffer[5]);
    Delay_ms(2);
    display(6,RX_Buffer[6]);
    Delay_ms(2);
    display(7,RX_Buffer[7]);
    Delay_ms(1);  
  }
  else                          //�����Ϩ
   LED_L;
 }   
}


