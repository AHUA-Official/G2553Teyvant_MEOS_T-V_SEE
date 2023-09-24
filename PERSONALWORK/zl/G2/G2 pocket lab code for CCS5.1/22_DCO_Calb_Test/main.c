/*===================DCO��У�����������========================
 * �������������32.768kHz��Ƶ����У������DCOƵ�ʡ�
 * У���Ĳ�����˳�������InfoFLashB�С�
 * ����У�黷����ʵ��Ӧ�û���һ�£���У���Ƶ��ֵ�ȳ���У��ֵ����ȷ��
 * ��У������ĸ�ʽ�����У������ĸ�ʽһ�¡�
 * У��ʱLaunchpad G2�ϵĺ�ƻ�����У������̵�����
 * У����Ϻ�P1.4����ѭ�����У����DCOƵ��
 * ==================ATTENTION======================
 * ȷ������CCS���̵�Erase Options��Ϊѡ��Erase main memory only����
 * ֻ����������InfoFlashB��У������Ų�����Ϊ��¼�������ʧ��InfoFlashA������
 * д������������Ϊ��¼��ʧ�� �Ӽ����Ƕȣ���Ȼ���԰���У�����д��InfoFlashA��
 * ��32�ֽڣ���0x10C0��0x10E0����ͬʱ��������У�������0x10F8��0x10FF)����
 * ������������������ȫ���ź����б�д����ʵ�֡�
 **************************************************************************************************/
#include "MSP430G2553.h"
#include "Flash.h"
#define       CALDCO_MHz(x)			* (unsigned char *)(0x1080+(x-1)*2)			//��ȡInfoFlashB
#define       CALBC1_MHz(x)				* (unsigned char *)(0x1080+(x-1)*2+1)
#define 		CAL_NUM 		16										/*��У��Ƶ�ʵ�����*/
unsigned int Temp[128]={0};										/*���ʵ���128������С����ƵMODx�ġ�������Ƶ��*/
unsigned char RSELx_DCOx_Winer[CAL_NUM]={0};	/*���У�����SELx��DCOxֵ*/
unsigned char MODx_Winer[CAL_NUM]={0};				/*���У�����MODxֵ*/
unsigned int 	InfoFlash_Data[CAL_NUM]={0};				/*��д��infoB�е�У��Ƶ�ʲ�������*/
unsigned int Ture_Freq[CAL_NUM]={0};						/*�������ʱ�鿴ʵ��Ƶ��ֵ*/

		// -----��У����Ƶ�ʣ���λKHz-----
const unsigned int Calibrate_Frequence[CAL_NUM]={1000,2000,3000,4000,5000,6000,
		                                 7000,8000,9000,10000,11000,12000,13000,14000,15000,16000};

void DCO_Set_Freq(unsigned char Num,unsigned char mod_Temp);
void Measure_Temp();
void Find_RSELx_DCOx_Winer();
void Measure_MODx();
void Calculate_InfoFlash_Data();
void Write_InfoFlashB();
void DCO_InfoFlashB(unsigned int freq);
void Calibrate_Start();
void Calibrate_Finish();
void DCOTest();

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;				//�ع�
	//-----��P1.4�����SMCLK����������ȫ����ʾ������Ƶ�ʼƹ۲�DCO----
	P1SEL |= BIT4;
	P1DIR |= BIT4;
	//-----��ʼ��TA������WDT�ж�ʹ��-----
	TA0CTL = TASSEL_2+MC_2+TACLR;          	// SMCLKʱ��Դ����������ʼ
	IE1|=WDTIE;													//����WDT�ж�
	 _enable_interrupts();									//��ͬ_ENT���������ж�
	//-----��ʼDCOƵ��У�飬ֻ����һ��-----
	Calibrate_Start();						//����1�����������ʾ����У��DCO
	Measure_Temp();					//����2���Զ�����128������Ƶ��
	Find_RSELx_DCOx_Winer();	//����3����128������Ƶ���У��ҳ���ӽ�Ԥ��Ƶ�ʵ��Ǽ���
	Measure_MODx();					//����4��������ӵĴ���Ƶ�ʻ����ϣ�����ҵ����ŵ�MODx����
	Calculate_InfoFlash_Data();	//����5��������ʵ�Ƶ���趨������ϳ�У������
	Write_InfoFlashB();					//����6����У������д��InfoFlahB��
	Calibrate_Finish();					//����7���̵�������ʾ���У��DCO
	//-----���DCOУ�飬��Flash�ж�ʱȡ��У׼ֵ����P1.4���������-----
	DCOTest();								//����8��ѭ�����DCO����У��Ƶ��ֵ�����۲�
}
/******************************************************************************************************
 * ��       �ƣ�Calibrate_Start()
 * ��       �ܣ���ɫLED������������У����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����DCOУ���ʱ��Ƚϳ������ú��ָʾ���Է�����Գ���
 * ��       ������
 ******************************************************************************************************/
