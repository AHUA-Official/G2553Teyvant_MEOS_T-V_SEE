/********************************************************************
//DM430-L型最小系统板4位独立按键测试程序，采用查询模式，键值显示到12864液晶
//按四个按键S1到S4，分别对应1,2,3,4
//12864使用带字库ST7920驱动芯片型号，欣创官方液晶
//显示打开了游标反白显示
//调试环境：EW430 V5.30
//作者：www.avrgcc.com
//时间：2014.03.01
********************************************************************/
#include <msp430x14x.h>
#include "Config.h"
#include "12864.c"

uchar key;

//*************************************************************************
//	初始化按键IO口子程序
//*************************************************************************
void KeyPort_init()
{

  P1SEL = 0x00;                   //P1普通IO功能
  P1DIR = 0xF0;                   //P10~P13输入模式，外部电路已接上拉电阻
  P6SEL = 0x00;                   //P6口普通IO功能
  P6DIR = 0xFF;                   //P6口输出模式
}

//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************

uchar Key_Scan(void) 
{
  uchar key_check;
  uchar key_checkin;
  key_checkin=KeyPort;          	//读取IO口状态，判断是否有键按下
  key_checkin&= 0x0F;          		//读取IO口状态，判断是否有键按下
  if(key_checkin!=0x0F)            	//IO口值发生变化则表示有键按下
    {
      delay_ms(40);                  	//键盘消抖，延时20MS
      key_checkin=KeyPort;
      if(key_checkin!=0x1F)
        {  
          key_check=KeyPort;
          switch (key_check & 0x0F)
            {
              case 0x0E:key=1;break;
              case 0x0D:key=2;break;
              case 0x0B:key=3;break;
              case 0x07:key=4;break;
            }
          
        }
      
   }
  else
   {
     key=0xFF;        
   }

  return key;
} 

//***********************************************************************
//     键值显示程序
//***********************************************************************
void LCD_DisplayKey(uchar keynum)
{    
  LCD_write_char(0x06,1,0x30+keynum);
}

//*************************************************************************
//		主程序
//*************************************************************************

void main(void)
{     
  WDT_Init();                         //看门狗设置
  Clock_Init();                       //系统时钟设置
  Port_init();                        //系统初始化，设置IO口属性
  delay_ms(100);                      //延时100ms
  LCD_init();                         //液晶参数初始化设置
  LCD_clear();                        //清屏
  LCD_Desk( );
  KeyPort_init();                     //按键IO口初始化，设置IO口属性
  delay_ms(100);                      //延时100ms
  while(1)
    { 
      Key_Scan();                     //键盘扫描，看是否有按键按下
      if(key!=0xff)                   //如果有按键按下，则显示该按键键值1～4
        {
	  {
            switch(key)
              {
	        case 1: LCD_DisplayKey(1);delay_ms(4);break;     //给不同的键赋键值，键值1，亮2个LED灯
                case 2: LCD_DisplayKey(2);delay_ms(4);break;     //给不同的键赋键值，键值2，亮2个LED灯
                case 3: LCD_DisplayKey(3);delay_ms(4);break;     //给不同的键赋键值，键值3，亮2个LED灯
                case 4: LCD_DisplayKey(4);delay_ms(4);break;     //给不同的键赋键值，键值4，亮2个LED灯
              }

          }
	
       }
     else
      {
        //LED=key;              	//没有按键的时候显示上次的键值
      }
    }

}

