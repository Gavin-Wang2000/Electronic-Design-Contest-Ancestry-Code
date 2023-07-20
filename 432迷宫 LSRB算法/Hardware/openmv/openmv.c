#include "openmv.h"

/**
 * @brief openmv初始化
 * @param None
 * @retval None
 * @note 波特率-9600
 */
void openmv_init(void)
{
	eUSCI_UART_Config uartConfig;
	
	
	//配置GPIO复用
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9,GPIO_PIN6|GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
	
	
	//配置结构体
	uartConfig.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
	uartConfig.clockPrescalar = 312;
	uartConfig.firstModReg = 8;
	uartConfig.secondModReg = 0;
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

int openmv[4];//接收来自openmv的数据包
int8_t rho;

/**
 * @brief 去除帧头帧尾获取有效偏移值
 * @param  无
 * @retval  无
 */

void Get_ValueData(void)
{
	rho = openmv[2];
}



/**
 * @brief 串口中断服务函数(0xff 0xef data 0xfe)
 * @param None
 * @retval None
 */
void EUSCIA3_IRQHandler(void)
{
    uint32_t status;
	static unsigned char flag = 0;
	static uint8_t data;

    UART_clearInterruptFlag(EUSCI_A3_BASE, status);
	
	status = UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
		data = UART_receiveData(EUSCI_A3_BASE);
		if (data == 0xff && flag == 0) //接收帧头
		{
			flag = 1;
			openmv[0] = 0xff;
		}
		else if (data == 0xef && flag == 1) //二次验证帧头
		{
			flag = 2;
			openmv[1] = 0xef;
		}
		else if (flag == 2) //开始接收有用数据
		{
			flag = 3;
			openmv[2] = data;
		}
		else if (flag == 3)
		{
			if (data == 0xfe) //判断是否为帧尾
			{
				flag = 0;
				openmv[3] = 0xef;
				Get_ValueData();
			}
			else if (data != 0xef) //不是帧尾 清除数据 重新来过
			{
				flag = 0;
				for (int i = 0; i < 4; i++)
				{
					openmv[i] = 0x00;
				}
			}
		}
		else
		{
			flag = 0;
			for (int i = 0; i < 4; i++)
			{
				openmv[i] = 0x00;
			}
		}
    }
}





