#include "key.h"
 
struct keys key[4] = {0};
 
 
/**
 * @brief 按键初始化
 * @param None
 * @retval None
 */
void key_init(void)
{
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN4);
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P8,GPIO_PIN4);
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P8,GPIO_PIN5);
}

/**
 * @brief 按键扫描（放入定时器）
 * @param  None
 * @retval None
 **/
void keyScan(void)
{
	key[0].keyState = GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1);
	key[1].keyState = GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4);
	key[2].keyState = GPIO_getInputPinValue(GPIO_PORT_P8,GPIO_PIN4);
	key[3].keyState = GPIO_getInputPinValue(GPIO_PORT_P8,GPIO_PIN5);
	
	for(int i = 0;i < 4;i++)
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