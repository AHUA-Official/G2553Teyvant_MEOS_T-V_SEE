/*���棺����G2 LaunchPad�ϵ�LED2ʹ����P1.6��I2C���ţ���
 *���������漰��I2C��ʵ�鶼�����P1.6���߰γ�,���������̣�
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"
#include"HT1621.h"
#include"LCD_128.h"

#define myMCLK	16000000
void LCD_Display_Pass();
void main(void)
{
	unsigned char i=0,j=0;
	WDTCTL = WDTPW + WDTHOLD;	//�ع�
	P1DIR = 0;
	P2DIR = 0;

        BCSCTL1 = CALBC1_16MHZ; /* Set DCO to16MHz */
        DCOCTL = CALDCO_16MHZ;
        TCA6416A_Init();	// ��ʼ��IO��չ��
       //----��ʾ��ʼ���ɹ�----
    	PinOUT(0,1);           // ָ��0�Źܽ����Ϊ0
    	PinOUT(1,0);           // ָ��1�Źܽ����Ϊ0
    	PinOUT(2,1);           // ָ��0�Źܽ����Ϊ0
    	PinOUT(3,0);           // ָ��1�Źܽ����Ϊ0
    	PinOUT(4,1);           // ָ��0�Źܽ����Ϊ0
    	PinOUT(5,0);           // ָ��1�Źܽ����Ϊ0
    	PinOUT(6,1);           // ָ��0�Źܽ����Ϊ0
    	PinOUT(7,0);           // ָ��1�Źܽ����Ϊ0
    	HT1621_init();			// ��ʼ��lcd_128

	while(1)
	{
		//----��0����127�����ε���------
		for(i=0;i<=127;i++)
		{
			LCD_DisplaySeg(i);
			HT1621_Reflash(LCD_Buffer);
		}
		 __delay_cycles(myMCLK);		// �ʵ���ʱԼΪ1s
		 //----��127����0������Ϩ��
		for(i=128;i>0;i--)
		{
			LCD_ClearSeg(i-1);
			HT1621_Reflash(LCD_Buffer);
		}
		 __delay_cycles(myMCLK);		// �ʵ���ʱԼΪ1s
		//-----�����������ʾһ�£���0�л���9----

		for(i=0;i<=9;i++)
		{
			for(j=1;j<=10;j++)
			{
				LCD_DisplayDigit(i,j);
			}
			HT1621_Reflash(LCD_Buffer);
			 __delay_cycles(myMCLK/2);		// �ʵ���ʱԼΪ1/2s
		}
		LCD_Clear();
		//-----���������ʾ-12456----
		LCD_DisplayNum(-123456);
		HT1621_Reflash(LCD_Buffer);
		 __delay_cycles(myMCLK);			// �ʵ���ʱԼΪ1s
		LCD_Clear();
		//----���Խ�������ʾPASS----
		LCD_Display_Pass();
		 __delay_cycles(2*myMCLK);		// �ʵ���ʱԼΪ2s
		 _bis_SR_register(LPM3_bits);	//�Լ���ϣ�����
	}
}

/******************************************************************************************************
 * ��    �ƣ�LCD_Display_Pass()
 * ��    �ܣ���ʾPASS��ĸ�ĺ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵    ����ʹ������ʾ���֣���ɾ���ض��εķ���ʵ����ĸ��ʾ
 * ��    ������
 ******************************************************************************************************/
void LCD_Display_Pass()
{
	LCD_DisplayNum(8855);
	LCD_ClearSeg(_LCD_3C);
	LCD_ClearSeg(_LCD_3D);
	LCD_ClearSeg(_LCD_4D);
	//----��ʾlogo----
	LCD_DisplaySeg(_LCD_TI_logo);
	LCD_DisplaySeg(_LCD_QDU_logo);
	HT1621_Reflash(LCD_Buffer);
}

