
#include  "DS18B20.h"

#define DQ1 P1OUT |= BIT6
#define DQ0 P1OUT &= ~BIT6
#define DQ_in   P1DIR &= ~BIT6
#define DQ_out  P1DIR |= BIT6
#define DQ_val  (P1IN & BIT6)

/*******************************************
函数名称：DelayNus
功    能：实现N个微秒的延时
参    数：n--延时长度
返回值  ：无
说明    ：定时器A的计数时钟是1MHz，CPU主频8MHz
          所以通过定时器延时能够得到极为精确的
          us级延时
********************************************/
void DelayNus(uint n)
{
    CCR0 = n;
    TACTL |= MC_1;           //增计数到CCR0
    while(!(TACTL & BIT0));   //等待
    TACTL &= ~MC_1;          //停止计数
    TACTL &= ~BIT0;          //清除中断标志
}
/*******************************************
函数名称：Init_18B20
功    能：对DS18B20进行复位操作
参    数：无
返回值  ：初始化状态标志：1--失败，0--成功
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
        Error = 1;          //初始化失败
    }
    else
    {
        Error = 0;          //初始化成功
    }
    DQ_out;
    DQ1;
    _EINT();
    
    DelayNus(400);
    
    return Error;
}
/*******************************************
函数名称：Write_18B20
功    能：向DS18B20写入一个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write_18B20(uchar wdata)
{
    uchar i;
    
    _DINT();
    for(i = 0; i < 8;i++)
    {
        DQ0;
        DelayNus(6);            //延时6us
        if(wdata & 0X01)    DQ1;
        else                DQ0;
        wdata >>= 1;
        DelayNus(50);           //延时50us
        DQ1;
        DelayNus(10);           //延时10us
    }
    _EINT();
}
/*******************************************
函数名称：Read_18B20
功    能：从DS18B20读取一个字节的数据
参    数：无
返回值  ：读出的一个字节数据
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
        DelayNus(6);            //延时6us
        DQ1;
        DelayNus(8);            //延时9us
        DQ_in;
        _NOP();
        if(DQ_val)   temp |= 0x80;
        DelayNus(45);           //延时45us
        DQ_out;
        DQ1;
        DelayNus(10);           //延时10us
    }
    _EINT();
    
    return  temp;
}

/*******************************************
函数名称：Skip
功    能：发送跳过读取产品ID号命令
参    数：无
返回值  ：无
********************************************/
void Skip(void)
{
    Write_18B20(0xcc);
}
/*******************************************
函数名称：Convert
功    能：发送温度转换命令
参    数：无
返回值  ：无
********************************************/
void Convert(void)
{
    Write_18B20(0x44);
}
/*******************************************
函数名称：Read_SP
功    能：发送读ScratchPad命令
参    数：无
返回值  ：无
********************************************/
void Read_SP(void)
{
    Write_18B20(0xbe);
}
/*******************************************
函数名称：ReadTemp
功    能：从DS18B20的ScratchPad读取温度转换结果
参    数：无
返回值  ：读取的温度数值
********************************************/
uint ReadTemp(void)
{
    uchar temp_low;
    uint  temp;
    
    temp_low = Read_18B20();      //读低位
    temp = Read_18B20();     //读高位
    temp = (temp<<8) | temp_low;
    
    return  temp;
}
/*******************************************
函数名称：ReadTemp
功    能：控制DS18B20完成一次温度转换
参    数：无
返回值  ：测量的温度数值
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
        DelayNus(60000); //延时800ms以上
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Read_SP();
    return ReadTemp();
}
