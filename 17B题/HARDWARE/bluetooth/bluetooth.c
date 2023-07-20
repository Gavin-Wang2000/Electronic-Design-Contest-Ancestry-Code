#include "bluetooth.h"
/**
 * @brief Bluetooth初始化
 * @param  波特率
 * @retval  无
 */
void Buletooth_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口 GPIO 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	// 将 USART Tx 的 GPIO 配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 将 USART Rx 的 GPIO 配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = bound;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(UART4, &USART_InitStructure);

	/* 配置 USART 为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* 抢断优先级为 1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 子优先级为 1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置 NVIC */
	NVIC_Init(&NVIC_InitStructure);

	// 使能串口接收中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	// 使能串口
	USART_Cmd(UART4, ENABLE);
}

bluetoothData ballMove = {0};
int blueTooth[4];

/**
 * @brief 去除帧头帧尾获取有效偏移值
 * @param  无
 * @retval  无
 */
void Get_bluetoothData(void)
{
	ballMove.mode = blueTooth[2];
}

/**
 * @brief 接收 bluetooth 数据包(0xff 0xef data 0xfe)
 * @param  无
 * @retval  无
 */
void UART4_IRQHandler(void)
{
	static int flag = 0;
	static int index = 0;
	uint8_t data;
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(UART4);
		if (data == 0xFF && flag == 0) //接收帧头
		{
			flag = 1;
			blueTooth[0] = 0xFF;
		}
		else if (data == 0xEF && flag == 1) //二次验证帧头
		{
			flag = 2;
			blueTooth[1] = 0xEF;
		}
		else if (flag == 2) //开始接收有用数据
		{
			flag = 3;
			blueTooth[2] = data;
		}
		else if (flag == 3)
		{
			if (data == 0xFE) //判断是否为帧尾
			{
				flag = 0;
				blueTooth[3] = 0xFE;
				Get_bluetoothData();
			}
			else if (data != 0xFE) //不是帧尾 清除数据 重新来过
			{
				flag = 0;
				for (int i = 0; i < 4; i++)
				{
					blueTooth[i] = 0x00;
				}
			}
		}
		else
		{
			flag = 0;
			for (int i = 0; i < 4; i++)
			{
				blueTooth[i] = 0x00;
			}
		}
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}