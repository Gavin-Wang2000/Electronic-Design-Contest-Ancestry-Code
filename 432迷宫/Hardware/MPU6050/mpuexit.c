#include "mpuexit.h"

/**
 * @brief �������ⲿ�жϳ�ʼ��
 * @param  None
 * @retval  None
 **/
 
void MPU6050_EXTI_Init(void)
{
	/*����6.4��Ϊ��������*/
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
	/*���IO�ڵ��жϱ�־λ*/
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
	/*�����ж�ģʽ*/
	MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P6,GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
	/*ʹ��P6.4���ⲿ�жϣ�����ֻ��һ�����ţ�*/
    MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
	/*ʹ��P1�˿ڵ��жϣ�������һ����˿ڣ�����������ţ�*/
    MAP_Interrupt_enableInterrupt(INT_PORT6);
}

/**
 * @brief �ⲿ�жϷ�����
 * @param  None
 * @retval  None
 **/
void PORT6_IRQHandler(void)
{
	uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    /*��⵽P6.4�����ⲿ�жϵĴ���*/
    if(status & GPIO_PIN4)
    {

    }
}