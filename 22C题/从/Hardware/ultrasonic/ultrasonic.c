#include "ultrasonic.h"
/**
 * @brief ��������ʼ��(115200)
 * @param None
 * @retval None
 */
void ultrasonic_init(void)
{
	eUSCI_UART_Config uartConfig;
	
	
	//����GPIO����
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9,GPIO_PIN6|GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
	
	
	//���ýṹ��
	uartConfig.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
	uartConfig.clockPrescalar = 26;
	uartConfig.firstModReg = 0;
	uartConfig.secondModReg = 111;
	uartConfig.overSampling = 1;
	uartConfig.parity = EUSCI_A_UART_NO_PARITY;
	uartConfig.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
	uartConfig.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	uartConfig.uartMode = EUSCI_A_UART_MODE;
	
	
	
	//��ʼ������
	UART_initModule(EUSCI_A3_BASE, &uartConfig);
	
	
	//��������
	UART_enableModule(EUSCI_A3_BASE);
	
	
	//������������ж�
	UART_enableInterrupt(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	
	
	//�������ڶ˿��ж�
	Interrupt_enableInterrupt(INT_EUSCIA3);
	

	//�������ж�
	Interrupt_enableMaster();	
}


//��ų��������ڽ��յ��ֽ���
unsigned char   dat_buf[4];	
//���������ݽ��ձ���
unsigned char  num_idx=0;
//��������ľ���
int distance=0;					


/**
 * @brief �����жϷ�����
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
			//�жϳ��������ڽ��յ����ݣ������0XA5�ͽ��գ����������
			num_idx++;
		}
		//�����յ�3���ֽ���֮�󣬿�ʼ�����ݽ��н��㲢�����ó����յľ��롣
        if(num_idx==3)	
        {	     
            num_idx = 0;
			flt_val = K*flt_val + (1-K)*(dat_buf[1]<<8 | dat_buf[2]);
            distance= flt_val;					  
        } 
    }
}
