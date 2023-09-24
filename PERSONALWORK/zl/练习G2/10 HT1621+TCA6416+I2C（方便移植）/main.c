

/*���棺����G2 LaunchPad�ϵ�LED2ʹ����P1.6��I2C���ţ���
 *���������漰��I2C��ʵ�鶼�����P1.6���߰γ�,���������̣�
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"
#include"HT1621.h"
#include"LCD_128.h"

#define myMCLK	16000000


void main(void)
{
	unsigned char i=0;
	WDTCTL = WDTPW + WDTHOLD;	//�ع�
	P1DIR = 0;
	P2DIR = 0;

        BCSCTL1 = CALBC1_16MHZ; /* Set DCO to16MHz */
        DCOCTL = CALDCO_16MHZ;
        TCA6416A_Init();	// ��ʼ��IO��չ��
       //----�ر�LED��----
    	PinOUT(0,1);           // ָ��1�Źܽ����Ϊ1
    	PinOUT(2,1);           // ָ��0�Źܽ����Ϊ1
    	PinOUT(3,1);           // ָ��1�Źܽ����Ϊ1
    	PinOUT(4,1);           // ָ��0�Źܽ����Ϊ1
    	PinOUT(5,1);           // ָ��1�Źܽ����Ϊ1
    	PinOUT(6,1);           // ָ��0�Źܽ����Ϊ1
    	PinOUT(7,1);           // ָ��1�Źܽ����Ϊ1
    	HT1621_init();	// ��ʼ��lcd_128
        LCD_Clear();    // LCD128����
	while(1)
	{
		//----��0����127�����ε���------
		for(i=0;i<=127;i++)
		{
			LCD_DisplaySeg(i);   //����һ��
			HT1621_Reflash(LCD_Buffer);
                        __delay_cycles(myMCLK);	
		}
		// __delay_cycles(myMCLK);		// �ʵ���ʱԼΪ1s
		//----��127����0������Ϩ�𡢡��ȵ�����ɾ��
		for(i=128;i>0;i--)
		{
			LCD_ClearSeg(i-1);    //ɾ��һ��
			HT1621_Reflash(LCD_Buffer);
                        __delay_cycles(myMCLK);	
		}
		// __delay_cycles(myMCLK);		// �ʵ���ʱԼΪ1s
		//-----�����������ʾһ�£���0�л���9----
		/*8for(i=0;i<=9;i++)
		{
			for(j=1;j<=10;j++)
			{
				LCD_DisplayDigit(i,j);
			}
			HT1621_Reflash(LCD_Buffer);
			 __delay_cycles(myMCLK/2);		// �ʵ���ʱԼΪ1/2s
		}*/
		
		//-----���������ʾ-12456----
		/*LCD_DisplayNum(-123456);
		  HT1621_Reflash(LCD_Buffer);
		  __delay_cycles(myMCLK);*/			// �ʵ���ʱԼΪ1s*/
		
                //----������������
                for(i=0;i<10;i++)
                {
		  LCD_DisplayDigit(i,i+1);  //�������ʾ����
                  HT1621_Reflash(LCD_Buffer);
                  __delay_cycles(myMCLK);
                }
		
                
		_BIS_SR(LPM3_bits);	//�Լ���ϣ�����
	}
}


