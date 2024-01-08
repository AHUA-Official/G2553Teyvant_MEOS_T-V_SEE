#include"msp430g2553.h"
#define uchar unsigned char
#define uint unsigned int
/*
#define CS P2.2;
#define SID P2.1;
#define CLCLK P2.0;
*/
#define CS_1  P2OUT|=0x04;//ע��λ�Ĳ���
#define CS_0  P2OUT&=~0x04;
#define SID_1 P2OUT|=0x02;
#define SID_0 P2OUT&=~0x02;
#define CLK_1 P2OUT|=0x01;
#define CLK_0 P2OUT&=~0x01;
#define nop _NOP();
void write_com(uchar x);
void write_data(uchar x);
void write_string(uchar x,uchar y,uchar *s);

void delay(uchar z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=20;y>0;y--);
}

void W_8bits(uchar W_bits)
{
  uchar i,temp;
  for(i=0;i<8;i++)
  {
    temp=W_bits;
    temp<<=i;
    if((temp&0x80)==0)
    {
      SID_0;
      nop;
      CLK_1;
      nop;
      nop;
      CLK_0;
      nop;
      SID_0;
    }
    else
    {
      SID_1;
      nop;
      CLK_1;
      nop;
      nop;
      CLK_0;
      nop;
      SID_0;
    }
  }
}
/*
  rw=1  ���ʾ����
  rw=0  ���ʾ����
  rs=1  ���ʾд����
  rs=0  ���ʾд����
  W_byte(0,0,x)д���
  W_byte(0,1,y)д���ݣ�
*/
void W_byte(uchar rw,uchar rs,uchar W_data)
{
  uchar H_data,L_data,s_id=0xf8;
  if(rw==0)
  {
    s_id&=~0x04;
  }
  else
  {
    s_id|=0x04;
  }
  if(rs==0)
  {
    s_id&=~0x02;
  }
  else
    {
      s_id|=0x02;
    }
  H_data=W_data;
  H_data&=0xf0;
  L_data=W_data;
  L_data&=0x0f;
  L_data<<=4;
  CS_1;
  W_8bits(s_id);
  W_8bits(H_data);
  W_8bits(L_data);
  CS_0;
}

void lcdinit()
{
  P2DIR|=0x07;//������������ָ������о��þã�����
  delay(10);
  W_byte(0,0,0x38);
  delay(5);
  W_byte(0,0,0x38);
  delay(5); 
  W_byte(0,0,0x0c);
  delay(5);
  W_byte(0,0,0x01);
  delay(5);
  W_byte(0,0,0x02);
  delay(5);
  W_byte(0,0,0x80);
  delay(5);
}

void W_str(uchar x,uchar y,uchar *s)
{
  switch(y)
  {
  case 1:W_byte(0,0,0x80+x);break;//0x80�ǵ�һ����ʼλ��
  case 2:W_byte(0,0,0x90+x);break;//0x90�ǵڶ�����ʼλ��
  case 3:W_byte(0,0,0x88+x);break;//0x88�ǵ�������ʼλ��
  case 4:W_byte(0,0,0x98+x);break;//0x98�ǵ�������ʼλ��
  default:W_byte(0,0,0x80+x);//���򶼵��ɵ�һ�д���
  }
  while(*s)
  {
    W_byte(0,1,*s);
    s++;
  }
}

/*====================================================================
����ĺ���ֻ��Ϊ����д���㣬ʵ�ʹ���һ��
write_com(uchar x)==W_byte(0,0,uchar x)
write_data(uchar x)==W_byte(0,1,uchar x)
write_string(uchar x,uchar y,uchar *s)==W_str(uchar x,uchar y,uchar *s);
=======================================================================*/
void write_com(uchar x)
{
  W_byte(0,0,x);
}
void write_data(uchar x)
{
  W_byte(0,1,x);
}
void write_string(uchar x,uchar y,uchar *s)
{
  switch(y)
  {
  case 1:W_byte(0,0,0x80+x);break;//0x80�ǵ�һ����ʼλ��
  case 2:W_byte(0,0,0x90+x);break;//0x90�ǵڶ�����ʼλ��
  case 3:W_byte(0,0,0x88+x);break;//0x88�ǵ�������ʼλ��
  case 4:W_byte(0,0,0x98+x);break;//0x98�ǵ�������ʼλ��
  default:W_byte(0,0,0x80+x);//���򶼵��ɵ�һ�д���
  }
  while(*s)
  {
    W_byte(0,1,*s);
    s++;
  }
}
/*===================OVER=============================*/