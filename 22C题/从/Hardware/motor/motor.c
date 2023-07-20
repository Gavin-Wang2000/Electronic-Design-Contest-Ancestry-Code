#include "motor.h"
int PWM_MAX = 60, PWM_MIN = -60;

/**
 * @brief 电机初始化
 * @param None
 * @retval None
 */
void motor_init(void)
{
	// A
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN2);
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN2);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);

	// B
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN7);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
}

/**
 * @brief 电机PWM初始化（1KHZ）
 * @param None
 * @retval None
 */
void motor_pwm_init(void)
{
	Timer_A_PWMConfig pwmConfig;

	//配置GPIO复用
	// A
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
	// B
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

	//配置结构体
	pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
	pwmConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48;
	pwmConfig.timerPeriod = 100;
	pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;
	pwmConfig.dutyCycle = 0;

	//初始化定时器
	// A
	pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
	Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
	// B
	pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

/**
 * @brief 轮子装载
 * @param A A电机值(左)
 * @param B B电机值(右)
 * @retval None
 */
void Load(int16_t A, int16_t B)
{
	if (A >= 0)
	{
		GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN7);
		GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
	}
	else
	{
		A = -A;
		GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN7);
		GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
	}

	if (B >= 0)
	{
		GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN2);
		GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0);
	}
	else
	{
		B = -B;
		GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN2);
		GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);
	}

	Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, A);
	Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, B);
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

/**
 * @brief 刹车
 * @param 无
 * @retval 无
 **/
void brake(void)
{
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN2);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN7);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
}