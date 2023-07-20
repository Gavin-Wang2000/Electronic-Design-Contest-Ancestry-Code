#include "laser.h"

/**
 * @brief 激光初始化
 * @param  None
 * @retval  None
 **/
void Laser_Init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
	Laser_Off();
}

/**
 * @brief 开启激光
 * @param  None
 * @retval  None
 **/
void Laser_On(void)
{
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
}

/**
 * @brief 关闭激光
 * @param  None
 * @retval  None
 **/
void Laser_Off(void)
{
	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
}