#include "key.h"

struct keys key[2] = {0};


/**
 * @brief 按键初始化
 * @param  无
 * @retval  无
 **/
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
}

/**
 * @brief 按键扫描（放入定时器）
 * @param  无
 * @retval  无
 **/
void keyScan(void)
{
	key[0].keyState = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
	key[1].keyState = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
	
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