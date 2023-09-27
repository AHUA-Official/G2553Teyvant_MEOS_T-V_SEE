/*
***************************************************************************
**�ļ�����Mpu-6050.c
**��  ����������ٶȣ����������Ǵ�����Mpu-6050���������򣬴˴�����149ϵ�С�
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
//I2C��ʼ�ź�
//**************************************
void I2C_Start()
{
    MPU_SCL_OUT(); // SCL����Ϊ���
    MPU_SDA_OUT(); // SDA����Ϊ���
    
    MPU_SDA_H();   //����������
    MPU_SCL_H();   //����ʱ����
    DELAY_US(5);   //��ʱ
    MPU_SDA_L();   //�����½���
    DELAY_US(5);   //��ʱ
    MPU_SCL_L();   //����ʱ����
}
//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop()
{
    MPU_SCL_OUT(); // SCL����Ϊ���
    MPU_SDA_OUT(); // SDA����Ϊ���
    
    MPU_SDA_L();   //����������
    MPU_SCL_H();   //����ʱ����
    DELAY_US(5);   //��ʱ
    MPU_SDA_H();   //����������
    DELAY_US(5);   //��ʱ
}
//**************************************
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uchar ack)
{
    MPU_SCL_OUT(); // SCL����Ϊ���
    MPU_SDA_OUT(); // SDA����Ϊ���
    
    if(ack) 
        MPU_SDA_H();
    else
        MPU_SDA_L();
//    SDA = ack;                  //дӦ���ź�
    MPU_SCL_H();                    //����ʱ����
    DELAY_US(5);                  //��ʱ
    MPU_SCL_L();                    //����ʱ����
    DELAY_US(5);                  //��ʱ
}
//**************************************
//I2C����Ӧ���ź�
//**************************************
uchar I2C_RecvACK()
{
    uchar cy;
    MPU_SCL_OUT(); // SCL����Ϊ���
    MPU_SDA_IN(); // SDA����Ϊ����
    
    MPU_SCL_H();                    //����ʱ����
    DELAY_US(5);                 //��ʱ
    if(MPU_SDA_DAT())
    {
        cy=1;
    }
    else 
    {
      cy=0;
    }
//    cy = SDA;                   //��Ӧ���ź�
    MPU_SCL_L();                    //����ʱ����
    DELAY_US(5);                //��ʱ
    MPU_SDA_OUT(); // SDA����Ϊ���
    
    return cy;
    
}
//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    MPU_SCL_OUT(); // SCL����Ϊ���
    MPU_SDA_OUT(); // SDA����Ϊ���   
    for (i=0; i<8; i++)         //8λ������
    {
        if((dat<<i)&0x80)
        {
            MPU_SDA_H();
        }
        else 
        {
            MPU_SDA_L();
        }
       // SDA = cy;               //�����ݿ�
        MPU_SCL_H();                //����ʱ����
        DELAY_US(5);              //��ʱ
        MPU_SCL_L();                //����ʱ����
        DELAY_US(5);              //��ʱ
    }
    I2C_RecvACK();
}
//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0,cy;
    MPU_SCL_OUT(); // SCL����Ϊ���
    MPU_SDA_OUT(); // SDA����Ϊ���
    
    MPU_SDA_H();  //ʹ���ڲ�����,׼����ȡ����,
    MPU_SDA_IN(); // SDA����Ϊ���룬׼����������������
    for (i=0; i<8; i++)         //8λ������
    {
       
        dat <<= 1;
        MPU_SCL_H();                //����ʱ����
        DELAY_US(5);             //��ʱ
        if(MPU_SDA_DAT()) 
        {
            cy=1;
        }
        else 
        {
          cy=0;
        }
        dat |= cy;             //������             
        MPU_SCL_L();                //����ʱ����
        DELAY_US(5);             //��ʱ
    }
    MPU_SDA_OUT();
    return dat;
}
//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void ByteWrite6050(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
uchar ByteRead6050(uchar REG_Address)
{
	uchar REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	I2C_SendACK(1);                //����Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}
//**************************************
//�ϳ�����
//**************************************
int Get6050Data(uchar REG_Address)
{
	char H,L;
	H=ByteRead6050(REG_Address);
	L=ByteRead6050(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}
//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU6050()
{
	ByteWrite6050(PWR_MGMT_1, 0x00);  // �������״̬
	ByteWrite6050(SMPLRT_DIV, 0x07);  // �����ǲ��������ã�125HZ��
	ByteWrite6050(CONFIG, 0x06);      // ��ͨ�˲���Ƶ�����ã�5HZ��
	ByteWrite6050(GYRO_CONFIG, 0x18); // �������Լ켰��ⷶΧ����(���Լ�,16.4LSB/DBS/S)
	ByteWrite6050(ACCEL_CONFIG, 0x01); // ���ټ��Լ졢������Χ����ͨ�˲�Ƶ��(���Լ죬2G(16384LSB/G)��5Hz)
}

/*
**********************************************
**������  ��float Mpu6050AccelAngle(int8 dir)
**�������ܣ�������ٶȴ��������������ֵ
**            ��ΧΪ2gʱ�������ϵ��16384 LSB/g
**            �ǶȽ�Сʱ��x=sinx�õ��Ƕȣ����ȣ�, deg = rad*180/3.14
**            ��Ϊx>=sinx,�ʳ���1.2�ʵ��Ŵ�
**���ز��������������ֵ
**���������dir - ��Ҫ�����ķ���
**           ACCEL_XOUT - X����
**           ACCEL_YOUT - Y����
**           ACCEL_ZOUT - Z����
**********************************************
*/
float Mpu6050AccelAngle(int8 dir)
{
    float accel_agle;// ���������ֵ
    float result; // ����ֵ�������
    result = (float)Get6050Data(dir); // ������ǰ����ļ��ٶ�ֵ,ת��Ϊ������
    accel_agle = (result + MPU6050_ZERO_ACCELL); // ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
//    accel_agle = accel_agle*1.2*180/3.14;     //����ת��Ϊ��
    
    return accel_agle; // ���ز���ֵ
}

/*
**********************************************
**������  ��float Mpu6050GyroAngle(int8 dir)
**�������ܣ���������ǲ�������Ǽ��ٶ�
**            ��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
**���ز�������������Ǽ��ٶ�ֵ
**���������dir - ��Ҫ�����ķ���
**           GYRO_XOUT - X�᷽��
**           GYRO_YOUT - Y�᷽��
**           GYRO_ZOUT - Z�᷽��
**********************************************
*/
float Mpu6050GyroAngle(int8 dir)
{
    float gyro_angle;
//    float Angle_gy;
    gyro_angle = (float)Get6050Data(dir);   // ��������ǵĵ�ǰֵ
    gyro_angle = -(gyro_angle + MPU6050_ZERO_GYRO)/16.4;    //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������ 
//    Angle_gy += gyro_angle*0.005;
    return gyro_angle; // ���ز���ֵ
}


//����10��ȥ�����������Сֵ��ƽ��
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



//�õ��Ƕ�
//x,y,z:x,y,z������������ٶȷ���(����Ҫ��λ,ֱ����ֵ����)
//dir:Ҫ��õĽǶ�.0,��x��ĽǶ�;1,��Y��ĽǶ�;2,��Z��ĽǶ�.
//����ֵ:�Ƕ�ֵ.��λ0.1��.
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
		case 0://����ȻZ��ĽǶ�
 			temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://����ȻX��ĽǶ�
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://����ȻY��ĽǶ�
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return (res*180/3.14);
}

