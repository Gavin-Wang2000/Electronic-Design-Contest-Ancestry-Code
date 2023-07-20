#include "oled.h"
#include "oledfont.h"
#include "stdarg.h"
#include "stdio.h"



/**
 * @brief 引脚初始化
 * @param None
 * @retval None
 */
void OLED_I2C_Init(void)
{
	//SCL
	GPIO_setAsOutputPin(SCL_PORT, SCL_PIN);
	GPIO_setOutputHighOnPin(SCL_PORT, SCL_PIN);

	//SDA
	GPIO_setAsOutputPin(SDA_PORT, SDA_PIN);
	GPIO_setOutputHighOnPin(SDA_PORT, SDA_PIN);
}



/**
 * @brief I2C开始
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
 * @brief I2C停止
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
 * @brief I2C发送一个字节
 * @param Byte 要发送的一个字节
 * @retval None
 */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA((Byte & (0x80 >> i)));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}



/**
 * @brief OLED写命令
 * @param Command 要写入的命令
 * @retval None
 */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}



/**
 * @brief OLED写数据
 * @param Data 要写入的数据
 * @retval None
 */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}



/**
 * @brief OLED设置光标位置
 * @param Y 以左上角为原点，向下方向的坐标，范围：0~7
 * @param X 以左上角为原点，向右方向的坐标，范围：0~127
 * @retval None
 */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置高4位
}



/**
 * @brief OLED清屏
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
 * @brief 显示汉字
 * @param row 行位置 范围 1-4
 * @param col 列位置 范围 1-16
 * @param gbk汉字存储的首地址
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
 * @brief OLED显示一个字符
 * @param row 行位置，范围：1~4
 * @param col 列位置，范围：1~16
 * @param Chr 要显示的一个字符，范围：ASCII可见字符
 * @param inv 黑白反显,0为不反显，1为反显
 * @retval None
 */
void OLED_ShowChar(uint8_t row,uint8_t col,char Chr,uint8_t inv)
{      	
	uint8_t i;
	//设置光标位置在上半部分
	OLED_SetCursor((row - 1) * 2, (col - 1) * 8);	
	for (i = 0; i < 8; i++)
	{
		//显示上半部分内容
		if(inv)
		{
			OLED_WriteData(~OLED_F8x16[Chr - ' '][i]);
		}
		else
		{
			OLED_WriteData(OLED_F8x16[Chr - ' '][i]);
		}
	}
	//设置光标位置在下半部分
	OLED_SetCursor((row - 1) * 2 + 1, (col - 1) * 8);
	for (i = 0; i < 8; i++)
	{
		//显示下半部分内容
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
 * @brief OLED显示字符串
 * @param row 起始行位置，范围：1~4
 * @param col 起始列位置，范围：1~16
 * @param str 要显示的字符串，范围：ASCII可见字符
 * @param inv 黑白反显,0为不反显，1为反显
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
 * @brief OLED初始化
 * @param None
 * @retval None
 */
void OLED_Init(void)
{	
	/*端口初始化*/
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
 * @brief 打印
 * @param row 行(序号从1开始)
 * @param col 列(序号从1开始)
 * @param formate 含或不含有有占位符的字符串
 * @param ...不定参数
 * @retval vsprintf的状态
 */
int OLED_Print(uint8_t row,uint8_t col,const char *formate,...)
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
 * @brief 进度条显示
 * @note 以左上角为原点
 * @param page 第几页 
	取值范围 0-7
 * @param percent 百分数
	取值范围 0-100
 * @retval 状态量
	返回0表示正常
	其他为异常
 */
int OLED_loadbar(uint8_t page,uint8_t percent)
{
	unsigned char i;
	
	if(page > 7)
		return -1;
	if(percent > 100)
		return -3;
	
	OLED_SetCursor(page,13);
	
	for(i=0;i<100;i++)
	{	
		if(i > percent)
		{
			//非填充区
			OLED_WriteData(0X81);	
		}
		else
		{
			//填充区
			OLED_WriteData(0XFF);
		}
	}	
	//底栏
	OLED_WriteData(0XFF);
	
	return 0;
}




/**
 * @brief 显示汉字
 * @param row 行位置 范围 1-4
 * @param col 列位置 范围 1-16
 * @param gbk汉字存储的首地址
 * @retval None
 */
int showCHN(unsigned char row,unsigned char col,uint8_t *chn)
{
	int i,j;	
	for(i=0;i<2;i++)
	{
		OLED_SetCursor((row-1)*2+i,(col-1)*16);
		for(j=0;j<16;j++)
		{
			OLED_WriteData(chn[i*16+j]);
		}
	}
	return 0;
}
/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+',0);
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-',0);
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0',0);
	}
}



