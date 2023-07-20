#ifndef __MPUI2C_H
#define __MPUI2C_H
#include "main.h" 
#include "stdio.h"

//��������
#define MPU_SCL_PORT 	GPIO_PORT_P6
#define MPU_SCL_PIN		GPIO_PIN5
#define MPU_SDA_PORT	GPIO_PORT_P4
#define MPU_SDA_PIN		GPIO_PIN6

//IO��������
#define SDA_IN()  GPIO_setAsInputPin(MPU_SDA_PORT,MPU_SDA_PIN)	//SDA����ģʽ
#define SDA_OUT() GPIO_setAsOutputPin(MPU_SDA_PORT,MPU_SDA_PIN) //SDA���ģʽ

//IO��������	 
#define IIC_SCL_High()  GPIO_setOutputHighOnPin(MPU_SCL_PORT,MPU_SCL_PIN) //SCL_High
#define IIC_SCL_Low()   GPIO_setOutputLowOnPin(MPU_SCL_PORT,MPU_SCL_PIN) //SCL_Low
#define IIC_SDA_High()  GPIO_setOutputHighOnPin(MPU_SDA_PORT,MPU_SDA_PIN) //SDA_High
#define IIC_SDA_Low()   GPIO_setOutputLowOnPin(MPU_SDA_PORT,MPU_SDA_PIN) //SDA_Low
#define READ_SDA        GPIO_getInputPinValue(MPU_SDA_PORT,MPU_SDA_PIN)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















