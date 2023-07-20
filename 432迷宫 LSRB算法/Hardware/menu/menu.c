#include "menu.h"

/*启动标志位*/
int startFlag = 0;

/*页面索引*/
int showIndex = 0;

/*路线临时变量*/
int tempRoadIndex = 1;
int tempRoadDir = 0;

/*改变调节参数*/
int changFlag = 0;//0表示调节路线 1表示调节方向

/**
 * @brief 主菜单
 * @param None
 * @retval None
 */
void main_menu(void)
{
	/*设置模式显示*/
	if (showIndex == 0)
	{
		OLED_Print(1, 1, "setting speed...");
		OLED_Print(2, 1, "targetSpeed:%2d", targtSpeed);
	}
	/*工作模式显示*/
	else if (showIndex == 1)
	{
		OLED_Print(1, 1, "roadIndex:%d",roadIndex);
		OLED_Print(2, 1, "moveMode:%d", judgeCrossing());
		OLED_Print(3, 1, "tempMoveMode:%2d", backMoveMode);
		OLED_Print(4, 1, "%d%d%d%d%d%d%d %d", sensor[6], sensor[5], sensor[4], sensor[3], sensor[2], sensor[1], sensor[0], sensor[7]);
	}
	/*按键调速*/
	if (key[0].keyShortDown == 1 && showIndex == 0)
	{
		targtSpeed += 1;
		if (targtSpeed >= 11)
		{
	    	turnRightGoLineTime = 30;//右转前直行距离
            turnLeftGoLineTime = 30;//左转前直行距离
		}
		key[0].keyShortDown = 0;
	}
	if (key[1].keyShortDown == 1 && showIndex == 0)
	{
		targtSpeed -= 1;
		if (targtSpeed <= 4)
		{
			targtSpeed = 4;
		}
		key[1].keyShortDown = 0;
	}
	/*发车指令*/
	if (key[0].keyLongDown == 1)
	{
		buzzer_on();
		delay_ms(400);
		buzzer_off();
		startFlag += 1;
		if(startFlag > 2)
		{
			startFlag = 2;
		}
		showIndex = 1;
		OLED_Clear();
		key[0].keyLongDown = 0;
	}
}
