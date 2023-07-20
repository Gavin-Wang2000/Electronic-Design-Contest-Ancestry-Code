#include "menu.h"

/**
 * @brief Ö÷²Ëµ¥º¯Êý
 * @param  None
 * @retval  None
 **/
 
void mainMenu(void)
{
	OLED_Print(1,1,"distance:%4d",distance);
	OLED_Print(2,1,"sensor:%d%d%d%d%d%d%d%d",sensor[0],sensor[1],sensor[2],sensor[3],sensor[4],sensor[5],sensor[6],sensor[7]);
	OLED_Print(3,1,"turnNum:%2d",turnNum);
	if(key[0].keyShortDown == 1 || zigbeeFlag != 0)
	{
		buzzer_on();
		delay_ms(200);
		buzzer_off();
		startFlag = 1;
		key[0].keyShortDown = 0;
		zigbeeFlag = 0;
	}
}