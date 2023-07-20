#include "key.h"
 
struct keys key[2] = {0};
 
 
/**
 * @brief 按键初始化
 * @param None
 * @retval None
 */
void key_init(void)
{
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN4);
}

#if USE_TIME
/**
 * @brief 按键扫描（放入定时器）
 * @param  None
 * @retval None
 **/
void keyScan(void)
{
	key[0].keyState = GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1);
	key[1].keyState = GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4);
	
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

#else
/**
 * @brief 按键扫描（无需定时器）
 * @param  None
 * @retval None
 **/
void keyScan(void)
{
	static uint8_t flag1 = 0,flag2 = 0;
	if((flag1 == 0) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1) == 0))
	{
		delay_us(5);
		if((flag1 == 0) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1) == 0))
		{
			flag1 = 1;
		}
	}
	if((flag1 == 1) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1) == 1))
	{
		delay_us(5);
		if((flag1 == 1) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1) == 1))
		{
			key[0].keyShortDown = 1;
			flag1 = 0;
		}
	}
	
	if((flag2 == 0) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4) == 0))
	{
		delay_us(5);
		if((flag2 == 0) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4) == 0))
		{
			flag2 = 1;
		}
	}
	if((flag2 == 1) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4) == 1))
	{
		delay_us(5);
		if((flag2 == 1) && (GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4) == 1))
		{
			key[1].keyShortDown = 1;
			flag2 = 0;
		}
	}
}
#endif