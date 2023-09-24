#include  <msp430x14x.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

/***************全局变量***************/
uchar key_Pressed;      //按键是否被按下:1--是，0--否
uchar key_val;          //存放键值
uchar key_Flag;         //按键是否已放开：1--是，0--否
//设置键盘逻辑键值与程序计算键值的映射
uchar key_Map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

/*******************************************
函数名称：Init_Keypad
功    能：初始化扫描键盘的IO端口
参    数：无
返回值  ：无
********************************************/
void Init_Keypad(void)
{     
    P1DIR = 0xf0;       //P1.0~P1.3设置为输入状态, P1.4~P1.7设置为输出状态
    P1OUT |= 0xf0;      // P1.4~P1.7输出高电平
    key_Flag = 0;       
    key_Pressed = 0;   
    key_val = 0;
}
/*********************************************
* Check_Key(),检查按键，确认键值
*********************************************/
/*******************************************
函数名称：Check_Key
功    能：扫描键盘的IO端口，获得键值
参    数：无
返回值  ：无
********************************************/
void Check_Key(void)
{
    uchar row ,col,tmp1,tmp2;
    
    tmp1 = 0x80;
    for(row = 0;row < 4;row++)              //行扫描
    {
        P1OUT = 0xf0;                      //P1.4~P1.7输出全1
        P1OUT -= tmp1;                      //P1.4~p1.7输出四位中有一个为0
        tmp1 >>=1;                              
        if ((P1IN & 0x0f) < 0x0f)           //是否P1IN的P1.0~P1.3中有一位为0
        {                       
            tmp2 = 0x01;                         // tmp2用于检测出那一位为0  
            for(col = 0;col < 4;col++)              // 列检测 
            {          
                if((P1IN & tmp2) == 0x00)           // 是否是该列,等于0为是
                {          
                    key_val = key_Map[row * 4 + col];  // 获取键值
                    return;                         // 退出循环
                }
                tmp2 <<= 1;                        // tmp2右移1位 
            }
        }
    }      
}
/*******************************************
函数名称：delay
功    能：延时约15ms，完成消抖功能
参    数：无
返回值  ：无
********************************************/
void delay()
{
    uint tmp;
     
    for(tmp = 12000;tmp > 0;tmp--);
}
/*******************************************
函数名称：Key_Event
功    能：检测按键，并获取键值
参    数：无
返回值  ：无
********************************************/
void Key_Event(void)
{     
    uchar tmp;
     
    P1OUT &= 0x00;              // 设置P1OUT全为0，等待按键输入
    tmp = P1IN;                 // 获取 p1IN
    if ((key_Pressed == 0x00)&&((tmp & 0x0f) < 0x0f)) //如果有键按下
    {                     
        key_Pressed = 1;        // 如果有按键按下，设置key_Pressed标识
        delay();                //消除抖动
        Check_Key();            // 调用check_Key(),获取键值 
    }     
    else if ((key_Pressed == 1)&&((tmp & 0x0f) == 0x0f)) //如果按键已经释放
    {                     
        key_Pressed = 0;        // 清除key_Pressed标识
        key_Flag    = 1;        // 设置key_Flag标识
    }
    else 
    {
        _NOP();
    }     
}