void Calibrate_Start()		//����1�����������ʾ����У��DCO
{
	P1DIR |=BIT0+BIT6;
	P1OUT |= BIT0;				//���������ʾ����У��
	P1OUT &=~BIT6;
}
/******************************************************************************************************
 * ��       �ƣ�Measure_Temp()
 * ��       �ܣ��Զ����βⶨ128�ִ���DCO��Ƶ�ʣ�������Temp[]����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ��������Ƶ����ָMOD=0ʱ�ķǻ�Ƶ
 * ��       ������
 ******************************************************************************************************/
void Measure_Temp()			//����2���Զ�����128������Ƶ��
{
	unsigned long Freq_Temp=0;
	unsigned char RSELx_DCOx_Num=0;
	for(RSELx_DCOx_Num=0;RSELx_DCOx_Num<128;RSELx_DCOx_Num++)
	{
		DCO_Set_Freq(RSELx_DCOx_Num,0);
		//-----��ʱ��Ƶ���뿪ʼ-----
		TA0CTL |=TACLR;							//������ʼ
		WDTCTL=WDT_ADLY_1_9;				//WDT��ʱ����Ϊ����ѡ�����
		 _bis_SR_register(LPM0_bits);		//CPU��ȷ����1.9ms(1/512��)
		WDTCTL=WDTPW+WDTHOLD;
		Freq_Temp = TA0R;						//��ȡ���������ֵ����DCO��1/512���ж������������
		//-----��ʱ��Ƶ�������-----
		Temp[RSELx_DCOx_Num]=((Freq_Temp)*512)/1000;		//����DCO׼ȷƵ��
	}
}
/******************************************************************************************************
 * ��       �ƣ�Find_RSELx_DCOx_Winer()
 * ��       �ܣ���128��Temp[]�У��ҳ���16��У��Ƶ����ӽ���Temp�����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����Temp������������RSELx��DCOx��ȡֵ��Ϣ
 * ��       ������
 ******************************************************************************************************/
void Find_RSELx_DCOx_Winer()		//����3����128������Ƶ���У��ҳ���ӽ�Ԥ��Ƶ�ʵ��Ǽ���
{
	unsigned char i=0;
	unsigned int Delta_Min=0;
	unsigned int Delta_Now=0;
	unsigned char Calibrate_Num=0;
	for(Calibrate_Num=0;Calibrate_Num<CAL_NUM;Calibrate_Num++)
	{
		Delta_Min=65530;														//��һ�αȽ�ʱ�����ⶨһ�����ֵ
		for(i=0;i<128;i++)
		{
			//-----������ȡ��С��ֵ��һ��ҪС��Ԥ��ֵ-----
			if(Temp[i]<Calibrate_Frequence[Calibrate_Num])
				{
					Delta_Now=Calibrate_Frequence[Calibrate_Num]-Temp[i];
					if (Delta_Now<Delta_Min)							//�����ǰ��ֵ�ȡ������¼����Сʱ
						{
						RSELx_DCOx_Winer[Calibrate_Num]=i;		//��Winner����
						Delta_Min=Delta_Now;								//ȡ��ǰ�������¼��
						}
				}
		}
	}
}
/******************************************************************************************************
 * ��       �ƣ�Measure_MODx()
 * ��       �ܣ�������У��ɹ���RSELx��DCOx����һ��У�������ʵ�MODx
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �������������̶�RSELx��DCOx�����32��MODx��ȡֵ��Ȼ���ж����ŵ�MODx
 * ��       ������
 ******************************************************************************************************/
