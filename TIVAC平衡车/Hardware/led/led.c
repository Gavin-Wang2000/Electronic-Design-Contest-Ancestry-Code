#include "led.h"

/**
 * @brief led��ʼ��
 * @param  None
 * @retval  None
 **/
void led_Init(void)
{

   /*GPIO����ʹ��*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
   /*GPIOģʽ����*/    
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}
