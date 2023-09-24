
//（ 1）初始化串口 
void UART_init(void)
{
 UCA0CTL1 |=UCSWRST;
 UCA0CTL1 |= UCSSEL_2; //SMCLK
 UCA0BR0 = 0x68; //32.768/9600=
 UCA0BR1 = 0x00; // 1000kHz/9600 = 104.166 =0X68 波特率9600
 UCA0MCTL = UCBRS_2; // Modulation UCBRSx = 1
 UCA0CTL0 &=~UCPEN;
 UCA0CTL0 &=~UCSPB;
 UCA0CTL0 &=~UC7BIT;
 UCA0CTL1 &=~UCSWRST;
 P1SEL|=BIT1+BIT2;
 P1SEL2|=BIT1+BIT2;
 //P1DIR|=BIT2; 第二功能无需配置？？
 IE2 |= UCA0RXIE+UCA0TXIE;
 unsigned int j;
119
 for(j=0;j<2000;j++);
}
//（ 2） 串口发送函数
/****************************************************************
 宏定义
****************************************************************/
unsigned char RX_BUFF[RXBUF_SIZE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //接收缓冲
区
unsigned int UART_InLen=16; //输入数据长度
unsigned int RX_IndexR = 0; //接收FIFO的读指针
unsigned int RX_IndexW = 0;  //接收FIFO的写指针
unsigned char TX_BUFF[TXBUF_SIZE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ; //发送缓冲区
unsigned int UART_OutLen =16; //发送FIFO内待发送的字节数
unsigned int TX_IndexR = 0; //发送FIFO的读指针
unsigned int TX_IndexW = 0; //发送FIFO的写指针
uint8 ch=0;
/****************************************************************
 串口发送一字节函数， 查询方式
****************************************************************/
void UART_sent(uint8 Chr)
{
 IFG2&=~UCA0TXIFG;
UCA0TXBUF=Chr;
 while ((IFG2&UCA0TXIFG) ==0); // USCI_A0 TX buffer ready?
}
/****************************************************************
 串口发送一帧数据函数， 中断方式
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
* 名  称： USCIxTX_ISR()
* 功 能：串口发送中断，每发完1字节会发生一次中断
*****************************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR (void)
{
 if (UART_OutLen > 0)
 {
 UART_OutLen--;
 UCA0TXBUF = TX_BUFF[TX_IndexR];
 while (!(IFG2&UCA0TXIFG)); //查看上一字节是否发送完毕
 if (++TX_IndexR >= TXBUF_SIZE)
 {
 TX_IndexR = 0;
 }
 }
 else IE2 &=~UCA0TXIE;
} 
//（ 3）串口接收函数
/*****************************************************************
 串口接收一字节函数，查询方式，此处虽定义但并未使用，不保证其正确性
*****************************************************************/
uint8 UART_receive(void)
{
 while ((IFG2&UCA0RXIFG)==0); // 收到一字节?
121
 IFG2&=~UCA0RXIFG;
 return(UCA0RXBUF);
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
 UART_InLen++;
 RX_BUFF[RX_IndexW] = UCA0RXBUF; //保存在缓冲区中
 if (++RX_IndexW >= RXBUF_SIZE)
 {
 RX_IndexW = 0;
 }
}