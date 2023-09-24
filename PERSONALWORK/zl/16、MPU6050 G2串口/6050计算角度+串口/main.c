


#include<msp430g2553.h>
#include "mytype.h"
#include "clock.h"
#include "UASRT.h"
#include "Mpu-6050.h"
#include "Filter.h"
#include "stdio.h"
#include "math.h"


char *tishi = "mpu-6050";



void main(void)
{ 
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�
    
    char sum1[10],sum2[10],sum3[10]; // ���ڷ��ͻ���
    float a_x,a_y,a_z;
    
    clock_init();
    UARST_int();
    InitMPU6050(); // ��ʼ��ģ��
    Delays();
    PutString(tishi);
  
    while(1)
    {
        Delays();

        a_x = mpu6050_Angle(2);
        a_y = mpu6050_Angle(1);
        a_z = mpu6050_Angle(0);

        
        sprintf(sum1,"%.2f",a_x); // ���������ֵת��Ϊ�ַ���
        sprintf(sum2,"%.2f",a_y);
        sprintf(sum3,"%.2f",a_z);
        
        
        
        PutString("The measured accel_X is : ");
        PutString(sum1);
        PutChar('\t');
        PutChar('\n');
        PutString("The measured accel_Y is : ");
        PutString(sum2);
        PutChar('\t');
        PutChar('\n');
        PutString("The measured accel_Z is : ");
        PutString(sum3);
        PutChar('\t');
        PutChar('\n');        
       // __delay_cycles(1000000);
    }
}

