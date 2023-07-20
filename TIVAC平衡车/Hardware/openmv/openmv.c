#include "openmv.h"

int fputc(int ch, FILE *f){UARTCharPut(UART0_BASE,ch);	return (ch);}

/**
 * @brief openmv���ڳ�ʼ��
 * @param  ������
 * @retval  None
 **/
void openmvUsart_init(int baud)
{
	//ʹ��UARTʹ�õ�GPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//ʹ�ܴ���0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	//������������
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	//����ʱ��16MHZ
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    //UART��ţ������ʣ�UARTʱ��Ƶ��
    UARTStdioConfig(0, baud, 16000000);
	 //ʹ��FIFO
	UARTFIFOEnable(UART0_BASE);
    //���շ��͵�FIFOΪ1���ֽ�
	UARTFIFOLevelSet(UART0_BASE,1,1);
	//ʹ�ܴ��ڽ��պ���
	UARTIntEnable(UART0_BASE,UART_INT_RX);
	//ע���жϺ���
	UARTIntRegister(UART0_BASE,USART0_IRQHandler);
	//�����ж����ȼ�
	IntPrioritySet(INT_UART0,0);
    //�����ж�
	IntEnable(INT_UART0);
	IntMasterEnable();
    //ʹ�ܴ���
    UARTEnable(UART0_BASE);
}

openmvData trace = {0};

int openmv[5];

/**
 * @brief ȥ��֡ͷ֡β��ȡ��Чƫ��ֵ 0x5b 0x5b data1 data2 0x3c
 * @param  ��
 * @retval  ��
 */
void Get_openmvData(void)
{
	trace.cx = openmv[2];
	trace.cy = openmv[3];
}

/**
 * @brief ����0�жϷ�����
 * @param  None
 * @retval  None
 **/
void USART0_IRQHandler(void)
{
	static int flag = 0;
	uint8_t data;
    //��ȡ�ж�״̬
    uint32_t status=UARTIntStatus(UART0_BASE, true);
    //����жϱ�־λ
	UARTIntClear(UART0_BASE, status);
    //�ж�UART0��û���ַ�δ��ȡ
	while(UARTCharsAvail(UART0_BASE))
	{
        //������ַ�Ϊ��ȡ��ȡ����ʹ��UARTCharGetNonBlocking��ֹ�ȴ�
	    data = (uint8_t)UARTCharGetNonBlocking(UART0_BASE);
		if(data == 0x5b && flag == 0) //����֡ͷ
		{
			flag = 1;
			openmv[0] = 0x5b;
		}
		else if(data == 0x5b && flag == 1) //������֤֡ͷ
		{
			flag = 2;
			openmv[1] = 0x5b;
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
			if(data == 0x3c) //�ж��Ƿ�Ϊ֡β
			{
				flag = 0;
				openmv[4] = 0x3c;
				Get_openmvData();
			}
			else if(data != 0x3c) //����֡β ������� ��������
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
        
	}
}