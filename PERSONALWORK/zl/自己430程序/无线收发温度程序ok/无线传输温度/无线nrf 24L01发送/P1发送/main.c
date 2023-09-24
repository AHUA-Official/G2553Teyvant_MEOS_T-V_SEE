/****************************************************

*******************Ӳ������**************************


             DS18B20������
        DS18B20 - (1)-----------------------DS18B20��������9013-C��3��
        DQ        (2)-----------------------P1.6  //DS18B20�Ŀ����źŽ�
        DS18B20 + (3)-----------------------VCC 

           DS18B20��������9013������
        DS18B20��������9013-B(2)------------P2.1   //DS18B20�Ŀ��Ƶ�Դ
        DS18B20��������9013-B(1)------------GND 

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
        nRF 2401��������9013(2)-------------P2.0 
        nRF 2401��������9013(1)-------------GND

                 ����������
        ˯��sleep��LPM4��-------------------P2.3
        ���ѣ�LPM4_EXIT) -------------------P2.4 

******************************************************/
#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

volatile uchar sleep=0;//˯�߱�־λ

#define NPN_9013   BIT0           //��������Ϊ���ؿ��Ʒ����Դ
#define NPN_9013_H P2OUT |= BIT0  //�������ø�ʱ�������Դ��
#define NPN_9013_L P2OUT &= ~BIT0 //�������õ�ʱ�������Դ�ر�

#define DS18B20_NPN_9013   BIT1           //��������Ϊ���ؿ���DS18B20��Դ
#define DS18B20_NPN_9013_H P2OUT |= BIT1  //�������ø�ʱ��DS18B20��Դ��
#define DS18B20_NPN_9013_L P2OUT &= ~BIT1 //�������õ�ʱ��DS18B20��Դ�ر�


void Delay_us(uint us);



//������DS18B20��ʾ�궨��

#define DS18B20      BIT6
#define DS18B20_H    P1OUT |= BIT6
#define DS18B20_L    P1OUT &= ~BIT6

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




#define TX_ADDR_WITDH 5//���͵�ַ�������Ϊ5���ֽ�
#define RX_ADDR_WITDH 5//���յ�ַ�������Ϊ5���ֽ�
#define TX_DATA_WITDH 8//��������λ��
#define RX_DATA_WITDH 8//��������λ��


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

#define IRQ    BIT5          
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

//uchar  TX_Buffer[]={3,2,1,0};//����������
uchar RX_Buffer[RX_DATA_WITDH];

void port_init(void)               //�˿ڳ�ʼ��
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI+LED+DS18B20;
  P1OUT &= ~LED;
  P1DIR &= ~MISO;
  
  P2DIR |=NPN_9013+DS18B20_NPN_9013;
  NPN_9013_H;          //�������ø�ʱ�������Դ��
  DS18B20_NPN_9013_H;   //�������ø�ʱ��DS18B20��Դ��
  
  P2OUT |=BIT3+BIT4;   //P2.3-4����������
  P2IE  |=BIT3+BIT4;   //P2.3-4�ж�ʹ��
  P2IES |=BIT3+BIT4;   //P2.3-4�ж��½��ش����ж�
  P2REN |=BIT3+BIT4;   //P2.3-4��������ʹ��
  P2IFG &=~(BIT3+BIT4);//P2.3-4�жϱ�־λ����
  
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
  __delay_cycles(8);      
}
void Delay_ms(uint ms)           // ��ʱms  ǰ����f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
}


//nRF24L01��ʼ��
void nRF24L01_Init(void)
{
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
 uchar reg_value;
 CSN_L;//SPIƬѡ
 SPI_RW(reg);//д���ַ
 reg_value=SPI_RW(0);//��ȡ�Ĵ�����ֵ
 CSN_H;
 return reg_value;//���ض�ȡ��ֵ
}

//SPI��ȡRXFIFO�Ĵ�������
//reg:�Ĵ�����ַ
//Dat_Buffer:�������ȡ������
//DLen:���ݳ���
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen)
{
 uchar status,i;
 CSN_L;//SPIƬѡ
 status=SPI_RW(reg);//д��Ĵ�����ַ��ͬʱ״̬
 for(i=0;i<Dlen;i++)
 {
  Dat_Buffer[i]=SPI_RW(0);//�洢����
 }
 CSN_H;
 return status;
}

//SPI��TXFIFO�Ĵ���д������
//reg:д��Ĵ�����ַ
//TX_Dat_Buffer:�����Ҫ���͵�����
//Dlen:���ݳ���
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen)
{
 uchar status,i;
 CSN_L;//SPIƬѡ������ʱ��
 status=SPI_RW(reg);
 for(i=0;i<Dlen;i++)
 {
  SPI_RW(TX_Dat_Buffer[i]);//��������
 }
 CSN_H;
 return status; 
}

