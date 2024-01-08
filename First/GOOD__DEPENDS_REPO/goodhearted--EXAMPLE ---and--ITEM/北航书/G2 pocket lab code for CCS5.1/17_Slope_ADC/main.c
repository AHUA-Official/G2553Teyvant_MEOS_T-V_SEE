/*���棺����G2 LaunchPad�ϵ�LED2ʹ����P1.6��I2C���ţ������������漰��I2C��ʵ�鶼�����P1.6���߰γ�,���������̣�
 * main.c
 */
#include"MSP430G2553.h"
#include "Slope.h"
#include "TCA6416A.h"
#include "HT1621.h"
#include "LCD_128.h"

unsigned char Key=0;		//���̵�λ���ĵ����Ч����ֵ
//-----���⡰���������衱�ķֵ�ֵ��ʵ�ʽ�����5k��ֵ����+10k���̵�λ��-----
const unsigned char R_Class[8]={55,67,82,97,113,128,138,144};		//��λΪ��ŷ

void WDT_OnTime();
void Slope_R_Dect();			//��ô���������¼�������
void TCA6416A_LED(unsigned char num);		//LED������ʾ����
void Display_SLOPE();
void Display_RSENS(unsigned char Rsens);

void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1 = CALBC1_16MHZ;      /* Set DCO to16MHz */
    DCOCTL = CALDCO_16MHZ;
	__delay_cycles(100000);			//�ȴ���ѹ�ȶ�
	TCA6416A_Init();
	Slope_Measure_Init();
	HT1621_init();
	LCD_Clear();
	Display_SLOPE();
	HT1621_Reflash(LCD_Buffer);
	//-----�趨WDTΪ16ms��ʱ�ж�-----
	WDTCTL=WDT_ADLY_16;
	//-----WDT�ж�ʹ��-----
    IE1|=WDTIE;
	 _enable_interrupts();

	while(1);
}

/******************************************************************************************************
 * ��       �ƣ�WDT_ISR(void)
 * ��       �ܣ�WDT��ʱ�ж��Ӻ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ֱ�ӵ����¼�����������
 * ��       ������
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	 IE1&=~WDTIE;
	WDT_OnTime();
	 IE1|=WDTIE;
}
/******************************************************************************************************
 * ��       �ƣ�WDT_OnTime()
 * ��       �ܣ�WDT��ʱ�жϵ��¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������Ҫ���������¼�������������
 * ��       ������
 ******************************************************************************************************/
void WDT_OnTime()
{
	static unsigned char Charge_Ready=0;			//�������־λ
	static unsigned char REF_Mreasure_Ready=0;	//����8�����ο������Ѳ���
	static unsigned long R_REF_Sum=0;			//�ݴ�ο�������ۼ�ֵ
		if(Charge_Ready==0)		//δ������
		{
			Slope_Port_Charge();	//���
			Slope_R_Dect();			//ִ����ʾLED������
			Charge_Ready=1;		//�������־
		}
		else
		{
			//-----����8�βο�����ֵ-----
			if(REF_Mreasure_Ready<8)					// �ж��Ƿ���������ο�����ֵ
			{
				Slope_Measure_REF();						//��ο�����ֵ
				R_REF_Sum=R_REF_Sum+R_REF;		//�ۼӵ���ֵ
				if(REF_Mreasure_Ready==7)	R_REF=R_REF_Sum>>3;	//�����������ο������ƽ��ֵ
				REF_Mreasure_Ready++;					//���������ۼ�
			}
			//-----�õ��ο�����ֵ�󣬿�ʼ��ͣ�Ĳ�������������ֵ-----
			else
				Slope_Measure_SENS();					//����ο�����Ų⴫��������
				Charge_Ready=0;							//����һ�Σ��ָó����
		}
}

/******************************************************************************************************
 * ��       �ƣ�Slope_R_Dect()
 * ��       �ܣ��Դ����������жϵ�λ������LED��ʾ����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������Ҫ���������¼�������������
 * ��       ������
 ******************************************************************************************************/
