
//�� 1����ʼ������ 
void UART_init(void)
{
 UCA0CTL1 |=UCSWRST;
 UCA0CTL1 |= UCSSEL_2; //SMCLK
 UCA0BR0 = 0x68; //32.768/9600=
 UCA0BR1 = 0x00; // 1000kHz/9600 = 104.166 =0X68 ������9600
 UCA0MCTL = UCBRS_2; // Modulation UCBRSx = 1
 UCA0CTL0 &=~UCPEN;
 UCA0CTL0 &=~UCSPB;
 UCA0CTL0 &=~UC7BIT;
 UCA0CTL1 &=~UCSWRST;
 P1SEL|=BIT1+BIT2;
 P1SEL2|=BIT1+BIT2;
 //P1DIR|=BIT2; �ڶ������������ã���
 IE2 |= UCA0RXIE+UCA0TXIE;
 unsigned int j;
119
 for(j=0;j<2000;j++);
}
//�� 2�� ���ڷ��ͺ���
/****************************************************************
 �궨��
****************************************************************/
unsigned char RX_BUFF[RXBUF_SIZE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //���ջ���
��
unsigned int UART_InLen=16; //�������ݳ���
unsigned int RX_IndexR = 0; //����FIFO�Ķ�ָ��
unsigned int RX_IndexW = 0;  //����FIFO��дָ��
unsigned char TX_BUFF[TXBUF_SIZE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ; //���ͻ�����
unsigned int UART_OutLen =16; //����FIFO�ڴ����͵��ֽ���
unsigned int TX_IndexR = 0; //����FIFO�Ķ�ָ��
unsigned int TX_IndexW = 0; //����FIFO��дָ��
uint8 ch=0;
/****************************************************************
 ���ڷ���һ�ֽں����� ��ѯ��ʽ
****************************************************************/
void UART_sent(uint8 Chr)
{
 IFG2&=~UCA0TXIFG;
UCA0TXBUF=Chr;
 while ((IFG2&UCA0TXIFG) ==0); // USCI_A0 TX buffer ready?
}
/****************************************************************
 ���ڷ���һ֡���ݺ����� �жϷ�ʽ
****************************************************************/
char UART0_PutFrame(unsigned char *Ptr, unsigned int Lenth)
{
 int i;
 if(IE2&UCA0TXIE)
 {
 return(0);
 }
 if(Lenth>TXBUF_SIZE)
 {
 return(0);
120
 }
 for(i=0;i<Lenth;i++)
 {
 delay_us(100);
 TX_BUFF[i]=Ptr[i];
 }
 TX_IndexR=0;
 UART_OutLen=Lenth;
 IFG2|=UCA0TXIFG;
 IE2|=UCA0TXIE;
 return(1);
}
/*****************************************************************
* ��  �ƣ� USCIxTX_ISR()
* �� �ܣ����ڷ����жϣ�ÿ����1�ֽڻᷢ��һ���ж�
*****************************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR (void)
{
 if (UART_OutLen > 0)
 {
 UART_OutLen--;
 UCA0TXBUF = TX_BUFF[TX_IndexR];
 while (!(IFG2&UCA0TXIFG)); //�鿴��һ�ֽ��Ƿ������
 if (++TX_IndexR >= TXBUF_SIZE)
 {
 TX_IndexR = 0;
 }
 }
 else IE2 &=~UCA0TXIE;
} 
//�� 3�����ڽ��պ���
/*****************************************************************
 ���ڽ���һ�ֽں�������ѯ��ʽ���˴��䶨�嵫��δʹ�ã�����֤����ȷ��
*****************************************************************/
uint8 UART_receive(void)
{
 while ((IFG2&UCA0RXIFG)==0); // �յ�һ�ֽ�?
121
 IFG2&=~UCA0RXIFG;
 return(UCA0RXBUF);
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
 UART_InLen++;
 RX_BUFF[RX_IndexW] = UCA0RXBUF; //�����ڻ�������
 if (++RX_IndexW >= RXBUF_SIZE)
 {
 RX_IndexW = 0;
 }
}