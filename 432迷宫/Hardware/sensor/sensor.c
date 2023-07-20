#include "sensor.h"
uint8_t sensor[8];
	

/**
 * @brief 红外初始化
 * @param  无
 * @retval  无
 **/
void sensor_init(void)
{
    GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN7);
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN6);
	GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN6);
	GPIO_setAsInputPin(GPIO_PORT_P7, GPIO_PIN5);
	GPIO_setAsInputPin(GPIO_PORT_P7, GPIO_PIN4);
	GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN2);
}


/**
 * @brief 红外读取
 * @param  None
 * @retval None
 **/
void sensorRade(void)
{
	sensor[0] = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1);
	sensor[1] = GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN5);
	sensor[2] = GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN3);
	sensor[3] = GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN7);
	sensor[4] = GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN6);
	sensor[5] = GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN4);
	sensor[6] = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6);
	sensor[7] = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN2);
}