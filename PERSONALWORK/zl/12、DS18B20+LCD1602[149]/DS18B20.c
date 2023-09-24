
#include  "DS18B20.h"

#define DQ1 P1OUT |= BIT6
#define DQ0 P1OUT &= ~BIT6
#define DQ_in   P1DIR &= ~BIT6
#define DQ_out  P1DIR |= BIT6
#define DQ_val  (P1IN & BIT6)

/*******************************************
�������ƣ�DelayNus
��    �ܣ�ʵ��N��΢�����ʱ
��    ����n--��ʱ����
����ֵ  ����
˵��    ����ʱ��A�ļ���ʱ����1MHz��CPU��Ƶ8MHz
          ����ͨ����ʱ����ʱ�ܹ��õ���Ϊ��ȷ��
          us����ʱ
********************************************/
void DelayNus(uint n)
{
    CCR0 = n;
    TACTL |= MC_1;           //��������CCR0
    while(!(TACTL & BIT0));   //�ȴ�
    TACTL &= ~MC_1;          //ֹͣ����
    TACTL &= ~BIT0;          //����жϱ�־
}
/*******************************************
�������ƣ�Init_18B20
��    �ܣ���DS18B20���и�λ����
��    ������
����ֵ  ����ʼ��״̬��־��1--ʧ�ܣ�0--�ɹ�
********************************************/
uchar Init_18B20(void)
{
    uchar Error;
    
    DQ_out;
    _DINT();
    DQ0;
    DelayNus(500);
    DQ1;
    DelayNus(55);
    DQ_in;
    _NOP();
    if(DQ_val)      
    {
        Error = 1;          //��ʼ��ʧ��
    }
    else
    {
        Error = 0;          //��ʼ���ɹ�
    }
    DQ_out;
    DQ1;
    _EINT();
    
    DelayNus(400);
    
    return Error;
}
/*******************************************
�������ƣ�Write_18B20
��    �ܣ���DS18B20д��һ���ֽڵ�����
��    ����wdata--д�������
����ֵ  ����
********************************************/
void Write_18B20(uchar wdata)
{
    uchar i;
    
    _DINT();
    for(i = 0; i < 8;i++)
    {
        DQ0;
        DelayNus(6);            //��ʱ6us
        if(wdata & 0X01)    DQ1;
        else                DQ0;
        wdata >>= 1;
        DelayNus(50);           //��ʱ50us
        DQ1;
        DelayNus(10);           //��ʱ10us
    }
    _EINT();
}
/*******************************************
�������ƣ�Read_18B20
��    �ܣ���DS18B20��ȡһ���ֽڵ�����
��    ������
����ֵ  ��������һ���ֽ�����
********************************************/
uchar Read_18B20(void)
{
    uchar i;
    uchar temp = 0;
    
    _DINT();
    for(i = 0;i < 8;i++)
    {
        temp >>= 1;
        DQ0;
        DelayNus(6);            //��ʱ6us
        DQ1;
        DelayNus(8);            //��ʱ9us
        DQ_in;
        _NOP();
        if(DQ_val)   temp |= 0x80;
        DelayNus(45);           //��ʱ45us
        DQ_out;
        DQ1;
        DelayNus(10);           //��ʱ10us
    }
    _EINT();
    
    return  temp;
}

/*******************************************
�������ƣ�Skip
��    �ܣ�����������ȡ��ƷID������
��    ������
����ֵ  ����
********************************************/
void Skip(void)
{
    Write_18B20(0xcc);
}
/*******************************************
�������ƣ�Convert
��    �ܣ������¶�ת������
��    ������
����ֵ  ����
********************************************/
void Convert(void)
{
    Write_18B20(0x44);
}
/*******************************************
�������ƣ�Read_SP
��    �ܣ����Ͷ�ScratchPad����
��    ������
����ֵ  ����
********************************************/
void Read_SP(void)
{
    Write_18B20(0xbe);
}
/*******************************************
�������ƣ�ReadTemp
��    �ܣ���DS18B20��ScratchPad��ȡ�¶�ת�����
��    ������
����ֵ  ����ȡ���¶���ֵ
********************************************/
uint ReadTemp(void)
{
    uchar temp_low;
    uint  temp;
    
    temp_low = Read_18B20();      //����λ
    temp = Read_18B20();     //����λ
    temp = (temp<<8) | temp_low;
    
    return  temp;
}
/*******************************************
�������ƣ�ReadTemp
��    �ܣ�����DS18B20���һ���¶�ת��
��    ������
����ֵ  ���������¶���ֵ
********************************************/
uint Do1Convert(void)
{
    uchar i; 
    
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Convert();
    for(i = 20;i > 0;i--)  
        DelayNus(60000); //��ʱ800ms����
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Read_SP();
    return ReadTemp();
}