void Measure_MODx()		//����4��������ӵĴ���Ƶ�ʻ����ϣ�����ҵ����ŵ�MODx����
{
	unsigned char mod_x = 0;
	unsigned long  Freq_Temp = 0;
	unsigned int Delta_Now=0;
	unsigned int Delta_Min=0;
    unsigned char Calibrate_Num=0;
	for(Calibrate_Num=0;Calibrate_Num<CAL_NUM;Calibrate_Num++)
	{
		Delta_Min=65530;											//��һ�αȽ�ʱ�����ⶨһ�����ֵ
		//-----RSELx��DCOxһ��ʱ������mod_x�ı�ʱ��ϵ��Ƶ��-----
		for(mod_x=0;mod_x <32;mod_x++)
		{
			DCO_Set_Freq(RSELx_DCOx_Winer[Calibrate_Num],mod_x);	//�ı�mod_x����DCO
			//-----��ʱ��Ƶ���뿪ʼ-----
			TA0CTL |=TACLR;
			WDTCTL=WDT_ADLY_1_9;
			 _bis_SR_register(LPM0_bits );					//CPU��ȷ����1.9ms(1/512��)
			WDTCTL=WDTPW+WDTHOLD;
			Freq_Temp = TA0R;									//��ȡ"��Ƶ"ֵ
			//-----��ʱ��Ƶ�������-----
			Freq_Temp=((Freq_Temp)*512)/1000;		//����DCO׼ȷƵ��
			//-----��Find_RSELx_DCOx_Winer()��ͬ�������Ҫ����������ֵ��С-----
			if(Freq_Temp<Calibrate_Frequence[Calibrate_Num])
			{
				Delta_Now=Calibrate_Frequence[Calibrate_Num]-(unsigned int )Freq_Temp;
			}
			else
			{
				Delta_Now=(unsigned int )Freq_Temp-Calibrate_Frequence[Calibrate_Num];
			}
			if (Delta_Now<Delta_Min)						//�����ǰ��ֵ�ȡ������¼����Сʱ
			{
				MODx_Winer[Calibrate_Num]=mod_x;									//��Winner����
				Ture_Freq[Calibrate_Num]=(unsigned int )Freq_Temp;		//��¼У����׼ȷƵ��
				Delta_Min=Delta_Now;							//ȡ��ǰ�������¼��
			}
		}
	}
}
/******************************************************************************************************
 * ��       �ƣ�Calculate_InfoFlash_Data()
 * ��       �ܣ�������У��ɹ���RSELx��DCOx��MODx��װ��д��Flash������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����16λ���ݵĸ�ʽ��ȫ���ճ���У����������4λ�̶�1000��Ȼ������ΪRSLEx��
 * 					 DCOx��MODx��
 * ��       ������
 ******************************************************************************************************/
void Calculate_InfoFlash_Data()		//����5��������ʵ�Ƶ���趨������ϳ�У������
{
	unsigned char RSELx=0;
	unsigned char DCOx=0;
	unsigned char i=0;
	for(i=0;i<CAL_NUM;i++)
	{
		RSELx=RSELx_DCOx_Winer[i]/8;
		DCOx=RSELx_DCOx_Winer[i]%8;
		//-----����У׼������16λ��ʽΪ��1000_RSELx_DCOx_MODx"-----
		InfoFlash_Data[i]=0x8000+(RSELx<<8)+(DCOx<<5)+MODx_Winer[i];
	}
}
/******************************************************************************************************
 * ��       �ƣ�Write_InfoFlashB()
 * ��       �ܣ�������õ�InfoFlash_Data[]���飬����д��InfoFlashB��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ȷ������CCS���̵�Erase Options��Ϊѡ��Erase main memory only����
 * 					ֻ����������InfoFlashB��У������Ų�����Ϊ��¼�������ʧ��
 * ��       ������
 ******************************************************************************************************/
