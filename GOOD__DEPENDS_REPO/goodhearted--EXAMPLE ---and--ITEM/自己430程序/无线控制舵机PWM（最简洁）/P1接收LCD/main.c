/****************************************************
           ��ʵ������������nRF24L01������ճ���.
           ��������ң��С����

           ������6λLCD�ú���λ��ʾ��ǰ�����PWM_24��ֵ
              
************************Ӳ������*********************

                    nRF 2401������
             nRF 2401������(6��IO)
        CE   -------------------------------P1.0
        SCK  -------------------------------P1.1
        MISO -------------------------------P1.2
        CSN  -------------------------------P1.3
        MOSI -------------------------------P1.4
        IRQ  -------------------------------P1.5
        nRF 2401 + -------------------------VCC
        nRF 2401 -  ------------------------GND

        6λLCD HT1621�����ӣ������۲�PWM_24����ռ�ձ�

        HT1621_DI ..................P2.0  //HT1621��������
        HT1621_CLK..................P2.1  //HT1621ʱ������(CLK) 
        HT1621_CS ..................P2.2  //HT1621ʹ������ 
        
        ������Ƶ�����

        ������ƽ�..................P2.4  //


******************************************************/

#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO��������=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO��������=8MHZ
}
void int_WDT(void)                 //���Ź�����
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
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

#define PWM_24       BIT4   // P2_4 ������������

//������HT1621 LCD��ʾ��������

//HT1621����λ��Һ��ģ��ӿڶ��壬�������ѵ���Ҫ���ģ� 
#define HT1621_DI     BIT0  //HT1621��������
#define HT1621_CLK    BIT1  //HT1621ʱ������ 
#define HT1621_CS     BIT2  //HT1621ʹ������ 

#define HT1621_DI_H P2OUT |= BIT0
#define HT1621_DI_L P2OUT &= ~BIT0
#define HT1621_CLK_H  P2OUT |= BIT1
#define HT1621_CLK_L  P2OUT &= ~BIT1
#define HT1621_CS_H  P2OUT |= BIT2
#define HT1621_CS_L  P2OUT &= ~BIT2

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
uchar  Ht1621Tab_0[]={0x00};                       //��������
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

