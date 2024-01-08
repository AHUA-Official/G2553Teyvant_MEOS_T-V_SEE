
#include <msp430x14x.h>
#include  "keypad.h"
//#include  "keypad.C"


//引用外部变量的声明
extern unsigned char key_Pressed;      
extern unsigned char key_val;         
extern unsigned char key_Flag; 

//定义全局变量
uchar  DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗 
    //WDTCTL = WDT_ADLY_1_9;
    //IE1 |= WDTIE;
    
    P3DIR |= 0XFF;
    P3OUT = 0XFF;
    
   
    Init_Keypad();                            //初始化键盘端口
    while(1)
    {
        Key_Event();
    
        if(key_Flag == 1)
        {
            key_Flag = 0;

        }
    }
}
