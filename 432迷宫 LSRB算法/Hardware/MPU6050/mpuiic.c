#include "mpuiic.h"


/**
 * @brief I2C引脚初始化
 * @param None
 * @retval None
 */
 
void IIC_Init(void)
{			
    GPIO_setAsOutputPin(SCL_PORT,SCL_PIN ); //SCL     
    GPIO_setAsOutputPin(SDA_PORT,SDA_PIN);//SDA
	
	IIC_SCL_High();
	IIC_SDA_High();
}


/**
 * @brief IIC起始信号
 * @param None
 * @retval None
 */

void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SCL_High();
	IIC_SDA_High();
	delay_us(4);
 	IIC_SDA_Low();//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_Low();//钳住I2C总线，准备发送或接收数据 
}

/**
 * @brief IIC停止信号
 * @param None
 * @retval None
 */

void IIC_Stop(void)//SDA 01 SCL 01
{
	SDA_OUT();//sda线输出
	IIC_SCL_Low();//STOP:when CLK is high DATA change form low to high
    IIC_SDA_Low();
 	delay_us(4);
    IIC_SCL_High();
	IIC_SDA_High();//发送I2C总线结束信号
	delay_us(4);							   	
}


/**
 * @brief 等待应答信号
 * @param None
 * @retval 返回0成功,1失败
 */
uint8_t IIC_Wait_Ack(void)
{
	uint8_t cy;
    SDA_IN();      //SDA设置为输入
    IIC_SCL_High();delay_us(10);
	IIC_SDA_High();delay_us(10);
    if(READ_SDA)
    {
        cy=1;
        IIC_SCL_Low();
        return cy; 
    }      
    else
    {
        cy=0;
    }             
	IIC_SCL_Low();//时钟输出0
	return cy;  
} 



/**
 * @brief IIC产生应答
 * @param None
 * @retval None
 */
void IIC_Ack(void)	
{
	IIC_SCL_Low();
	SDA_OUT();
    IIC_SDA_Low();
	delay_us(2);
    IIC_SCL_High();
    delay_us(2);
	IIC_SCL_Low();
}



/**
 * @brief 产生非应答
 * @param None
 * @retval None
 */
void IIC_NAck(void)	//产生非应答
{
	IIC_SCL_Low();
	SDA_OUT();
    IIC_SDA_High();
	delay_us(2);
    IIC_SCL_High();
    delay_us(2);
	IIC_SCL_Low();
}					 		


/**
 * @brief 发送一个字节
 * @param txd 要发送的字节
 * @retval None
 */
void IIC_Send_Byte(uint8_t txd)	//发送一个字节
{                        
    uint8_t t;   
	SDA_OUT(); 
    IIC_SCL_Low();//拉低时钟开始数据传输
    delay_us(2);
    for(t=0;t<8;t++)
    {       
        if(txd&0x80)
        {
            IIC_SDA_High();delay_us(2); 
        }  
        else
        {
            IIC_SDA_Low();delay_us(2);  
        }
        txd<<=1;
		IIC_SCL_High();
		delay_us(4); 
        IIC_SCL_Low();
        delay_us(2); 
    }
        delay_us(2);
}



/**
 * @brief 读取一个字节
 * @param ack 1发送应答,0发送非应答
 * @retval 读取的字节
 */
uint8_t IIC_Read_Byte(uint8_t ack)	
{
	uint8_t i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
		IIC_SCL_Low();
        delay_us(2);
        IIC_SCL_High();
        receive<<=1;
        if(READ_SDA)
            receive++;
        delay_us(2);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
