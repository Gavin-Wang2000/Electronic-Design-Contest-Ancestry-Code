#include "buzzer.h"

/**
 * @brief 蜂鸣器初始化
 * @param None
 * @retval None
 */
void buzzer_init(void)
{
	GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN6);
	GPIO_setOutputHighOnPin(GPIO_PORT_P7,GPIO_PIN6);
}

