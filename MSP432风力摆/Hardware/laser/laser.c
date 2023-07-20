#include "laser.h"

/**
 * @brief �����ʼ��
 * @param  None
 * @retval  None
 **/
void Laser_Init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
	Laser_Off();
}

/**
 * @brief ��������
 * @param  None
 * @retval  None
 **/
void Laser_On(void)
{
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
}

/**
 * @brief �رռ���
 * @param  None
 * @retval  None
 **/
void Laser_Off(void)
{
	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
}