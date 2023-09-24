
//��ʱ������IAR�Դ�������ʹ�õ�
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long


//4����������������P10~P13
#define KeyPort         P1IN                              //�������̽���P10~P13


//����/12864Һ��/1602Һ�������ݿڣ���Һ������
#define DataDIR         P2DIR                     //���ݿڷ���
#define DataPort        P2OUT                     //P4��Ϊ���ݿ�

//12864/1602Һ�����ƹܽ�
#define RS_CLR	        P6OUT &= ~BIT3           //RS�õ�
#define RS_SET	        P6OUT |=  BIT3           //RS�ø�

#define RW_CLR	        P6OUT &= ~BIT4           //RW�õ�
#define RW_SET	        P6OUT |=  BIT4           //RW�ø�

#define EN_CLR	        P6OUT &= ~BIT5          //E�õ�
#define EN_SET	        P6OUT |=  BIT5           //E�ø�

#define PSB_CLR	        P6OUT &= ~BIT6            //PSB�õͣ����ڷ�ʽ
#define PSB_SET	        P6OUT |=  BIT6            //PSB�øߣ����ڷ�ʽ

//#define RST_CLR	        P6OUT &= ~BIT7            //RST�õ�
//#define RST_SET	        P6OUT |= BIT7             //RST�ø�

//12864Ӧ��ָ�
#define CLEAR_SCREEN	0x01		          //����ָ�������ACֵΪ00H
#define AC_INIT		0x02		          //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD	0x06		          //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE	0x30		          //����ģʽ��8λ����ָ�
#define DISPLAY_ON	0x0f		          //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF	0x08		          //��ʾ��
#define CURSE_DIR	0x14		          //�α������ƶ�:AC=AC+1
#define SET_CG_AC	0x40		          //����AC����ΧΪ��00H~3FH
#define SET_DD_AC	0x80                      //����DDRAM AC
#define FUN_MODEK	0x36		          //����ģʽ��8λ��չָ�

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ⲿ8M����
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ڲ�RC����
//***********************************************************************
void Clock_Init_Inc()
{
  //uchar i;
  
 // DCOCTL = DCO0 + DCO1 + DCO2;              // Max DCO
 // BCSCTL1 = RSEL0 + RSEL1 + RSEL2;          // XT2on, max RSEL
  
  DCOCTL = 0x60 + 0x00;                       //DCOԼ3MHZ��3030KHZ
  BCSCTL1 = DIVA_0 + 0x07;
  BCSCTL2 = SELM_2 + DIVM_0 + SELS + DIVS_0;
}

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ⲿ32.768K����
//***********************************************************************
void Clock_Init_Ex32768()
{
  uchar i;

  BCSCTL2|=SELM1 + SELM0 + SELS;    //MCLKΪ32.768KHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}

//***********************************************************************
//               MSP430�ڲ����Ź���ʼ��
//***********************************************************************
void WDT_Init()
{
   WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
}

