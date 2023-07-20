#ifndef __MPUI2C_H
#define __MPUI2C_H
#include "main.h" 
#include "stdio.h"

//引脚配置
#define MPU_SCL_PORT 	GPIO_PORT_P6
#define MPU_SCL_PIN		GPIO_PIN5
#define MPU_SDA_PORT	GPIO_PORT_P4
#define MPU_SDA_PIN		GPIO_PIN6

//IO方向设置
#define SDA_IN()  GPIO_setAsInputPin(MPU_SDA_PORT,MPU_SDA_PIN)	//SDA输入模式
#define SDA_OUT() GPIO_setAsOutputPin(MPU_SDA_PORT,MPU_SDA_PIN) //SDA输出模式

//IO操作函数	 
#define IIC_SCL_High()  GPIO_setOutputHighOnPin(MPU_SCL_PORT,MPU_SCL_PIN) //SCL_High
#define IIC_SCL_Low()   GPIO_setOutputLowOnPin(MPU_SCL_PORT,MPU_SCL_PIN) //SCL_Low
#define IIC_SDA_High()  GPIO_setOutputHighOnPin(MPU_SDA_PORT,MPU_SDA_PIN) //SDA_High
#define IIC_SDA_Low()   GPIO_setOutputLowOnPin(MPU_SDA_PORT,MPU_SDA_PIN) //SDA_Low
#define READ_SDA        GPIO_getInputPinValue(MPU_SDA_PORT,MPU_SDA_PIN)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















