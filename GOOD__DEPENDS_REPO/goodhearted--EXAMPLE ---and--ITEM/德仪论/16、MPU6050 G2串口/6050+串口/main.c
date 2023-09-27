


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
**函数名  ：void main(void)
**函数功能：初始化所有资源并打开中断，进入低功耗
**返回参数：无
**传入参数：无
**********************************************
*/
void main(void)
{ 
    WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗
    
    float result_accel,result_gyro; // 转换结果缓存
    char sum1[10],sum2[10]; // 串口发送缓存
    float filter_result[2]; // 缓存滤波后的倾角值和倾角加速度值    
    
   
/*    float result_gyro_X,result_gyro_Y,result_gyro_Z; // 转换结果缓存
    float result_accel_X,result_accel_Y,result_accel_Z; 
    char sum1[10],sum2[10],sum3[10]; // 串口发送缓存
//    char sum4[10],sum5[10],sum6[10];
    float filter_result[2]; // 缓存滤波后的倾角值和倾角加速度值*/
    _DINT();    //关闭总中断
    
    clock_init();
    UARST_int();
    InitMPU6050(); // 初始化模块
    Delays();
    PutString(tishi);
    //_EINT(); // 使能中断 
    while(1)
    {
        Delays();

/*        result_gyro_X  = Mpu6050GyroAngle(GYRO_XOUT); // 测量陀螺仪的X轴值
        result_gyro_Y  = Mpu6050GyroAngle(GYRO_YOUT); // 测量陀螺仪的Y轴值
        result_gyro_Z  = Mpu6050GyroAngle(GYRO_ZOUT); // 测量陀螺仪的
        
        result_accel_X = Mpu6050AccelAngle(ACCEL_XOUT);
        result_accel_Y = Mpu6050AccelAngle(ACCEL_YOUT);
        result_accel_Z = Mpu6050AccelAngle(ACCEL_ZOUT);
        AngleCalcu(result_accel_Y,result_gyro_X,filter_result);// 卡尔曼滤波及融合
       
        
 //       dis_LCD160(Angle_gx);
        
        //result_gyro = (float)Get6050Data(GYRO_XOUT);
        sprintf(sum1,"%.2f",filter_result[0]); // 将测量倾角值转换为字符串
        sprintf(sum2,"%.2f",filter_result[1]); // 将测量倾角转换为字符串
        sprintf(sum3,"%.2f",result_gyro_Z);
*/
        result_accel = Mpu6050AccelAngle(ACCEL_YOUT); // 测量加速度传感器的Y轴值
        result_gyro = Mpu6050GyroAngle(GYRO_XOUT); // 测量陀螺仪的X轴值
        AngleCalcu(result_accel,result_gyro,filter_result);// 卡尔曼滤波及融合
        
        //result_gyro = (float)Get6050Data(GYRO_XOUT);
        sprintf(sum1,"%.2f",filter_result[0]); // 将测量倾角值转换为字符串
        sprintf(sum2,"%.2f",filter_result[1]); // 将测量倾角加速度值转换为字符串        
        
        
        PutString("The measured accel_X is : ");
        PutString(sum1);
        PutChar('\n');
        PutString("The measured accel_Y is : ");
        PutString(sum2);
        PutChar('\n');
        __delay_cycles(1000000);
    }
}


