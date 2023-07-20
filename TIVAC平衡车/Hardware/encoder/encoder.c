#include "encoder.h"

/**
 * @brief QEI0初始化
 * @param  None
 * @retval  None
 **/
void QEI0_Init(void)
{
	//开启QEI0时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
	
	//开启对应GPIO复用
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);  
	
	/*！一定要注意PD7要解锁！*/
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
	
	GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7); //配置PD6,7为QEI模块A、B相
	GPIOPinConfigure(GPIO_PD6_PHA0);
	GPIOPinConfigure(GPIO_PD7_PHB0);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);  //上拉输入
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
	
	//配置QEI0模块1
	QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B         //A、B相边沿都计数
												 |QEI_CONFIG_NO_RESET						 //索引检测到脉冲不复位
												 |QEI_CONFIG_QUADRATURE					 //选择QEI为正交模式
												 |QEI_CONFIG_NO_SWAP, 65535);		 //A、B不交换，最大计数值65535
	//配置测速滤波
	QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_16);
	QEIFilterEnable(QEI0_BASE);
	
	QEIEnable(QEI0_BASE);
}

/**
 * @brief QEI1初始化
 * @param  None
 * @retval  None
 **/
void QEI1_Init(void)
{
	//开启QEI0时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
	
	//开启对应GPIO复用
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5|GPIO_PIN_6); //配置PD6,7为QEI模块A、B相
	GPIOPinConfigure(GPIO_PC5_PHA1);
	GPIOPinConfigure(GPIO_PC6_PHB1);
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);  //上拉输入
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
	//配置QEI0模块1
	QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B         //A、B相边沿都计数
												 |QEI_CONFIG_NO_RESET						 //索引检测到脉冲不复位
												 |QEI_CONFIG_QUADRATURE					 //选择QEI为正交模式
												 |QEI_CONFIG_NO_SWAP, 65535);		 //A、B不交换，最大计数值65535
	//配置测速滤波
	QEIFilterConfigure(QEI1_BASE, QEI_FILTCNT_16);
	QEIFilterEnable(QEI1_BASE);
	
	QEIEnable(QEI1_BASE);
}

/**
 * @brief 编码器初始化
 * @param  None
 * @retval  None
 **/
void Encoder_Init(void)
{
	QEI0_Init();
	QEI1_Init();
}

/**
 * @brief 编码器读取计数的差分(速度)
 * @param a A相编码器数据缓存地址
 * @param b B相编码器数据缓存地址
 * @retval None
 */
void speedRead(int32_t* a,int32_t* b)
{
	int aSpeed,bSpeed;
	
	/*A电机*/
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
	
	/*B电机*/
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

