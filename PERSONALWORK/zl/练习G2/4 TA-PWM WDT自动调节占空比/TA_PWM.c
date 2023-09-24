/*
 * TA_PWM.c
 * ������PWM�⺯����������TA��������PWM��
 *  Created on: 2013-2-9
 *  Author: Administrator
 */

#include "msp430g2553.h"
#include "TA_PWM.h"

#define DEADTIME 20//Ԥ������ʱ�䣬��TA��clkΪ��λ

/*******�趨TA���IO�ڣ�Ŀǰ�趨ΪMSP430G2553��20Pin��װ��TA0.2********/
#define TA01_SET 	P1SEL |= BIT6; P1DIR |= BIT6		//P1.6
#define TA02_SET 	P3SEL |= BIT0; P3DIR |= BIT0		//P3.0
#define TA11_SET 	P2SEL |= BIT2; P2DIR |= BIT2		//P2.2
#define TA12_SET 	P2SEL |= BIT4; P2DIR |= BIT4		//P2.4
#define TA01_OFF 	P1SEL &= ~BIT6; 			//P1.6
#define TA02_OFF 	P3SEL &= ~BIT0; 						//P3.0
#define TA11_OFF 	P2SEL &= ~BIT2; 						//P2.2
#define TA12_OFF 	P2SEL &= ~BIT4; 						//P2.4
/******************************************************************************************************
* ��       �ƣ�TA0_PWM_Init()
* ��       �ܣ�TA0��ʱ����ΪPWM�������ĳ�ʼ�����ú���
* ��ڲ�����Clk:ʱ��Դ 'S'=SMCLK;   'A'=ACLK ;   'E'=TACLK(�ⲿ����)�� 'e'= TACLK(TACLKȡ��)
                    Div:ʱ�ӷ�Ƶϵ��: 1/2/4/8
		    Mode1:ͨ��1�����ģʽ 'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM��ģʽ3�� ��'D'��������PWM��ģʽ6����0=����
		    Mode2:ͨ��2�����ģʽ 'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM ��ģʽ3����'D'��������PWM��ģʽ2����0=����
		    ����������������Ƶ�PWMʱ����ͨ������ʹ�ã��Ҿ�Ϊ����ģʽ��
* ���ڲ�����1��ʾ���óɹ���0��ʾ������������ʧ�ܡ�
* ˵       �� : �ڵ���PWM��غ���֮ǰ����Ҫ���øú�������TA��ģʽ��ʱ��Դ��
* ��       �� : TA0_PWM_Init('A',1,'F','P')TAʱ����ΪACLK,ͨ��1��ͨ��2��Ϊ��ǰPWM���
                   TA0_PWM_Init('S',4,'D','D')TAʱ����ΪSMCLK/4, ͨ��1Ϊ������PWM��ͨ��2Ϊ������PWM
                   TA0_PWM_Init('A',1,'F',0)TAʱ����ΪACLK,ͨ��1��ǰPWM�����ͨ��2����TA�á�
 ******************************************************************************************************/
