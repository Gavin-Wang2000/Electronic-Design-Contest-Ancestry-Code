#include "timer.h"

void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	// 开启定时器时钟,即内部时钟 CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	// 自动重装载寄存器周的值(计数值)
	TIM_TimeBaseStructure.TIM_Period=199;
	// 累计 TIM_Period 个频率后产生一个更新或者中断
	// 时钟预分频数为 71，则驱动计数器的时钟 CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler= 7199;
	// 时钟分频因子 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	// 计数器计数模式
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// 重复计数器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	// 初始化定时器
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	
	
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	// 设置子优先级为 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	// 清除计数器中断标志位
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	// 开启计数器中断
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	// 使能计数器
	TIM_Cmd(TIM4, ENABLE);
}


unsigned int oneSecond;

/**
 * @brief 定时器4中断服务函数
 * @param None
 * @retval None
 */
void TIM4_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM4,TIM_IT_Update) != RESET ) 
	{	
		/*每秒计时*/
		static unsigned int myTime;
		myTime++;
		if(myTime >= 50)
		{
			oneSecond++;
			myTime = 0;
		}
		
		/*按键扫描*/
		keyScan();
		
		/*主函数执行*/
		mainControl();
		
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
	}
}