void Write_InfoFlashB()		//����6����У������д��InfoFlahB��
{
	unsigned char i=0;
	//-----дFlashǰDCOʱ��һ��Ҫ����ȷ��һ��-----
    BCSCTL1 = CALBC1_1MHZ;      										/* Set DCO to 8MHz */
    DCOCTL = CALDCO_1MHZ;
    Flash_Init(3,'B');																// ��ʼ��Flash
	Flash_Erase();																	// ����Info_B
	//-----��InfoFlash_Data[CAL_NUM]����InfoFlashB-----
	for(i=0;i<CAL_NUM;i++)
		Flash_Direct_WriteWord(i*2,InfoFlash_Data[i]);		//����ֱ��д
}
/******************************************************************************************************
 * ��       �ƣ�Calibrate_Finish()
 * ��       �ܣ���ɫLED������ʾDCOУ�����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����ڳ��������У�һ��ǡ����LEDָʾ��������ů����
 * ��       ������
 ******************************************************************************************************/
void Calibrate_Finish()			//����7���̵�������ʾ���У��DCO
{
	P1OUT |= BIT6;					//���������ʾ����У��
	P1OUT &=~BIT0;
}
/******************************************************************************************************
 * ��       �ƣ�DCOTest()
 * ��       �ܣ����3��ѭ����ȡInfoFlashB�е�У����������ݴ��趨DCOƵ��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����������ʾ������Ƶ�ʼƽ���P1.4��SMCLK����ڣ���ʵ��DCOУ�������׼ȷ��
 * ��       ������
 ******************************************************************************************************/
void DCOTest()											//����8��ѭ�����DCO����У��Ƶ��ֵ�����۲�
{
	unsigned char i=1,freqCnt=1;
	WDTCTL=WDT_ADLY_1000;					//WDT�ж���Ϊ1sһ��
	while(1)													//�൱������ѭ���ˣ��ظ��л�DCO�����
	{
		if(i++ >2)											//WTD�жϻ�������(����ʱ3s),�л�һ��Ƶ��
		{
			i=1;
			if(freqCnt>CAL_NUM)					// ��Χ���ƣ�1~CAL_NUM��
				freqCnt=1;
		//-----���ճ���У�����CALDCO_xMHz��CALBC1_xMHz����ʽ����DCOƵ��-----
			DCOCTL=CALDCO_MHz(freqCnt);
			BCSCTL1=CALBC1_MHz(freqCnt);
			freqCnt++;
		}
		 _bis_SR_register(LPM0_bits);			//�ȴ�1s��WDT�жϻ���
	}
}
/******************************************************************************************************
 * ��       �ƣ�DCO_Set_Freq()
 * ��       �ܣ����ݴ���������趨DCOƵ��
 * ��ڲ�����Num��Num��0~2λ��ʾDCOx��3~6λ��ʾRSELx
 * 					modTemp��MODx����
 * ���ڲ�������
 * ˵       ����Num��������ְ������DCOx��RSELx����Ϣ
 * ��       ����DCO_Set_Freq(10,5)����ʾ��DCO�趨ΪRSELx=1��DCOx=2��MODx=5
 ******************************************************************************************************/
void DCO_Set_Freq(unsigned char Num,unsigned char modTemp) //�ò����趨DCOƵ��
{
	DCOCTL = ((Num%8)<<5)+modTemp;	//���ݲ��
	BCSCTL1 &=~0x0F;
	BCSCTL1 |=Num/8;
	__delay_cycles(15000);							//�ȴ�DCOƵ���ȶ�
}
/******************************************************************************************************
 * ��       �ƣ�WDT_ISR()
 * ��       �ܣ���ӦWDT��ʱ�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ���������ڲ�ֻ��1�份��CPU���룬����д�����õ���WDT�Ķ�ʱ���ܣ�
 * 					 ͬʱ���⽫��������д���жϷ������У�Ӱ�����Ŀɶ��ԡ�
 * ��       ������
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	__bic_SR_register_on_exit(LPM0_bits );		//�˳��ж�ʱ������CPU
}


