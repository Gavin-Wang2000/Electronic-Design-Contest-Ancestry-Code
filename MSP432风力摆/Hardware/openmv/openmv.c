#include "openmv.h"

/**
 * @brief openmv初始化
 * @param None
 * @retval None
 * @note 波特率-115200
 */
void openmv_init(void)
{
	eUSCI_UART_Config uartConfig;
	
	
	//配置GPIO复用
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9,GPIO_PIN6|GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
	
	
	//配置结构体
	uartConfig.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
	uartConfig.clockPrescalar = 26;
	uartConfig.firstModReg = 0;
	uartConfig.secondModReg = 111;
	uartConfig.overSampling = 1;
	uartConfig.parity = EUSCI_A_UART_NO_PARITY;
	uartConfig.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
	uartConfig.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	uartConfig.uartMode = EUSCI_A_UART_MODE;
	
	//优先级最高
	Interrupt_setPriority(INT_EUSCIA3,0<<5);
	
	//初始化串口
	UART_initModule(EUSCI_A3_BASE, &uartConfig);
	
	
	//开启串口
	UART_enableModule(EUSCI_A3_BASE);
	
	
	//开启串口相关中断
	UART_enableInterrupt(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	
	
	//开启串口端口中断
	Interrupt_enableInterrupt(INT_EUSCIA3);
	

	//开启总中断
	Interrupt_enableMaster();
}


reciveData storage;

/**
 * @brief 串口中断服务函数(0x5b 0x5b trace crossCount 0x3c)
 * @param None
 * @retval None
 */
void EUSCIA3_IRQHandler(void)
{
    uint32_t status;
	uint8_t data;
	uint8_t i;
	static int flag = 0; 
	static uint8_t openmv[5];
	static uint8_t dataIndex = 0;
	

    UART_clearInterruptFlag(EUSCI_A3_BASE, status);
	
	status = UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
		data = UART_receiveData(EUSCI_A3_BASE);
		if (data == 0x5b && flag == 0) //接收第一个帧头
		{
			flag = 1;
			openmv[dataIndex++] = data;
		}
		else if (data == 0x5b && flag == 1)  //接收第二个帧头
		{ 
			flag = 2;
			openmv[dataIndex++] = data;
		}
		else if(flag == 2)//接收第一个数据
		{
			openmv[dataIndex++] = data;
			if(dataIndex >= 4)
			{
				flag = 3;
			}
		}
		else if (flag == 3)
		{
			if (data == 0x3c) //判断帧尾
			{
				flag = 0;
				openmv[dataIndex++] = data;
				storage.trace = openmv[2];
				storage.crossCount = openmv[3];
			}
			else if (data != 0x3c) //不是帧尾 清除数据 重新来过
			{
				flag = 0;
				for (i = 0; i <= 5; i++)
				{
					openmv[i] = 0x00;
				}
			}
		}
		else
		{
			flag = 0;
			for (i = 0; i <= 5; i++)
			{
				openmv[i] = 0x00;
			}
		}
   }
}