char TA0_PWM_Init(char Clk,char Div,char Mode1,char Mode2)
{
  TA0CTL =0;// �����ǰ����

  switch(Mode1)	//Ϊ��ʱ��ѡ�����ģʽ
  {
  case 'F': case 'f':	//��ͨPWM
		  TA0CTL |=MC_1; break;	//����ʱ��Ϊ������
  case 'B': case 'b':
	  	  TA0CTL |=MC_1; break;	//����ʱ��Ϊ������
  case 'D': case 'd'://����PWM
     	          TA0CTL |=MC_3; break;	//����ʱ��Ϊ��������
  default : return(0);		        //��������������ò������󣬷���0
  }

  switch(Clk)//Ϊ��ʱ��TAѡ��ʱ��Դ
  {
    case 'A': case 'a':  	TA0CTL|=TASSEL_1; break;   	//ACLK
    case 'S': case 's': 	TA0CTL|=TASSEL_2; break;  	//SMCLK
    case 'E':            	TA0CTL|=TASSEL_0; break;  	//�ⲿ����(TACLK)
    case 'e':          		TA0CTL|=TASSEL_3; break;   	//�ⲿ����(TACLKȡ��)
    default :  return(0);//���ò������󣬷���0
  }
  switch(Div)//Ϊ��ʱ��TAѡ���Ƶϵ��
  {
    case 1:   TA0CTL|=ID_0; break;   //1
    case 2:   TA0CTL|=ID_1; break;   //2
    case 4:   TA0CTL|=ID_2; break;   //4
    case 8:   TA0CTL|=ID_3; break;   //8
    default :  return(0);  //���ò������󣬷���0
  }
  switch(Mode1)//����PWMͨ��1�����ģʽ��
  {
     case 'F':	case 'f':
              TA0CCTL1 = OUTMOD_7;
              TA01_SET;
              break;
     case 'B':	case 'b':
              TA0CCTL1 = OUTMOD_3;
              TA01_SET;
              break;
     case 'D': case'd':
	      TA0CCTL1 = OUTMOD_6;
    	      TA01_SET;
    	      break;
      case '0':case 0: //�������Ϊ����
              TA01_OFF; //TA0.1�ָ�Ϊ��ͨIO��
              break;
     default :  return(0);//���ò������󣬷���0
  }
  switch(Mode2)//����PWMͨ��2�����ģʽ��
  {
       case 'F': case 'f':
              TA0CCTL2 = OUTMOD_7;
              TA02_SET;  
              break;
       case 'B': case 'b':
              TA0CCTL2 = OUTMOD_3;
              TA02_SET;
              break;
       case 'D': case 'd':
	      TA0CCTL2 = OUTMOD_2;
	      TA02_SET;
	      break;
       case '0': case 0://�������Ϊ����
              TA02_OFF;//TA0.1�ָ�Ϊ��ͨIO��
              break;
       default :  return(0);//���ò������󣬷���0
    }
  return(1);
}
/******************************************************************************************************
* ��       �ƣ�TA0_PWM_SetPeriod()
* ��       �ܣ�����PWM������������
* ��ڲ�����Channel: TA0=0, TA1=1
*                     Period:����(0~65535) ʱ�Ӹ���
* ���ڲ�����1�����óɹ�     0������ʧ��
* ˵       �� : ��ͨPWM�������PWM�������һ��
* ��       �� : TA0_PWM_SetPeriod(500)����PWM��������Ϊ500��1000��ʱ������
 ******************************************************************************************************/
char TA0_PWM_SetPeriod(unsigned int Period)
{
	if (Period>65534)	
          return(0);
	TA0CCR0 = Period;
	  return(1);
}

/******************************************************************************************************
* ��    �ƣ�TA0_PWM_SetPermill()
* ��    �ܣ�����PWM�����ռ�ձ�(ǧ�ֱ�)
* ��ڲ�����Channel: ��ǰ���õ�ͨ����  1/2
            Duty: PWM�ߵ�ƽ��Чʱ���ǧ�ֱ� (0~1000)��
* ���ڲ�����1���óɹ���0����ʧ��
* ˵    ��: 1000=100.0%  500=50.0% ���������ơ�����ģʽʱ����channelͬʱ�趨��
* ��    ��: TA0_PWM_SetPermill(1,300)����PWMͨ��1������ռ�ձ�Ϊ30.0%
               TA0_PWM_SetPermill(2,,825)����PWMͨ��2������ռ�ձ�Ϊ82.5%
 ******************************************************************************************************/
char TA0_PWM_SetPermill(char Channel,unsigned int Duty)
{
	unsigned char Mod = 0;
	unsigned long int Percent=0;//��ֹ�˷�����ʱ���
	Percent=Duty;
	switch (Channel)//���жϳ�ͨ���Ĺ���ģʽ
	{
	case 1:
		Mod = (TA0CCTL1 & 0x00e0)>>5;break;//��ȡ���ģʽ��OUTMOD0λ��5-7λ
	case 2:
		Mod = (TA0CCTL2 & 0x00e0)>>5;break;//��ȡ���ģʽ��OUTMOD1λ��5-7λ
	default: return(0);
	}

	switch(Mod)//����ģʽ�趨TACCRx
	{
	case 2: case 6:	/**����ģʽ2,6ʱ����Ҫ�ж���������ʱ�䣬��ͬʱ�趨TA0CCR1/2 ��ֵ*/
		{
			if((1000-2*Percent)<=DEADTIME)//Ԥ������ʱ��
			Percent=(1000-DEADTIME)/2;
			TA0CCR1=Percent*TA0CCR0/1000;
			TA0CCR2= TA0CCR0-TA0CCR1;
			break;
		}
		case 7:
		{
			if(Percent>1000)	Percent=1000;
			if(Channel==1) TA0CCR1=Percent* TA0CCR0/1000;
			if(Channel==2) TA0CCR2=Percent* TA0CCR0/1000;
			break;
		}
		case 3:	//ռ�ձ�һ��Ϊ������������Ҫ TA0CCR0��ȥռ�ձ�
		{
			if(Percent>1000)	Percent=1000;
			if(Channel==1) TA0CCR1= TA0CCR0-Percent*TA0CCR0/1000;
			if(Channel==2) TA0CCR2= TA0CCR0-Percent*TA0CCR0/1000;
			break;
		}
		default: return(0);
		}
		return (1);
	}

