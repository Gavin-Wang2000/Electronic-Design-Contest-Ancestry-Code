#include "servo.h"

/**
 * @brief 舵机初始化（频率50HZ）
 * @param  无
 * @retval 无
 **/
 
void Servo_init(void)
{
	Timer_A_PWMConfig pwmConfig;
	
	
	//配置GPIO复用
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7,GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
	
	
	//配置结构体
    pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    pwmConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48;
    pwmConfig.timerPeriod = 20000;
    pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;
    pwmConfig.dutyCycle = 0;
	
	
	//初始化定时器
	Timer_A_generatePWM(TIMER_A1_BASE,&pwmConfig);
	
	Servo_SetAnger(1550);//舵机回正
}

/**
 * @brief 设置舵机角度
 * @param 预设角度（0到180°）
 * @retval  无
 **/
void Servo_SetAnger(float Angle)
{
	Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1,Angle);
}






