#include "encoder.h"

/*编码器计数值*/
static volatile int32_t enc_cnt[2];

/**
 * @brief 编码器初始化
 * @param None
 * @retval None
 */
void encoder_init(void)
{
	/*编码器1*/
	//中断输入
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN2);
	//非中断输入
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
	//清除中断标志位
	GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN2);
	//配置触发方式
	GPIO_interruptEdgeSelect(GPIO_PORT_P4,GPIO_PIN2,GPIO_LOW_TO_HIGH_TRANSITION);
	//开启外部中断
	GPIO_enableInterrupt(GPIO_PORT_P4,GPIO_PIN2);
	//开启端口中断
	Interrupt_enableInterrupt(INT_PORT4);

	
	
	/*编码器2*/
	//中断输入
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN4);
	//非中断输入
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
	//清除中断标志位
	GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN4);
	//配置触发方式
	GPIO_interruptEdgeSelect(GPIO_PORT_P4,GPIO_PIN4,GPIO_LOW_TO_HIGH_TRANSITION);
	//开启外部中断
	GPIO_enableInterrupt(GPIO_PORT_P4,GPIO_PIN4);
	//开启端口中断
	Interrupt_enableInterrupt(INT_PORT4);		
}


/**
 * @brief 编码器读取计数的差分(速度)
 * @param a A相编码器数据缓存地址
 * @param b B相编码器数据缓存地址
 * @retval None
 */
void encoder_read(int32_t *a,int32_t *b)
{	
	//暂存
	*a = -enc_cnt[0];
	*b =  enc_cnt[1];
	
	//清零
	enc_cnt[0] = 0;
	enc_cnt[1] = 0;
}



/**
 * @brief 外部中断服务函数
 * @param None 
 * @retval None
 */
void PORT4_IRQHandler(void)
{
    uint32_t status;

    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    if(status & GPIO_PIN2)
    {
		if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3))
		{
			enc_cnt[0]++;
		}
		else
		{
			enc_cnt[0]--;
		}
    }
	
	if(status & GPIO_PIN4)
	{
		if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5))
		{
			enc_cnt[1]++;
		}
		else
		{
			enc_cnt[1]--;
		}
	}
}


