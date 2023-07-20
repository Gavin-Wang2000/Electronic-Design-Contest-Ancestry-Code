#include "OPENMV_USART.H"

/**
 * @brief Openmv初始化
 * @param  波特率
 * @retval  无
 */
void Openmv_usart_init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口 GPIO 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// 将 USART Tx 的 GPIO 配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 将 USART Rx 的 GPIO 配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

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
	USART_Init(USART3, &USART_InitStructure);

	/* 配置 USART 为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/* 抢断优先级为 1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 子优先级为 1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置 NVIC */
	NVIC_Init(&NVIC_InitStructure);

	// 使能串口接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	// 使能串口
	USART_Cmd(USART3, ENABLE);
}

reciveData ball = {0};

int openmv[5];

/**
 * @brief 去除帧头帧尾获取有效偏移值 0xff 0xef data1 data2 0xfe
 * @param  无
 * @retval  无
 */
void Get_openmvData(void)
{
	ball.cx = openmv[2];
	ball.cy = openmv[3];
}

/**
 * @brief 接收 Openmv 数据包
 * @param  无
 * @retval  无
 */
void USART3_IRQHandler(void)
{
	static int flag = 0;
	static int index = 0;
	uint8_t data;
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART3);
		if(data == 0xFF && flag == 0) //接收帧头
		{
			flag = 1;
			openmv[0] = 0xFF;
		}
		else if(data == 0xEF && flag == 1) //二次验证帧头
		{
			flag = 2;
			openmv[1] = 0xEF;
		}
		else if(flag == 2) //开始接收有用数据
		{
			flag = 3;
			openmv[2] = data;
		}
		else if(flag == 3)
		{
			flag = 4;
			openmv[3] = data;
		}
		else if(flag == 4)
		{
			if(data == 0xFE) //判断是否为帧尾
			{
				flag = 0;
				openmv[4] = 0xFE;
				Get_openmvData();
			}
			else if(data != 0xFE) //不是帧尾 清除数据 重新来过
			{
				flag = 0;
				for(int i = 0; i < 5; i++)
				{
					openmv[i] = 0x00;
				}
			}
		}
		else
		{
			flag = 0;
			for(int i = 0; i < 5; i++)
			{
				openmv[i] = 0x00;
			}
		}
	    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

