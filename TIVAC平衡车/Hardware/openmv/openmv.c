#include "openmv.h"

int fputc(int ch, FILE *f){UARTCharPut(UART0_BASE,ch);	return (ch);}

/**
 * @brief openmv串口初始化
 * @param  波特率
 * @retval  None
 **/
void openmvUsart_init(int baud)
{
	//使能UART使用的GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//使能串口0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	//串口引脚配置
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	//设置时钟16MHZ
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    //UART编号，波特率，UART时钟频率
    UARTStdioConfig(0, baud, 16000000);
	 //使能FIFO
	UARTFIFOEnable(UART0_BASE);
    //接收发送的FIFO为1个字节
	UARTFIFOLevelSet(UART0_BASE,1,1);
	//使能串口接收函数
	UARTIntEnable(UART0_BASE,UART_INT_RX);
	//注册中断函数
	UARTIntRegister(UART0_BASE,USART0_IRQHandler);
	//设置中断优先级
	IntPrioritySet(INT_UART0,0);
    //开启中断
	IntEnable(INT_UART0);
	IntMasterEnable();
    //使能串口
    UARTEnable(UART0_BASE);
}

openmvData trace = {0};

int openmv[5];

/**
 * @brief 去除帧头帧尾获取有效偏移值 0x5b 0x5b data1 data2 0x3c
 * @param  无
 * @retval  无
 */
void Get_openmvData(void)
{
	trace.cx = openmv[2];
	trace.cy = openmv[3];
}

/**
 * @brief 串口0中断服务函数
 * @param  None
 * @retval  None
 **/
void USART0_IRQHandler(void)
{
	static int flag = 0;
	uint8_t data;
    //读取中断状态
    uint32_t status=UARTIntStatus(UART0_BASE, true);
    //清除中断标志位
	UARTIntClear(UART0_BASE, status);
    //判断UART0有没有字符未读取
	while(UARTCharsAvail(UART0_BASE))
	{
        //如果有字符为读取就取出，使用UARTCharGetNonBlocking防止等待
	    data = (uint8_t)UARTCharGetNonBlocking(UART0_BASE);
		if(data == 0x5b && flag == 0) //接收帧头
		{
			flag = 1;
			openmv[0] = 0x5b;
		}
		else if(data == 0x5b && flag == 1) //二次验证帧头
		{
			flag = 2;
			openmv[1] = 0x5b;
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
			if(data == 0x3c) //判断是否为帧尾
			{
				flag = 0;
				openmv[4] = 0x3c;
				Get_openmvData();
			}
			else if(data != 0x3c) //不是帧尾 清除数据 重新来过
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