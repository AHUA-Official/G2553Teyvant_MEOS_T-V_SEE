


#include<msp430g2553.h>
#include "mytype.h"
#include "clock.h"
#include "UASRT.h"
#include "Mpu-6050.h"
#include "Filter.h"
#include "stdio.h"

char *tishi = "mpu-6050";

/*
**********************************************
**������  ��void main(void)
**�������ܣ���ʼ��������Դ�����жϣ�����͹���
**���ز�������
**�����������
**********************************************
*/
void main(void)
{ 
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�
    
    float result_accel,result_gyro; // ת���������
    char sum1[10],sum2[10]; // ���ڷ��ͻ���
    float filter_result[2]; // �����˲�������ֵ����Ǽ��ٶ�ֵ    
    
   
/*    float result_gyro_X,result_gyro_Y,result_gyro_Z; // ת���������
    float result_accel_X,result_accel_Y,result_accel_Z; 
    char sum1[10],sum2[10],sum3[10]; // ���ڷ��ͻ���
//    char sum4[10],sum5[10],sum6[10];
    float filter_result[2]; // �����˲�������ֵ����Ǽ��ٶ�ֵ*/
    _DINT();    //�ر����ж�
    
    clock_init();
    UARST_int();
    InitMPU6050(); // ��ʼ��ģ��
    Delays();
    PutString(tishi);
    //_EINT(); // ʹ���ж� 
    while(1)
    {
        Delays();

/*        result_gyro_X  = Mpu6050GyroAngle(GYRO_XOUT); // ���������ǵ�X��ֵ
        result_gyro_Y  = Mpu6050GyroAngle(GYRO_YOUT); // ���������ǵ�Y��ֵ
        result_gyro_Z  = Mpu6050GyroAngle(GYRO_ZOUT); // ���������ǵ�
        
        result_accel_X = Mpu6050AccelAngle(ACCEL_XOUT);
        result_accel_Y = Mpu6050AccelAngle(ACCEL_YOUT);
        result_accel_Z = Mpu6050AccelAngle(ACCEL_ZOUT);
        AngleCalcu(result_accel_Y,result_gyro_X,filter_result);// �������˲����ں�
       
        
 //       dis_LCD160(Angle_gx);
        
        //result_gyro = (float)Get6050Data(GYRO_XOUT);
        sprintf(sum1,"%.2f",filter_result[0]); // ���������ֵת��Ϊ�ַ���
        sprintf(sum2,"%.2f",filter_result[1]); // ���������ת��Ϊ�ַ���
        sprintf(sum3,"%.2f",result_gyro_Z);
*/
        result_accel = Mpu6050AccelAngle(ACCEL_YOUT); // �������ٶȴ�������Y��ֵ
        result_gyro = Mpu6050GyroAngle(GYRO_XOUT); // ���������ǵ�X��ֵ
        AngleCalcu(result_accel,result_gyro,filter_result);// �������˲����ں�
        
        //result_gyro = (float)Get6050Data(GYRO_XOUT);
        sprintf(sum1,"%.2f",filter_result[0]); // ���������ֵת��Ϊ�ַ���
        sprintf(sum2,"%.2f",filter_result[1]); // ��������Ǽ��ٶ�ֵת��Ϊ�ַ���        
        
        
        PutString("The measured accel_X is : ");
        PutString(sum1);
        PutChar('\n');
        PutString("The measured accel_Y is : ");
        PutString(sum2);
        PutChar('\n');
        __delay_cycles(1000000);
    }
}


