#include "motor.h"
int PWM_MAX = 90, PWM_MIN = -90;

/**
 * @brief �����ʼ��
 * @param None
 * @retval None
 */
void motor_Init(void)
{
	/*GPIO����ʹ��*/
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
 * @brief ���PWM��ʼ����10KHZ��
 * @param None
 * @retval None
 */
void motor_pwm_Init(void)
{
	//����PWMʱ�ӣ�����USEPWMDIV��Ƶ����
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);	

	//ʹ��ʱ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//ʹ��PWMģ��0ʱ��																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		//ʹ��GPIOBʱ��		
	
	//ʹ�����Ÿ���PWM����
	GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_5);
	
	//PWM�źŷ���
	GPIOPinConfigure(GPIO_PB4_M0PWM2);					//PB4->PWMģ��0�ź�2																							
	GPIOPinConfigure(GPIO_PB5_M0PWM3);					//PB5->PWMģ��0�ź�3	
	
	//����PWM������
	//ģ��0->������1->���¼�������ͬ��
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);	
	
	//����PWM����
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,8000-1);		
	
	//ʹ��PWMģ��0���
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
	PWMOutputState(PWM0_BASE,PWM_OUT_3_BIT,true);
	
	//ʹ��PWM������
	PWMGenEnable(PWM0_BASE,PWM_GEN_1);
}

/**
 * @brief ����װ��
 * @param A A���ֵ(��)
 * @param B B���ֵ(��)
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
 * @brief �޷�����
 * @param  �޷�������ַ
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
 * @brief ���ʧ��
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