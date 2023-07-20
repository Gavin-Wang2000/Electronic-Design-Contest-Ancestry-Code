#include "encoder.h"

/**
 * @brief QEI0��ʼ��
 * @param  None
 * @retval  None
 **/
void QEI0_Init(void)
{
	//����QEI0ʱ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
	
	//������ӦGPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);  
	
	/*��һ��Ҫע��PD7Ҫ������*/
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
	
	GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7); //����PD6,7ΪQEIģ��A��B��
	GPIOPinConfigure(GPIO_PD6_PHA0);
	GPIOPinConfigure(GPIO_PD7_PHB0);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);  //��������
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
	
	//����QEI0ģ��1
	QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B         //A��B����ض�����
												 |QEI_CONFIG_NO_RESET						 //������⵽���岻��λ
												 |QEI_CONFIG_QUADRATURE					 //ѡ��QEIΪ����ģʽ
												 |QEI_CONFIG_NO_SWAP, 65535);		 //A��B��������������ֵ65535
	//���ò����˲�
	QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_16);
	QEIFilterEnable(QEI0_BASE);
	
	QEIEnable(QEI0_BASE);
}

/**
 * @brief QEI1��ʼ��
 * @param  None
 * @retval  None
 **/
void QEI1_Init(void)
{
	//����QEI0ʱ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
	
	//������ӦGPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5|GPIO_PIN_6); //����PD6,7ΪQEIģ��A��B��
	GPIOPinConfigure(GPIO_PC5_PHA1);
	GPIOPinConfigure(GPIO_PC6_PHB1);
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);  //��������
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
	//����QEI0ģ��1
	QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B         //A��B����ض�����
												 |QEI_CONFIG_NO_RESET						 //������⵽���岻��λ
												 |QEI_CONFIG_QUADRATURE					 //ѡ��QEIΪ����ģʽ
												 |QEI_CONFIG_NO_SWAP, 65535);		 //A��B��������������ֵ65535
	//���ò����˲�
	QEIFilterConfigure(QEI1_BASE, QEI_FILTCNT_16);
	QEIFilterEnable(QEI1_BASE);
	
	QEIEnable(QEI1_BASE);
}

/**
 * @brief ��������ʼ��
 * @param  None
 * @retval  None
 **/
void Encoder_Init(void)
{
	QEI0_Init();
	QEI1_Init();
}

/**
 * @brief ��������ȡ�����Ĳ��(�ٶ�)
 * @param a A����������ݻ����ַ
 * @param b B����������ݻ����ַ
 * @retval None
 */
void speedRead(int32_t* a,int32_t* b)
{
	int aSpeed,bSpeed;
	
	/*A���*/
	aSpeed = QEIPositionGet(QEI0_BASE);
	if(QEIDirectionGet(QEI0_BASE) < 0 && aSpeed != 0)
	{
		aSpeed -= 65535; 
		*a = aSpeed;
	}
	else
	{
		*a = aSpeed;
	}
	QEIPositionSet(QEI0_BASE,0);
	
	/*B���*/
	bSpeed = QEIPositionGet(QEI1_BASE);
	if(QEIDirectionGet(QEI1_BASE) < 0 && bSpeed != 0)
	{
		bSpeed -= 65535; 
		*b = bSpeed;
	}
	else
	{
		*b = bSpeed;
	}
	QEIPositionSet(QEI1_BASE,0);
}

