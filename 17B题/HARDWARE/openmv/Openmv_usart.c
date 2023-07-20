#include "OPENMV_USART.H"

/**
 * @brief Openmv��ʼ��
 * @param  ������
 * @retval  ��
 */
void Openmv_usart_init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴��� GPIO ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// �򿪴��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// �� USART Tx �� GPIO ����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// �� USART Rx �� GPIO ����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = bound;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART3, &USART_InitStructure);

	/* ���� USART Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/* �������ȼ�Ϊ 1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ�Ϊ 1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������ NVIC */
	NVIC_Init(&NVIC_InitStructure);

	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
}

reciveData ball = {0};

int openmv[5];

/**
 * @brief ȥ��֡ͷ֡β��ȡ��Чƫ��ֵ 0xff 0xef data1 data2 0xfe
 * @param  ��
 * @retval  ��
 */
void Get_openmvData(void)
{
	ball.cx = openmv[2];
	ball.cy = openmv[3];
}

/**
 * @brief ���� Openmv ���ݰ�
 * @param  ��
 * @retval  ��
 */
void USART3_IRQHandler(void)
{
	static int flag = 0;
	static int index = 0;
	uint8_t data;
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART3);
		if(data == 0xFF && flag == 0) //����֡ͷ
		{
			flag = 1;
			openmv[0] = 0xFF;
		}
		else if(data == 0xEF && flag == 1) //������֤֡ͷ
		{
			flag = 2;
			openmv[1] = 0xEF;
		}
		else if(flag == 2) //��ʼ������������
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
			if(data == 0xFE) //�ж��Ƿ�Ϊ֡β
			{
				flag = 0;
				openmv[4] = 0xFE;
				Get_openmvData();
			}
			else if(data != 0xFE) //����֡β ������� ��������
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

