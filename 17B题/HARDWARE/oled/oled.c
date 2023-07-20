#include "oled.h"
#include "oledfont.h"
#include "stdarg.h"
#include "stdio.h"
#include "delay.h"


/**
 * @brief ���ų�ʼ��
 * @param None
 * @retval None
 */
void OLED_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}



/**
 * @brief I2C��ʼ
 * @param None
 * @retval None
 */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}



/**
 * @brief I2Cֹͣ
 * @param None
 * @retval None
 */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}



/**
 * @brief I2C����һ���ֽ�
 * @param Byte Ҫ���͵�һ���ֽ�
 * @retval None
 */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//�����һ��ʱ�ӣ�������Ӧ���ź�
	OLED_W_SCL(0);
}



/**
 * @brief OLEDд����
 * @param Command Ҫд�������
 * @retval None
 */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}



/**
 * @brief OLEDд����
 * @param Data Ҫд�������
 * @retval None
 */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x40);		//д����
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}






/**
 * @brief OLED���ù��λ��
 * @param Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
 * @param X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
 * @retval None
 */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�õ�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�ø�4λ
}



/**
 * @brief OLED����
 * @param None
 * @retval None
 */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}


/**
 * @brief ��ʾ����
 * @param row ��λ�� ��Χ 1-4
 * @param col ��λ�� ��Χ 1-16
 * @param gbk���ִ洢���׵�ַ
 * @retval None
 */
int showGBK(unsigned char row,unsigned char col,uint8_t *gbk)
{
	int i,j;	
	for(i=0;i<2;i++)
	{
		OLED_SetCursor((row-1)*2+i,(col-1)*16);
		for(j=0;j<16;j++)
		{
			OLED_WriteData(gbk[i*16+j]);
		}
	}
	return 0;
}



/**
 * @brief OLED��ʾһ���ַ�
 * @param row ��λ�ã���Χ��1~4
 * @param col ��λ�ã���Χ��1~16
 * @param Chr Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
 * @param inv �ڰ׷���,0Ϊ�����ԣ�1Ϊ����
 * @retval None
 */
void OLED_ShowChar(uint8_t row,uint8_t col,char Chr,uint8_t inv)
{      	
	uint8_t i;
	//���ù��λ�����ϰ벿��
	OLED_SetCursor((row - 1) * 2, (col - 1) * 8);	
	for (i = 0; i < 8; i++)
	{
		//��ʾ�ϰ벿������
		if(inv)
		{
			OLED_WriteData(~OLED_F8x16[Chr - ' '][i]);
		}
		else
		{
			OLED_WriteData(OLED_F8x16[Chr - ' '][i]);
		}
	}
	//���ù��λ�����°벿��
	OLED_SetCursor((row - 1) * 2 + 1, (col - 1) * 8);
	for (i = 0; i < 8; i++)
	{
		//��ʾ�°벿������
		if(inv)
		{
			OLED_WriteData(~OLED_F8x16[Chr - ' '][i + 8]);
		}
		else 
		{
			OLED_WriteData(OLED_F8x16[Chr - ' '][i + 8]);
		}
	}
}



/**
 * @brief OLED��ʾ�ַ���
 * @param row ��ʼ��λ�ã���Χ��1~4
 * @param col ��ʼ��λ�ã���Χ��1~16
 * @param str Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
 * @param inv �ڰ׷���,0Ϊ�����ԣ�1Ϊ����
 * @retval None
 */
void OLED_ShowString(uint8_t row,uint8_t col,char *str,uint8_t inv)
{
	uint8_t i;
	for (i = 0; str[i] != '\0'; i++)
	{
		OLED_ShowChar(row, col + i,str[i],inv);
	}
}



/**
 * @brief OLED��ʼ��
 * @param None
 * @retval None
 */
void OLED_Init(void)
{	
	/*�˿ڳ�ʼ��*/
	OLED_I2C_Init();
	
	delay_ms(100);
	OLED_WriteCommand(0xAE);	//--display off
	OLED_WriteCommand(0x00);	//---set low column address
	OLED_WriteCommand(0x10);	//---set high column address
	OLED_WriteCommand(0x40);	//--set start line address  
	OLED_WriteCommand(0xB0);	//--set page address
	OLED_WriteCommand(0x81); 	// contract control
	OLED_WriteCommand(0xFF);	//--128   
	OLED_WriteCommand(0xA1);	//set segment remap 
	OLED_WriteCommand(0xA6);	//0xA6--normal / 0xA7--reverse
	OLED_WriteCommand(0xA8);	//--set multiplex ratio(1 to 64)
	OLED_WriteCommand(0x3F);	//--1/32 duty
	OLED_WriteCommand(0xC8);	//Com scan direction
	OLED_WriteCommand(0xD3);	//-set display offset
	OLED_WriteCommand(0x00);	//
	
	OLED_WriteCommand(0xD5);	//set osc division
	OLED_WriteCommand(0x80);	//
	
	OLED_WriteCommand(0xD8);	//set area color mode off
	OLED_WriteCommand(0x05);	//
	
	OLED_WriteCommand(0xD9);	//Set Pre-Charge Period
	OLED_WriteCommand(0xF1);	//
	
	OLED_WriteCommand(0xDA);	//set com pin configuartion
	OLED_WriteCommand(0x12);	//
	
	OLED_WriteCommand(0xDB);	//set Vcomh
	OLED_WriteCommand(0x30);	//
	
	OLED_WriteCommand(0x20);	//set memory addressing mode
	OLED_WriteCommand(0x02);	//page addressing mode
	
	OLED_WriteCommand(0x8D);	//set charge pump enable
	OLED_WriteCommand(0x14);	//
	
	OLED_WriteCommand(0xAF);	//--turn on oled panel
		
	OLED_Clear();				//clear
}



/**
 * @brief ��ӡ
 * @param row ��(��Ŵ�1��ʼ)
 * @param col ��(��Ŵ�1��ʼ)
 * @param formate ���򲻺�����ռλ�����ַ���
 * @param ...��������
 * @retval vsprintf��״̬
 */
int oledPrint(u8 row,u8 col,const char *formate,...)
{
	char buf[32]={'\0'};
	int ret=0;
	va_list ap;
	va_start(ap,formate);
	ret = vsprintf(buf,formate,ap);
	va_end(ap);
	OLED_ShowString(row,col,buf,0);
	return ret;
}



/**
 * @brief ��������ʾ
 * @note �����Ͻ�Ϊԭ��
 * @param page �ڼ�ҳ 
	ȡֵ��Χ 0-7
 * @param percent �ٷ���
	ȡֵ��Χ 0-100
 * @retval ״̬��
	����0��ʾ����
	����Ϊ�쳣
 */
int loadbar(u8 page,u8 percent)
{
	unsigned char i;
	
	if(page > 7)
		return -1;
	if(percent > 100)
		return -3;
	
	OLED_SetCursor(page,0);
	
	for(i=0;i<100;i++)
	{	
		if(i > percent)
		{
			//�������
			OLED_WriteData(0X81);	
		}
		else
		{
			//�����
			OLED_WriteData(0XFF);
		}
	}	
	//����
	OLED_WriteData(0XFF);
	
	return 0;
}


