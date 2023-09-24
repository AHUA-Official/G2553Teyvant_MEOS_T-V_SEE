//*************************************************************************
//			初始化IO口子程序
//*************************************************************************
void Port_init()
{

  P2SEL = 0x00;
  P2DIR = 0xFF;
  P6SEL = 0x00;
  P6DIR|= BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
  PSB_SET;		  //液晶并口方式
//  RST_SET;
}

//***********************************************************************
//	显示屏命令写入函数
//***********************************************************************
void LCD_write_com(unsigned char com) 
{	
  RS_CLR;
  RW_CLR;
  EN_SET;
  DataPort = com;
  delay_ms(5);
  EN_CLR;
}

//***********************************************************************
//	显示屏数据写入函数
//***********************************************************************
void LCD_write_data(unsigned char data) 
{
  RS_SET;
  RW_CLR;
  EN_SET;
  DataPort = data;
  delay_ms(5);
  EN_CLR;
}

//***********************************************************************
//	显示屏清空显示
//***********************************************************************

void LCD_clear(void) 
{
  LCD_write_com(0x01);
  delay_ms(5);
}

//***********************************************************************
//函数名称：DisplayCgrom(uchar hz)显示CGROM里的汉字
//***********************************************************************
void DisplayCgrom(uchar addr,uchar *hz)
{
  LCD_write_com(addr);
  delay_ms(5);
  while(*hz != '\0')  
  {
    LCD_write_data(*hz);
    hz++;
    delay_ms(5);
  }

} 

//***********************************************************************
//	显示屏单字符写入函数
//***********************************************************************
void LCD_write_char(unsigned char x,unsigned char y,unsigned char data) 
{
	
    if (y == 0) 
    {
    	LCD_write_com(0x80 + x);        //第一行显示
    }
    if(y == 1) 
    {
    	LCD_write_com(0x90 + x);        //第二行显示
    }
    if (y == 2) 
    {
    	LCD_write_com(0x88 + x);        //第三行显示
    }
    if(y == 3) 
    {
    	LCD_write_com(0x98 + x);        //第四行显示
    }
    delay_ms(1);
    LCD_write_data(data);
    delay_ms(1);
}

//****************************************************************
//函数名称：Display()显示测试结果
//****************************************************************
void Display(void)
{ 
  LCD_clear();
  DisplayCgrom(0x80,"欣世纪电子欢迎你");
  DisplayCgrom(0x88,"旺:jingyehanxing");
  DisplayCgrom(0x90,"www.avrgcc.com  ");
  DisplayCgrom(0x98,"电话057487470625");
  delay_ms(250);
}

//***********************************************************************
//	显示屏初始化函数
//***********************************************************************
void LCD_init(void) 
{
  LCD_write_com(FUN_MODE);			//显示模式设置
  delay_ms(5);
  LCD_write_com(FUN_MODE);			//显示模式设置
  delay_ms(5);
  LCD_write_com(CURSE_DIR);			//显示模式设置
  delay_ms(5);
  LCD_write_com(DISPLAY_ON);			//显示开
  delay_ms(5);
  LCD_write_com(CLEAR_SCREEN);			//清屏
  delay_ms(5);
}

//***********************************************************************
//      液晶显示界面初始化
//***********************************************************************
void LCD_Desk(void)
{    
  LCD_clear();
  DisplayCgrom(0x80,"欣世纪电子欢迎你");
  DisplayCgrom(0x90,"当前键值是：");
  delay_ms(250);
}