/******************************************************
д���ݺ���,cntΪ��������λ��,���ݴ���Ϊ��λ��ǰ
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
    uchar i;
    for (i=0;i<cnt;i++)
     {
        HT1621_CLK_L;
        Delay_us(1);
        if(Data&0x80)
          HT1621_DI_H;
        else
          HT1621_DI_L;
        Delay_us(1);
        HT1621_CLK_H;
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
    HT1621_CLK_H;
    HT1621_DI_H;
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
      Ht1621Tab[2]=0x00;       //������ߵ�һλ�����
      Ht1621Tab[3]=0x00;       //����ʾС���㣬�ڴ˵���SEG_8[R6][1]

      Ht1621Tab[4]=0x00;       //������ߵڶ�λ�����
      Ht1621Tab[5]=0x00;       //����ʾС���㣬�ڴ˵���SEG_8[R5][1]
      
      Ht1621Tab[6]=0x00;       //������ߵ���λ�����
      Ht1621Tab[7]=0x00;       //����ʾС���㣬�ڴ˵���SEG_8[R4][1]
      
      Ht1621Tab[8]=0x00;       //������ߵ���λ�����
      Ht1621Tab[9]=0x00;       //����ʾС���㣬�ڴ˵���SEG_8[R3][1]
      
      Ht1621Tab[10]=SEG_7[R2][0];      //����ߵ���λ�����
      Ht1621Tab[11]=SEG_7[R2][1];       //����ʾС���㣬�ڴ˵���SEG_8[R2][1]
      
      Ht1621Tab[12]=SEG_7[R1][0];      //����ߵ���λ�����
      Ht1621Tab[13]=SEG_7[R1][1];     

//      Ht1621Tab[13]=SEG_8[R1][1];     //��ʾʱ����С���� ��(:)COL1
//      Ht1621Tab[14]=0xcc;             //��ʾR1,R2,R3,R4
//      Ht1621Tab[15]=0x40;             //��ʾʱ���С���� ��(:)COL2
//      Ht1621Tab[15]=0x0c;             //��ʾL1,L2  
      Ht1621WrAllData(0,Ht1621Tab,16);//д1621�Ĵ������ݣ�����ʾ    
}
//������HT1621 LCD��ʾ�����Ӻ���

#define TX_ADDR_WITDH 5  //���͵�ַ�������Ϊ5���ֽ�
#define RX_ADDR_WITDH 5  //���յ�ַ�������Ϊ5���ֽ�
#define TX_DATA_WITDH 8  //��������λ��
#define RX_DATA_WITDH 8  //��������λ��

//nRF24L01��Ӧ����

#define CE     BIT0
#define SCK    BIT1
#define MISO   BIT2 //input
#define CSN    BIT3
#define MOSI   BIT4

#define CE_H   P1OUT |= BIT0
#define CE_L   P1OUT &= ~BIT0
#define IRQ    BIT5  

#define SCK_H  P1OUT |= BIT1
#define SCK_L  P1OUT &= ~BIT1              
#define MISO_H P1OUT |= BIT2
#define MISO_L P1OUT &= ~BIT2
#define CSN_H  P1OUT |= BIT3
#define CSN_L  P1OUT &= ~BIT3
#define MOSI_H P1OUT |= BIT4
#define MOSI_L P1OUT &= ~BIT4       
#define IRQ_H  P1OUT |=BIT5 
#define IRQ_L  P1OUT &= ~BIT5

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

//Ϊ�˺ͱ�������ģ���ͻ��TX_Addr[]��ò�Ҫ��ͬ
uchar  TX_Addr[]={0x12,0x89,0x70,0x81,0x16};
//uchar  TX_Buffer[]={3,3,3,3};
uchar RX_Buffer[RX_DATA_WITDH];

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
 for(i=0;i<8;i++)                  //һ�ֽ�8λѭ��8��д��
 {
  if(byte&0x80)                    //����������λ��1
   MOSI_H;                         //��NRF24L01д1
  else                             //����д0
   MOSI_L;
  byte<<=1;                        //��һλ�Ƶ����λ
  SCK_H;                           //SCK���ߣ�д��һλ���ݣ�ͬʱ��ȡһλ����
  if(P1IN & MISO)
   byte|=0x01;
  SCK_L;                           //SCK����
 }
 return byte;                      //���ض�ȡһ�ֽ�
}

//SPIд�Ĵ���һ�ֽں���
//reg:�Ĵ�����ַ
//value:һ�ֽڣ�ֵ��
uchar SPI_W_Reg(uchar reg,uchar value)
{
 uchar status;                     //����״̬
 CSN_L;                            //SPIƬѡ
 status=SPI_RW(reg);               //д��Ĵ�����ַ��ͬʱ��ȡ״̬
 SPI_RW(value);                    //д��һ�ֽ�
 CSN_H;                            //
 return status;                    //����״̬
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
void port_init(void)               //�˿ڳ�ʼ��
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI;
  P1DIR &= ~MISO;
  
  P2DIR |= HT1621_DI+HT1621_CLK+HT1621_CS; //����HT1621���
  
  P2DIR |= PWM_24;                   // P2.2������Χģ�������������PWM2
  P2SEL |= PWM_24;
}

void int_Time_A(void)              //��ʱ��A��ʼ��
{
  BCSCTL1 |=DIVA_2;                //����ϵͳʱ�Ӳ���Ƶ
  TA1CTL = TASSEL_1 + MC_1;        // AMCLK, up mode
  TA1CCR0 =163;        // PWM Period T=32768/4/164=50Hz ��20ms���ң������
  TA1CCTL2 = OUTMOD_7;             //OUTMOD_7
  TA1CCR2 = 7;                     // ռ�ձ�D=7/163=0.42ms
}
void init_devices(void)
{
   port_init();                     //IO������
   int_Clock();                     //ʱ������
   int_WDT();                       //���Ź�����
   int_Time_A();
}
void main(void)
{
   init_devices(); 
   Ht1621_Init();                   //�ϵ��ʼ��LCD
   Delay_ms(5);                     //��ʱһ��ʱ��
   Ht1621WrAllData(0,Ht1621Tab_0,16); //����
   nRF24L01_Init();                 //NRF24L01��ʼ��
//   _EINT();                        //�����ж�
   while(1)
   {
     nRF24L01_Set_RX_Mode();
     Delay_ms(5);
      if(nRF24L01_RX_Data())          //��������
       {
          R1=RX_Buffer[0]%10;          //
          R2=RX_Buffer[0]/10;
          LCD_Display();  
          TA1CCR2=RX_Buffer[0];
        } 

   }
}