#include "ultrasonic.h"
/**
 * @brief 超声波初始化(115200)
 * @param None
 * @retval None
 */
void ultrasonic_init(void)
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


//存放超声波串口接收的字节数
unsigned char   dat_buf[4];	
//超声波数据接收变量
unsigned char  num_idx=0;
//计算出来的距离
int distance=0;					


/**
 * @brief 串口中断服务函数
 * @param None
 * @retval None
 */
void EUSCIA3_IRQHandler(void)
{
	const static float K = 0.1;
	static float flt_val = 212;
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    UART_clearInterruptFlag(EUSCI_A3_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
		dat_buf[num_idx]=UART_receiveData(EUSCI_A3_BASE); 
		if(dat_buf[0] != 0xA5) 
		{
			num_idx = 0; 
		}	
        else 
		{
			//判断超声波串口接收的数据，如果是0XA5就接收，否则不予接收
			num_idx++;
		}
		//当接收到3个字节数之后，开始对数据进行解算并处理。得出最终的距离。
        if(num_idx==3)	
        {	     
            num_idx = 0;
			flt_val = K*flt_val + (1-K)*(dat_buf[1]<<8 | dat_buf[2]);
            distance= flt_val;					  
        } 
    }
}
