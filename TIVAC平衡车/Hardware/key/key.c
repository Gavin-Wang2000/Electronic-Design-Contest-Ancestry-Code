#include "key.h"

struct keys key[2] = {0};
/**
 * @brief 按键初始化
 * @param  None
 * @retval  None
 **/
void key_Init(void)
{
   //使能外设时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	//解锁 PF0
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x00;
	
	//配置GPIO
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);
	
	//配置上拉
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

/**
 * @brief 按键扫描（放入定时器）
 * @param  None
 * @retval None
 **/
void keyScan(void)
{
	key[0].keyState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)&GPIO_PIN_4;
	key[1].keyState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)&GPIO_PIN_0;
	
	for(int i = 0;i < 2;i++)
	{
		switch(key[i].keyFlag)
		{
			case 0:
			{
				if(key[i].keyState == 0)
				{
					key[i].keyFlag = 1;
					key[i].keyTime = 0;
				}
			}break;
			case 1:
			{
				if(key[i].keyState == 0)
				{
					key[i].keyFlag = 2;
				}
				else
				{
					key[i].keyFlag = 0;
				}
			}break;
			case 2:
			{
				if(key[i].keyState == 1)
				{
					key[i].keyFlag = 0;
					if(key[i].keyTime < 70)
					{
						key[i].keyShortDown = 1;
					}
				}
				else
				{
					key[i].keyTime++;
					if(key[i].keyTime > 70)
					{
						key[i].keyLongDown = 1;
					}
				}
			}break;
		}
	}
}
