#include "sensor.h"
uint8_t sensor[9];

/**
 * @brief 红外初始化
 * @param  无
 * @retval  无
 **/
void sensor_init(void)
{
	GPIO_setAsInputPin(GPIO_PORT_P7, GPIO_PIN6);
	GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN6);
	GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN6);
	GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN7);
	GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN3);
	GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN1);
	GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN5);
	GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN7);
}

/**
 * @brief 红外读取
 * @param  None
 * @retval None
 **/
void sensorRade(void)
{
	sensor[0] = GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN6);
	sensor[1] = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6);
	sensor[2] = GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN6);
	sensor[3] = GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN7);
	sensor[4] = GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN3);
	sensor[5] = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1);
	sensor[6] = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5);
	sensor[7] = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7);

	if ((sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0 && sensor[5] == 0 && sensor[6] == 0 && sensor[7] == 0) || (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0 && sensor[5] == 0 && sensor[6] == 0 && sensor[7] == 0) || (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0 && sensor[5] == 0 && sensor[6] == 0 && sensor[7] == 1))
	{
		LED_RED_On();
		stopFlag = 1;
	}
	else
	{
		stopFlag = 0;
	}
}
/**
 * @brief 红外偏差换算
 * @param  无
 * @retval  偏差
 **/
int sensorOffset(void)
{
	if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 0 && sensor[4] == 0 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 0;
	}

	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 0 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 1;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 0 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return -1;
	}

	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 2;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 0 && sensor[5] == 0 && sensor[6] == 1 && sensor[7] == 1)
	{
		return -2;
	}

	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 3;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 0 && sensor[6] == 1 && sensor[7] == 1)
	{
		return -3;
	}

	else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 4;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 0 && sensor[6] == 0 && sensor[7] == 1)
	{
		return -4;
	}

	else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 5;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 0 && sensor[7] == 1)
	{
		return -5;
	}

	else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 6;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 0 && sensor[7] == 0)
	{
		return -6;
	}

	else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1)
	{
		return 7;
	}
	else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 0)
	{
		return -7;
	}
}