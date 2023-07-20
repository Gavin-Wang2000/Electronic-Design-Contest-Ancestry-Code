#include "led.h"

/**
 * @brief led初始化
 * @param  None
 * @retval  None
 **/
void led_Init(void)
{

   /*GPIO外设使能*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
   /*GPIO模式配置*/    
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}
