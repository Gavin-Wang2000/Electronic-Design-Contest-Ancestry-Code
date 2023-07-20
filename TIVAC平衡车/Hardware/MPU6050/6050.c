/*
 * 6050.c
 *
 *  Created on: 2020年5月10日
 *      Author: jzk
 */
#include "6050.h"


void Init6050()
{
    int res;
    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_PWR_MGMT1_REG, 0x80);//复位6050
    SysCtlDelay(100*(SysCtlClockGet()/3000));
    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_PWR_MGMT1_REG, 0x00);//唤醒6050

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_GYRO_CFG_REG, 3<<3);//陀螺仪传感器,±2000dps

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_ACCEL_CFG_REG, 0<<3);//加速度传感器,±2g

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_SAMPLE_RATE_REG, 19);//设置采样率50Hz

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_CFG_REG, 6);//自动设置LPF为采样率的一半

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭所有中断

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_USER_CTRL_REG, 0x00);//I2C主模式关闭

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_FIFO_EN_REG, 0x00);//关闭FIFO

    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INTBP_CFG_REG, 0x80);//INT引脚低电平有效

    res=Read_addr();
    UARTprintf("addr_read:   0x%x \n",res);
    if(res==MPU_ADDR)
    {
        I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_PWR_MGMT1_REG, 0x01);//设置CLKSEL,PLL X轴为参考
        I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_PWR_MGMT2_REG, 0x00);//加速度与陀螺仪都工作
        I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_SAMPLE_RATE_REG, 19);//设置采样率50Hz
        I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_CFG_REG, 6);//自动设置LPF为采样率的一半
        UARTprintf("\nmpu6050------init_finish\n");
    }
    else
    {
        UARTprintf("\nmpu6050------addr_error\n");
    }

//    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭所有中断
//    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭所有中断
//    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭所有中断
//    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭所有中断
//    I2CWriteRegOneByte(MPU6050_IIC_BASE,MPU_ADDR, MPU_INT_EN_REG, 0x00);//关闭所有中断
}

uint8_t Read_addr()
{
    uint8_t a[2]={0};
    //I2CReadRegOneByte(MPU6050_IIC_BASE, MPU_ADDR,
      //                MPU_DEVICE_ID_REG, a);
    MPU_Read_Len(MPU_ADDR,MPU_DEVICE_ID_REG, 1,a);
    //I2CReadRegMultiByte(MPU6050_IIC_BASE, MPU_ADDR,
                        //MPU_DEVICE_ID_REG,a, 1);
    //I2CReadRegTwoByte(MPU6050_IIC_BASE, MPU_ADDR,
                                //    MPU_DEVICE_ID_REG, a);
     return a[0];
}

//void delay
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
//       int i=0;
//       int m=0;
//       for(i=0;i<len;i++)
//       {
//         m+=I2CWriteRegOneByte(MPU6050_IIC_BASE, addr,
//                                                  reg, buf+i);
//       }
//       return m;
    if(len>1)
        return I2CWriteRegMultiByte(MPU6050_IIC_BASE,addr,reg, buf, len);
    else if(len==1)
        return I2CWriteRegOneByte(MPU6050_IIC_BASE, addr,reg, *buf);
    return 0;
}
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
//    int i=0;
//    int m=0;
//    for(i=0;i<len;i++)
//    {
//
//        m+=I2CReadRegOneByte(MPU6050_IIC_BASE, addr,
//                                               reg, buf+i);
//    }
//    return m;
    if(len>1)
        return I2CReadRegMultiByte(MPU6050_IIC_BASE, addr,reg,buf, len);
    else if(len==1)
        return I2CReadRegOneByte(MPU6050_IIC_BASE, addr,reg, buf);
    return 0;
}
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
   return I2CWriteRegOneByte(MPU6050_IIC_BASE, MPU_ADDR,reg, data);
}
uint8_t MPU_Read_Byte(uint8_t reg)

{
   uint8_t res=0;
   I2CReadRegOneByte(MPU6050_IIC_BASE, MPU_ADDR,
                     reg, &res);
    return res;
}

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Init(void)
{
    uint8_t res;
   // IIC_Init();//初始化IIC总线
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80); //复位MPU6050
    delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00); //唤醒MPU6050

    MPU_Set_Gyro_Fsr(3);                    //陀螺仪传感器,±2000dps
    MPU_Set_Accel_Fsr(0);                   //加速度传感器,±2g
    MPU_Set_Rate(50);                       //设置采样率50Hz

    MPU_Write_Byte(MPU_INT_EN_REG,0X00);    //关闭所有中断
    MPU_Write_Byte(MPU_USER_CTRL_REG,0X00); //I2C主模式关闭
    MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);   //关闭FIFO
    MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80); //INT引脚低电平有效
    res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
    if(res==MPU_ADDR)//器件ID正确
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01); //设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00); //加速度与陀螺仪都工作
        MPU_Set_Rate(50);                       //设置采样率为50Hz
    }else return 1;
    return 0;
}




//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);  //设置数字低通滤波器
    return MPU_Set_LPF(rate/2); //自动设置LPF为采样率的一半
}

uint16_t MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    short m_tmpValue;
    //short raw;
    double raw;
    float temp;
    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    //raw=((uint16_t)buf[0]<<8)|buf[1];
    m_tmpValue = buf[0];
    m_tmpValue = (m_tmpValue<<8)|buf[1];

    //temp=36.53+((double)raw)/340;
    raw = m_tmpValue/340;
    temp=36.53 + raw;

    m_tmpValue = temp*100;
    return m_tmpValue;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((uint16_t)buf[0]<<8)|buf[1];
        *gy=((uint16_t)buf[2]<<8)|buf[3];
        *gz=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;//res只是获取数据成功或失败的标志位

    /*
     * 加速度数据按 x轴 -> y轴 -> z轴 顺序发送
     * 每轴数据两字节，先发高八位 -> 再发低八位
     * ax\ay\az 都是 short类型数据，占两个字节
     * 原始数据是和温度一样
     */
    res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;
}