/*************TA1*******************/

/******************************************************************************************************
* ��       �ƣ�TA1_PWM_Init()
* ��       �ܣ�TA1��ʱ����ΪPWM�������ĳ�ʼ�����ú���
* ��ڲ�����Clk:ʱ��Դ 'S'=SMCLK;   'A'=ACLK ;   'E'=TACLK(�ⲿ����)�� 'e'= TACLK(TACLKȡ��)
                    Div:ʱ�ӷ�Ƶϵ��: 1/2/4/8
		    Mode1:ͨ��1�����ģʽ 'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM��ģʽ3�� ��'D'��������PWM��ģʽ6����0=����
		    Mode2:ͨ��2�����ģʽ 'P'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM ��ģʽ3����'D'��������PWM��ģʽ2����0=����
		    ����������������Ƶ�PWMʱ����ͨ������ʹ�ã��Ҿ�Ϊ����ģʽ��
* ���ڲ�����1��ʾ���óɹ���0��ʾ������������ʧ�ܡ�
* ˵       �� : �ڵ���PWM��غ���֮ǰ����Ҫ���øú�������TA��ģʽ��ʱ��Դ��
* ��       �� : TA1_PWM_Init('A',1,'P','P')TAʱ����ΪACLK,ͨ��1��ͨ��2��Ϊ��ǰPWM���
                   TA1_PWM_Init('S',4,'D','D')TAʱ����ΪSMCLK/4, ͨ��1Ϊ������PWM��ͨ��2Ϊ������PWM
                   TA1_PWM_Init('A',1,'P',0)TAʱ����ΪACLK,ͨ��1��ǰPWM�����ͨ��2����TA�á�
 ******************************************************************************************************/
char TA1_PWM_Init(char Clk,char Div,char Mode1,char Mode2)
{
  TA1CTL =0;	
  // �����ǰ����

  switch(Mode1)	//Ϊ��ʱ��ѡ�����ģʽ
  {
  case 'F': case 'f'://��ͨPWM
	  TA1CTL |=MC_1; break;	//����ʱ��Ϊ������
  case 'B':case 'b':
	  TA1CTL |=MC_1; break;	//����ʱ��Ϊ������
  case 'D': case 'd':														//����PWM
          TA1CTL |=MC_3; break;	//����ʱ��Ϊ��������
  default : return(0);		//��������������ò������󣬷���0
  }

  switch(Clk)  																//Ϊ��ʱ��TAѡ��ʱ��Դ
  {
    case 'A': case 'a':  	TA1CTL|=TASSEL_1; break;  //ACLK
    case 'S': case 's': 	TA1CTL|=TASSEL_2; break;  //SMCLK
    case 'E':            	TA1CTL|=TASSEL_0; break;  //�ⲿ����(TACLK)
    case 'e':          		TA1CTL|=TASSEL_3; break;  //�ⲿ����(TACLKȡ��)
    default :  return(0);  				  //���ò������󣬷���0
  }
  switch(Div) 						  //Ϊ��ʱ��TAѡ���Ƶϵ��
  {
    case 1:   TA1CTL|=ID_0; break;   //1
    case 2:   TA1CTL|=ID_1; break;   //2
    case 4:   TA1CTL|=ID_2; break;   //4
    case 8:   TA1CTL|=ID_3; break;   //8
    default :  return(0);  //���ò������󣬷���0
  }
  switch(Mode1)		   //����PWMͨ��1�����ģʽ��
  {
     case 'F':	case 'f':
              TA1CCTL1 =OUTMOD_7;
              TA11_SET;
              break;
     case 'B':	case 'b':
              TA1CCTL1 =OUTMOD_3;
              TA11_SET;
              break;
     case 'D': case'd':
	     TA1CCTL1 =OUTMOD_6;
    	     TA11_SET;
    	     break;
      case '0':case 0:   			//�������Ϊ����
             TA11_OFF;   			//TA0.1�ָ�Ϊ��ͨIO��
             break;
     default :  return(0); 			//���ò������󣬷���0
  }
  switch(Mode2) 			        //����PWMͨ��2�����ģʽ��
  {
       case 'F': case 'f':
              TA1CCTL2 =OUTMOD_7;
              TA12_SET;
       case 'B': case 'b':
              TA1CCTL2 =OUTMOD_3;
              TA12_SET;
              break;
       case 'D': case 'd':
	      TA1CCTL2 =OUTMOD_2;
	      TA12_SET;
	      break;
       case '0':case 0:   		//�������Ϊ����
              TA12_OFF;   		//TA0.1�ָ�Ϊ��ͨIO��
              break;
       default :  return(0);//���ò������󣬷���0
    }
  return(1);
}
/******************************************************************************************************
* ��       �ƣ�TA1_PWM_SetPeriod()
* ��       �ܣ�����PWM������������
* ��ڲ�����Channel: TA0=0, TA1=1
*                     Period:����(0~65535) ʱ�Ӹ���
* ���ڲ�����1�����óɹ�     0������ʧ��
* ˵       �� : ��ͨPWM�������PWM�������һ��
* ��       �� : TA1_PWM_SetPeriod(500)����PWM��������Ϊ500��1000��ʱ������
 ******************************************************************************************************/
