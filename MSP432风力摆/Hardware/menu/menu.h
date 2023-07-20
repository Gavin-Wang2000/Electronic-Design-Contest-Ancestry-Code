#ifndef __MENU_H
#define __MENU_H

#include "main.h"

/**
 * @brief 菜单结构体
 */
typedef struct 
{
	uint8_t current;			//当前组索引号
	uint8_t down;				//下翻索引号
	void (*operation)(void);	//动作
}menu_table_t;

extern int modeFlag;

void mainMeun(void);

#endif //__MENU_H








