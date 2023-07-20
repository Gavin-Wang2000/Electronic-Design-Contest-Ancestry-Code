#include "motor.h"
int PWM_MAX = 90, PWM_MIN = -90;

/**
 * @brief 电机初始化
 * @param None
 * @retval None
 */
void motor_Init(void)
{
	/*GPIO外设使能*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	/*A*/
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	
	/*B*/
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
}

/**
 * @brief 电机PWM初始化（10KHZ）
 * @param None
 * @retval None
 */
void motor_pwm_Init(void)
{
	//配置PWM时钟（设置USEPWMDIV分频器）
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);	

	//使能时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//使能PWM模块0时钟																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		//使能GPIOB时钟		
	
	//使能引脚复用PWM功能
	GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_5);
	
	//PWM信号分配
	GPIOPinConfigure(GPIO_PB4_M0PWM2);					//PB4->PWM模块0信号2																							
	GPIOPinConfigure(GPIO_PB5_M0PWM3);					//PB5->PWM模块0信号3	
	
	//配置PWM发生器
	//模块0->发生器1->上下计数，不同步
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	
	//配置PWM周期
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,8000-1);		
	
	//使能PWM模块0输出
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_3_BIT,true);
	
	//使能PWM发生器
	PWMGenEnable(PWM0_BASE,PWM_GEN_1);
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
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
	}
	else
	{
		A = -A;
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	}
	
	if (B >= 0)
	{
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
		
	}
	else
	{
		B = -B;
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
	}
	
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) * A / 100);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) * B / 100);
}

/**
 * @brief 限幅函数
 * @param  限幅变量地址
 * @retval  None
 **/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/**
 * @brief 电机失能
 * @param  None
 * @retval  None
 **/
void brake(void)
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
}