#include "zigbee.h"


#pragma import(__use_no_semihosting)                            
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
  
void _sys_exit(int x)
{ 
	x = x; 
} 

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
int fputc(int ch, FILE *f)
{      
	UART_transmitData(EUSCI_A2_BASE,(uint8_t)ch); 
	return ch;
}


/**
 * @brief zigbee初始化
 * @param None
 * @retval None
 * @note 波特率-9600
 */
void zigbee_init(void)
{
	eUSCI_UART_Config uartConfig;
	
	
	//配置GPIO复用
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3,GPIO_PRIMARY_MODULE_FUNCTION);
	
	
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
	
	
	
	//初始化串口
	UART_initModule(EUSCI_A2_BASE, &uartConfig);
	
	
	//开启串口
	UART_enableModule(EUSCI_A2_BASE);
	
	
	//开启串口相关中断
	UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	
	
	//开启串口端口中断
	Interrupt_enableInterrupt(INT_EUSCIA0);
	

	//开启总中断
	Interrupt_enableMaster();
}




/**
 * @brief 串口中断服务函数
 * @param None
 * @retval None
 */
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
		UART_receiveData(EUSCI_A2_BASE);
    }
}