void Slope_R_Dect()
{
	static unsigned int Rsens=0;
	unsigned char Key_Last=0;		//�����ж��Ƿ������и���
	Key_Last=Key;
	//-----ͨ��10k�ο����裬����������ֵ����λ��ŷ-----
	Rsens=(((unsigned long int)100)*(unsigned long int)R_SENS)/R_REF;
	//-----����������ֵ��жϣ�5k��15k֮�䣬��8��-----
	if(Rsens<=R_Class[0]) 			Key=0;
	else if (Rsens<=R_Class[1]) 	Key=1;
	else if (Rsens<=R_Class[2])	Key=2;
	else if (Rsens<=R_Class[3]) 	Key=3;
	else if (Rsens<=R_Class[4]) 	Key=4;
	else if (Rsens<=R_Class[5]) 	Key=5;
	else if (Rsens<=R_Class[6]) 	Key=6;
	else 	if(Rsens<=R_Class[7]) 	Key=7;
	else											Key=8;
	//-----����һϵ����ʾ����-----
	Display_RSENS(Rsens);		//��ʾʵ�ʵ���ֵ
	if(!(Key==Key_Last))			//��ʾ����ֵ�ֵ
	{
		LCD_DisplayDigit(LCD_DIGIT_CLEAR ,6);
		LCD_DisplayDigit(Key,6);
		HT1621_Reflash_Digit(6);
	}
	TCA6416A_LED(Key);		 	//LED������ʾ
}
/******************************************************************************************************
 * ��       �ƣ�TCA6416A_LED()
 * ��       �ܣ����ݴ���������ֵ��ʵ��LED����Ч��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������Ҫ����I2C��TCA6416����LED
 * ��       ������
 ******************************************************************************************************/
void TCA6416A_LED(unsigned char num)
{
	unsigned char i = 0;
	for(i=0;i<8;i++)
	{
		if(i<num)					// �жϵ�ǰ��Ҫ������յ��
			PinOUT(i,0);			//����LED�ӷ���IO�͵�ƽΪ����LED
		else
			PinOUT(i,1);			//����LED�ӷ���IO�ߵ�ƽΪϨ��LED
	}
}
/******************************************************************************************************
 * ��       �ƣ�Display_SLOPE()
 * ��       �ܣ���LED����ʾ�̶����������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����������ʾSLOPE��2��С���㣬logo��k����λ��
 * ��       ������
 ******************************************************************************************************/
void Display_SLOPE()
{
	//-----��ʾS-----
	LCD_DisplayDigit(5,1);
	//-----��ʾL-----
	LCD_DisplayDigit(0,2);
	LCD_ClearSeg(_LCD_2A);
	LCD_ClearSeg(_LCD_2B);
	LCD_ClearSeg(_LCD_2C);
	//-----��ʾO-----
	LCD_DisplayDigit(0,3);
	//-----��ʾP-----
	LCD_DisplayDigit(8,4);
	LCD_ClearSeg(_LCD_4C);
	LCD_ClearSeg(_LCD_4D);
	//-----��ʾE-----
	LCD_DisplayDigit(8,5);
	LCD_ClearSeg(_LCD_5B);
	LCD_ClearSeg(_LCD_5C);
	//-----��ʾС����-----
	LCD_DisplaySeg(_LCD_DOT4);
	LCD_DisplaySeg(_LCD_DOT6);
	//-----��ʾlogo-----
	LCD_DisplaySeg(_LCD_TI_logo);
	LCD_DisplaySeg(_LCD_QDU_logo);
	//-----��ʾk��-----
	LCD_DisplaySeg(_LCD_k_OHOM);
	LCD_DisplaySeg(_LCD_OHOM);
}
/******************************************************************************************************
 * ��       �ƣ�Display_RSENS()
 * ��       �ܣ���LED��С8�ֶ�����ʾʵ�⴫��������ֵ
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������λ�����иı�Ÿ��¸�λ���Դ档
 * ��       ������
 ******************************************************************************************************/
void Display_RSENS(unsigned char Rsens)
{
	unsigned char i=0;
	static unsigned char Digit[3]={0};
	unsigned char Digit_Past[3]={0};		//���ڶԱ���ֵ�Ƿ��иı�
	for(i=0;i<3;i++){
		Digit_Past[i]=Digit[i];
	}
	//-----�������-----
	Digit[2]=Rsens/100;
	Digit[1]=(Rsens%100)/10;
	Digit[0]=Rsens%10;
	//-----�б���λ�ĸı䣬�����¸�λ�Դ�-----
	for(i=0;i<3;i++)
	{
		if(!(Digit[i]==Digit_Past[i]))		//����б仯
		{
			LCD_DisplayDigit(LCD_DIGIT_CLEAR ,i+8);	//�����8�ֶ�
			LCD_DisplayDigit(Digit[i],i+8);						//д�Դ�ӳ��
			HT1621_Reflash_Digit(i+8);							//�����¸�8�ֶε��Դ�
		}
	}
}

