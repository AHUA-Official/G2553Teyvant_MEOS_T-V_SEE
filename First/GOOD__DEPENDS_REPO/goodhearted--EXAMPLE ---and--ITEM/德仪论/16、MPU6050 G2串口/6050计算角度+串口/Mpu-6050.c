/*
***************************************************************************
**文件名：Mpu-6050.c
**描  述：三轴加速度，三轴陀螺仪传感器Mpu-6050的驱动程序，此处用于149系列。
****************************************************************************
*/

//#include<msp430g2553.h>
//#include "mytype.h"
#include "Mpu-6050.h"

static void I2C_Start();
static void I2C_Stop();
static void I2C_SendACK(uchar ack);
static uchar I2C_RecvACK();
static void I2C_SendByte(uchar dat);
static uchar I2C_RecvACK();

short accData[3]={0};




//**************************************
//I2C起始信号
//**************************************
void I2C_Start()
{
    MPU_SCL_OUT(); // SCL设置为输出
    MPU_SDA_OUT(); // SDA设置为输出
    
    MPU_SDA_H();   //拉高数据线
    MPU_SCL_H();   //拉高时钟线
    DELAY_US(5);   //延时
    MPU_SDA_L();   //产生下降沿
    DELAY_US(5);   //延时
    MPU_SCL_L();   //拉低时钟线
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop()
{
    MPU_SCL_OUT(); // SCL设置为输出
    MPU_SDA_OUT(); // SDA设置为输出
    
    MPU_SDA_L();   //拉低数据线
    MPU_SCL_H();   //拉高时钟线
    DELAY_US(5);   //延时
    MPU_SDA_H();   //产生上升沿
    DELAY_US(5);   //延时
}
//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uchar ack)
{
    MPU_SCL_OUT(); // SCL设置为输出
    MPU_SDA_OUT(); // SDA设置为输出
    
    if(ack) 
        MPU_SDA_H();
    else
        MPU_SDA_L();
//    SDA = ack;                  //写应答信号
    MPU_SCL_H();                    //拉高时钟线
    DELAY_US(5);                  //延时
    MPU_SCL_L();                    //拉低时钟线
    DELAY_US(5);                  //延时
}
//**************************************
//I2C接收应答信号
//**************************************
uchar I2C_RecvACK()
{
    uchar cy;
    MPU_SCL_OUT(); // SCL设置为输出
    MPU_SDA_IN(); // SDA设置为输入
    
    MPU_SCL_H();                    //拉高时钟线
    DELAY_US(5);                 //延时
    if(MPU_SDA_DAT())
    {
        cy=1;
    }
    else 
    {
      cy=0;
    }
//    cy = SDA;                   //读应答信号
    MPU_SCL_L();                    //拉低时钟线
    DELAY_US(5);                //延时
    MPU_SDA_OUT(); // SDA设置为输出
    
    return cy;
    
}
//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    MPU_SCL_OUT(); // SCL设置为输出
    MPU_SDA_OUT(); // SDA设置为输出   
    for (i=0; i<8; i++)         //8位计数器
    {
        if((dat<<i)&0x80)
        {
            MPU_SDA_H();
        }
        else 
        {
            MPU_SDA_L();
        }
       // SDA = cy;               //送数据口
        MPU_SCL_H();                //拉高时钟线
        DELAY_US(5);              //延时
        MPU_SCL_L();                //拉低时钟线
        DELAY_US(5);              //延时
    }
    I2C_RecvACK();
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0,cy;
    MPU_SCL_OUT(); // SCL设置为输出
    MPU_SDA_OUT(); // SDA设置为输出
    
    MPU_SDA_H();  //使能内部上拉,准备读取数据,
    MPU_SDA_IN(); // SDA设置为输入，准备向主机输入数据
    for (i=0; i<8; i++)         //8位计数器
    {
       
        dat <<= 1;
        MPU_SCL_H();                //拉高时钟线
        DELAY_US(5);             //延时
        if(MPU_SDA_DAT()) 
        {
            cy=1;
        }
        else 
        {
          cy=0;
        }
        dat |= cy;             //读数据             
        MPU_SCL_L();                //拉低时钟线
        DELAY_US(5);             //延时
    }
    MPU_SDA_OUT();
    return dat;
}
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void ByteWrite6050(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
uchar ByteRead6050(uchar REG_Address)
{
	uchar REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//**************************************
//合成数据
//**************************************
int Get6050Data(uchar REG_Address)
{
	char H,L;
	H=ByteRead6050(REG_Address);
	L=ByteRead6050(REG_Address+1);
	return (H<<8)+L;   //合成数据
}
//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050()
{
	ByteWrite6050(PWR_MGMT_1, 0x00);  // 解除休眠状态
	ByteWrite6050(SMPLRT_DIV, 0x07);  // 陀螺仪采样率设置（125HZ）
	ByteWrite6050(CONFIG, 0x06);      // 低通滤波器频率设置（5HZ）
	ByteWrite6050(GYRO_CONFIG, 0x18); // 陀螺仪自检及检测范围设置(不自检,16.4LSB/DBS/S)
	ByteWrite6050(ACCEL_CONFIG, 0x01); // 加速计自检、测量范围及高通滤波频率(不自检，2G(16384LSB/G)，5Hz)
}

/*
**********************************************
**函数名  ：float Mpu6050AccelAngle(int8 dir)
**函数功能：输出加速度传感器测量的倾角值
**            范围为2g时，换算关系：16384 LSB/g
**            角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14
**            因为x>=sinx,故乘以1.2适当放大
**返回参数：测量的倾角值
**传入参数：dir - 需要测量的方向
**           ACCEL_XOUT - X方向
**           ACCEL_YOUT - Y方向
**           ACCEL_ZOUT - Z方向
**********************************************
*/
float Mpu6050AccelAngle(int8 dir)
{
    float accel_agle;// 测量的倾角值
    float result; // 测量值缓存变量
    result = (float)Get6050Data(dir); // 测量当前方向的加速度值,转换为浮点数
    accel_agle = (result + MPU6050_ZERO_ACCELL); // 去除零点偏移,计算得到角度（弧度）
//    accel_agle = accel_agle*1.2*180/3.14;     //弧度转换为度
    
    return accel_agle; // 返回测量值
}

/*
**********************************************
**函数名  ：float Mpu6050GyroAngle(int8 dir)
**函数功能：输出陀螺仪测量的倾角加速度
**            范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)
**返回参数：测量的倾角加速度值
**传入参数：dir - 需要测量的方向
**           GYRO_XOUT - X轴方向
**           GYRO_YOUT - Y轴方向
**           GYRO_ZOUT - Z轴方向
**********************************************
*/
float Mpu6050GyroAngle(int8 dir)
{
    float gyro_angle;
//    float Angle_gy;
    gyro_angle = (float)Get6050Data(dir);   // 检测陀螺仪的当前值
    gyro_angle = -(gyro_angle + MPU6050_ZERO_GYRO)/16.4;    //去除零点偏移，计算角速度值,负号为方向处理 
//    Angle_gy += gyro_angle*0.005;
    return gyro_angle; // 返回测量值
}


//采样10次去掉两个最大最小值求平均
void MPU6050ReadAcc()
{
       
        int i=0,j=0;
	int x_buf[10];	
	int y_buf[10];
	int z_buf[10];
	int temp=0;
	long temp2=0;
	for(i=0;i<10;i++)
	{
		x_buf[i] = Get6050Data(ACCEL_XOUT);
		y_buf[i] = Get6050Data(ACCEL_YOUT);
		z_buf[i] = Get6050Data(ACCEL_ZOUT);
	}
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(x_buf[j]>x_buf[i])
			{
				temp=x_buf[j];
				x_buf[j]=x_buf[i];
				x_buf[i]=temp;
			}
			if(y_buf[j]>y_buf[i])
			{
				temp=y_buf[j];
				y_buf[j]=y_buf[i];
				y_buf[i]=temp;
			}
			if(z_buf[j]>z_buf[i])
			{
				temp=z_buf[j];
				z_buf[j]=z_buf[i];
				z_buf[i]=temp;
			}
		}
	}
	temp2=0;
	for(i=2;i<8;i++)
	{
		temp2=temp2+x_buf[i];
	}
	accData[0]=temp2/6;
	
	temp2=0;
	for(i=2;i<8;i++)
	{
		temp2=temp2+y_buf[i];
	}
	accData[1]=temp2/6;
	
	temp2=0;
	for(i=2;i<8;i++)
	{
		temp2=temp2+z_buf[i];
	}
	accData[2]=temp2/6;
}



//得到角度
//x,y,z:x,y,z方向的重力加速度分量(不需要单位,直接数值即可)
//dir:要获得的角度.0,与x轴的角度;1,与Y轴的角度;2,与Z轴的角度.
//返回值:角度值.单位0.1°.
float mpu6050_Angle(u8 dir)
{
	float temp,x,y,z;
 	float res=0;
        
        MPU6050ReadAcc();
        
        x = accData[0];
        y = accData[1];
        z = accData[2];
        
	switch(dir)
	{
		case 0://与自然Z轴的角度
 			temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://与自然X轴的角度
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://与自然Y轴的角度
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return (res*180/3.14);
}