char TA1_PWM_SetPeriod(unsigned int Period)
{
	if (Period>65534)	
          return(0);
	TA1CCR0 = Period;
	  return(1);
}

/******************************************************************************************************
* ��    �ƣ�TA0_PWM_SetPermill()
* ��    �ܣ�����PWM�����ռ�ձ�(ǧ�ֱ�)
* ��ڲ�����Channel: ��ǰ���õ�ͨ����  1/2
            Duty: PWM�ߵ�ƽ��Чʱ���ǧ�ֱ� (0~1000)��
* ���ڲ�����1���óɹ���0����ʧ��
* ˵    ��: 1000=100.0%  500=50.0% ���������ơ�����ģʽʱ����channelͬʱ�趨��
* ��    ��: TA_PWM_SetPermill(1,300)����PWMͨ��1������ռ�ձ�Ϊ30.0%
               TA_PWM_SetPermill(2,,825)����PWMͨ��2������ռ�ձ�Ϊ82.5%
 ******************************************************************************************************/
char TA1_PWM_SetPermill(char Channel,unsigned int Duty)
{
	unsigned char Mod;
	unsigned long int Percent=0;//��ֹ�˷�����ʱ���
	Percent=Duty;
	switch (Channel)	//���жϳ�ͨ���Ĺ���ģʽ
	{
	case 1:
		Mod = (TA1CCTL1 & 0x00e0)>>5;		break;	//��ȡ���ģʽ��OUTMOD0λ��5-7λ
	case 2:
		Mod = (TA1CCTL2 & 0x00e0)>>5;		break;	//��ȡ���ģʽ��OUTMOD1λ��5-7λ
	default:	return(0);
	}

	switch(Mod)			//����ģʽ�趨TACCRx
	{
	case 2: case 6:			/**����ģʽ2,6ʱ����Ҫ�ж���������ʱ�䣬��ͬʱ�趨TA1CCR1/2 ��ֵ*/
		{
			if((1000-2*Percent)<=DEADTIME)	//Ԥ������ʱ��
				Percent=(1000-DEADTIME)/2;
			TA1CCR1=Percent* TA1CCR0/1000;
			TA1CCR2= TA1CCR0-TA1CCR1;
			break;
		}
		case 7:
		{
			if(Percent>1000)	Percent=1000;
			if(Channel==1) TA1CCR1=Percent* TA1CCR0/1000;
			if(Channel==2) TA1CCR2=Percent* TA1CCR0/1000;
			break;
		}
		case 3:		//ռ�ձ�һ��Ϊ������������Ҫ TA1CCR0��ȥռ�ձ�
		{
			if(Percent>1000)	Percent=1000;
			if(Channel==1) TA1CCR1= TA1CCR0-Percent* TA1CCR0/1000;
			if(Channel==2) TA1CCR2= TA1CCR0-Percent* TA1CCR0/1000;
			break;
		}
		default: return(0);
	}
	return (1);
}
