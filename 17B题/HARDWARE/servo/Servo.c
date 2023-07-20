#include "servo.h"                  // Device header

/**
 * @brief 舵机初始化
 * @param  无
 * @retval  无
 */
void Servo_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStrue);
	
	//20ms=50HZ=72 000000/(72-1)/(20000-1)
	TIM_TimeBaseStructure.TIM_Period = 20000-1; //ARR
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	//CCR
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	
 
    TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	
}

/**
 * @brief 舵机角度设置
 * @param  角度大小（0—180度）
 * @retval
 */
void Servo1_SetAngle(float Angle)
{
	if(Angle > 110)
	{
		Angle = 110;
	}
	if(Angle < 65)
	{
		Angle = 65;
	}
	TIM_SetCompare2(TIM8,Angle / 180 * 2000 + 500);
}

void Servo2_SetAngle(float Angle)
{
	if(Angle > 115)
	{
		Angle = 115;
	}
	if(Angle < 65)
	{
		Angle = 65;
	}
	TIM_SetCompare1(TIM8,Angle / 180 * 2000 + 500);
}
