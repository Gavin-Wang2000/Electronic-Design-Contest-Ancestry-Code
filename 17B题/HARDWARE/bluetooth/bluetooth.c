#include "bluetooth.h"
/**
 * @brief Bluetooth��ʼ��
 * @param  ������
 * @retval  ��
 */
void Buletooth_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴��� GPIO ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// �򿪴��������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	// �� USART Tx �� GPIO ����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// �� USART Rx �� GPIO ����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

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
	USART_Init(UART4, &USART_InitStructure);

	/* ���� USART Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* �������ȼ�Ϊ 1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ�Ϊ 1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������ NVIC */
	NVIC_Init(&NVIC_InitStructure);

	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(UART4, ENABLE);
}

bluetoothData ballMove = {0};
int blueTooth[4];

/**
 * @brief ȥ��֡ͷ֡β��ȡ��Чƫ��ֵ
 * @param  ��
 * @retval  ��
 */
void Get_bluetoothData(void)
{
	ballMove.mode = blueTooth[2];
}

/**
 * @brief ���� bluetooth ���ݰ�(0xff 0xef data 0xfe)
 * @param  ��
 * @retval  ��
 */
void UART4_IRQHandler(void)
{
	static int flag = 0;
	static int index = 0;
	uint8_t data;
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(UART4);
		if (data == 0xFF && flag == 0) //����֡ͷ
		{
			flag = 1;
			blueTooth[0] = 0xFF;
		}
		else if (data == 0xEF && flag == 1) //������֤֡ͷ
		{
			flag = 2;
			blueTooth[1] = 0xEF;
		}
		else if (flag == 2) //��ʼ������������
		{
			flag = 3;
			blueTooth[2] = data;
		}
		else if (flag == 3)
		{
			if (data == 0xFE) //�ж��Ƿ�Ϊ֡β
			{
				flag = 0;
				blueTooth[3] = 0xFE;
				Get_bluetoothData();
			}
			else if (data != 0xFE) //����֡β ������� ��������
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