//���÷���ģʽ
void nRF24L01_Set_TX_Mode(uchar *TX_Data)
{
 CE_L;//������д�Ĵ���֮ǰһ��Ҫ�������ģʽ�����ģʽ��
 SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);//д�Ĵ���ָ��+���սڵ��ַ+��ַ���
 SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,TX_Addr,TX_ADDR_WITDH);//Ϊ�˽����豸Ӧ���źţ����յ�0��ַ�뷢�͵�ַ��ͬ
 SPI_W_DBuffer(W_TX_PLOAD,TX_Data,TX_DATA_WITDH);//д��Ч���ݵ�ַ+��Ч����+��Ч���ݿ��
 SPI_W_Reg(W_REGISTER+EN_AA,0x01);//����ͨ��0�Զ�Ӧ��
 SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);//ʹ�ܽ���ͨ��0
 SPI_W_Reg(W_REGISTER+SETUP_RETR,0x0a);//�Զ��ط���ʱ250US+86US���ط�10��
 SPI_W_Reg(W_REGISTER+RF_CH,0);//2.4GHZ
 SPI_W_Reg(W_REGISTER+RF_SETUP,0x07);//1Mbps����,���书��:0DBM,�������Ŵ�������
 SPI_W_Reg(W_REGISTER+CONFIG,0x0e);//����ģʽ,�ϵ�,16λCRCУ��,CRCʹ��
 CE_H;//��������
 Delay_ms(5);//CE�ߵ�ƽ����ʱ������10US����
}

//���Ӧ���ź�
uchar Check_Ack(void)
{
 sta=SPI_R_byte(R_REGISTER+STATUS);//��ȡ�Ĵ�״̬
 if(TX_DS||MAX_RT)//���TX_DS��MAX_RTΪ1,������жϺ����TX_FIFO�Ĵ�����ֵ
 {
  SPI_W_Reg(W_REGISTER+STATUS,0xff);
  CSN_L;
  SPI_RW(FLUSH_TX);       //���û����һ��ֻ�ܷ�һ�����ݣ����Ҫע��
  CSN_H;
  return 0;
 }
 else
  return 1;
}

void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;          //watchdog
}

void stop_IO(void) //��������֮ǰ������IO�ڣ����������������⣬ȫ��������ø�
{
  P1DIR =0XFF;
  P1OUT =0XFF;
  P2DIR |= 0XE7;   //����������Ϊ���룬����Ӧ�ж�
  P2OUT |= 0XE6;
  NPN_9013_L;      //���������õͣ��������Դ�ر�
  DS18B20_NPN_9013_L;
}

void main(void)
{
   uint t;
   uchar i;
   uchar TX_Buffer[]={1,1,1,1};//����������
   init_devices();
   Delay_ms(5);
   nRF24L01_Init();                  //NRF24L01��ʼ��
   _EINT();                          //�����ж�
   while(1)
 {
    t=Read_temp();                    //��ȡ�¶�ֵ
    t=t*6.25;
    TX_Buffer[3]=t/1000;              //
    TX_Buffer[2]=t%1000/100;
    TX_Buffer[1]=t%1000%100/10;
    TX_Buffer[0]=t%10;
   
//  for(i=0;i<TX_DATA_WITDH-1;i++)  //����7������
  for(i=0;i<TX_DATA_WITDH-7;i++)    //����һ������  
   {
         nRF24L01_Set_TX_Mode(&TX_Buffer[i]);//��������
         while(Check_Ack());//�ȴ��������
           
        //LED_L;            //��������ǲ��Ƿ������
        //Delay_ms(2);
        //LED_H;            //��������ǲ��Ƿ�����ɣ������LED��˸һ��
        //Delay_ms(2);
       
   }
 
  if(sleep)
   {
         stop_IO();        //��������֮ǰ������IO�ڣ����������������⣬ȫ��������ø�
         LPM4;
         
         init_devices();   //�������ã�����ʼ��
         Delay_ms(5);
         nRF24L01_Init();  //NRF24L01��ʼ��
         sleep=0;          //��˯�߱�־
   }
 }   
}

//P2�жϷ������

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{ 
    Delay_ms(10);            //����10ms
    if((P2IN & BIT3)==0)
    {
      sleep=1;               //��˯�߱�־λ��λ��׼������
    }
    if((P2IN & BIT4)==0)
    {
      sleep=0; 
      LPM4_EXIT;             //�˳�˯��LPM3
    }
    P2IFG =0;                //P2�жϱ�־λ����
}  
