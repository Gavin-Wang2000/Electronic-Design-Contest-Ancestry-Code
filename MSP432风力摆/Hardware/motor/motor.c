#include "motor.h"
int PWM_MAX = 100, PWM_MIN = -100;

/**
 * @brief 电机PWM初始化（10KHZ）
 * @param None
 * @retval None
 */
void motor_pwm_init(void)
{
	Timer_A_PWMConfig pwmConfig;

	//配置GPIO复用
	// A
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
	// B
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	// C
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
	// D
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	
	//配置结构体
	pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
	pwmConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48;
	pwmConfig.timerPeriod = 100;
	pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;
	pwmConfig.dutyCycle = 0;

	//初始化定时器
	// A
	pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
	Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
	// B
	pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
	// C
	pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
	Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
	// D
	pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
	Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
	
}

/**
 * @brief 轮子装载
 * @param X方向
 * @param Y方向
 * @retval None
 */
void Load(int16_t Motor_X, int16_t Motor_Y)
{
	if (Motor_X >= 0)
	{
		/*A*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, Motor_X);
		/*C*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);
	}
	else
	{
		Motor_X = -Motor_X;
		/*A*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, 0);
		/*C*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, Motor_X);
	}

	if (Motor_Y >= 0)
	{
		/*B*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, Motor_Y);
		/*D*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, 0);
	}
	else
	{
		Motor_Y = -Motor_Y;
		/*B*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 0);
		/*D*/
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, Motor_Y);
	}
}

/**
 * @brief 电机限幅
 * @param  限幅电机地址值
 * @retval  无
 **/
void Limit(int *motorA, int *motorB)
{
	if (*motorA > PWM_MAX)
	{
		*motorA = PWM_MAX;
	}
	if (*motorA < PWM_MIN)
	{
		*motorA = PWM_MIN;
	}

	if (*motorB > PWM_MAX)
	{
		*motorB = PWM_MAX;
	}
	if (*motorB < PWM_MIN)
	{
		*motorB = PWM_MIN;
	}
}
