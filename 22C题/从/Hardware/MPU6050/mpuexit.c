#include "mpuexit.h"

/**
 * @brief 陀螺仪外部中断初始化
 * @param  None
 * @retval  None
 **/
 
void MPU6050_EXTI_Init(void)
{
	/*配置6.4口为上拉输入*/
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
	/*清除IO口的中断标志位*/
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
	/*配置中断模式*/
	MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P6,GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
	/*使能P6.4的外部中断（这里只是一个引脚）*/
    MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
	/*使能P1端口的中断（这里是一个大端口，包括多个引脚）*/
    MAP_Interrupt_enableInterrupt(INT_PORT6);
}

/**
 * @brief 外部中断服务函数
 * @param  None
 * @retval  None
 **/
void PORT6_IRQHandler(void)
{
	uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    /*检测到P6.4产生外部中断的处理*/
    if(status & GPIO_PIN4)
    {

    }
}