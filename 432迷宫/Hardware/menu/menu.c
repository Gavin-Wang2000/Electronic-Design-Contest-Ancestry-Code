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
		OLED_Print(1, 1, "working...");
//		OLED_Print(1, 11, "[%d].%d",Road_indexs,Road_date[Road_indexs].dir);
		OLED_Print(2, 1, "moveMode:%d", judgeCrossing());
		OLED_Print(3, 1, "nowSpeed:%2d", speedSum);
		OLED_Print(4, 1, "%d%d%d%d%d%d%d %d", sensor[6], sensor[5], sensor[4], sensor[3], sensor[2], sensor[1], sensor[0], sensor[7]);
	}
    else if(showIndex == 2)
    {
        OLED_Print(1, 1, "working...");
		OLED_Print(1, 11, "[%d].%d",Road_indexs,Road_date[Road_indexs].dir);//只在返回时打印R_indexs
		OLED_Print(2, 1, "moveMode:%d", judgeCrossing());
		OLED_Print(3, 1, "nowSpeed:%2d", speedSum);
		OLED_Print(4, 1, "%d%d%d%d%d%d%d %d", sensor[6], sensor[5], sensor[4], sensor[3], sensor[2], sensor[1], sensor[0], sensor[7]);
    }
	else
	{
		OLED_Print(1, 1, "backroad...");
		OLED_Print(2, 1, "[%d].dir = %d", tempRoadIndex,tempRoadDir);
	}
	/*页面切换*/
	if(key[2].keyShortDown == 1)
	{
		showIndex = 3;
		OLED_Clear();
		key[2].keyShortDown = 0;
	}
	/*编辑路线索引*/
	if (key[0].keyShortDown == 1 && showIndex == 3 && changFlag == 0)
	{
		tempRoadIndex ++;
		key[0].keyShortDown = 0;
	}
	if (key[1].keyShortDown == 1 && showIndex == 3 && changFlag == 0)
	{
		tempRoadIndex --;
		if(tempRoadIndex < 1)
		{
			tempRoadIndex = 1;
		}
		key[1].keyShortDown = 0;
	}
	/*编辑路线方向*/
	if (key[0].keyShortDown == 1 && showIndex == 3 && changFlag == 1)
	{
		tempRoadDir ++;
		if(tempRoadDir > 3)
		{
			tempRoadDir = 0;
		}
		key[0].keyShortDown = 0;
	}
	if (key[1].keyShortDown == 1 && showIndex == 3 && changFlag == 1)
	{
		tempRoadDir --;
		if(tempRoadIndex < 0)
		{
			tempRoadIndex = 3;
		}
		key[1].keyShortDown = 0;
	}
	/*调节切换*/
	if (key[3].keyShortDown == 1 && showIndex == 3)
	{
		changFlag = !changFlag;
		OLED_Clear();
		key[3].keyShortDown = 0;
	}
	/*确定赋值*/
	if (key[3].keyLongDown == 1 && showIndex == 3)
	{
		Road_date[tempRoadIndex].dir = tempRoadDir;
		Road_indexs = tempRoadIndex;
		LED_RED_On();
		delay_ms(200);
		LED_RED_Off();
		OLED_Clear();
		key[3].keyLongDown = 0;
	}
	/*按键调速*/
	if (key[0].keyShortDown == 1 && showIndex == 0)
	{
		targtSpeed += 1;
		if (targtSpeed >= 15)
		{
			targtSpeed = 15;
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
