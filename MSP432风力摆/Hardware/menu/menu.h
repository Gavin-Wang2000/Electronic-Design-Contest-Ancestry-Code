#ifndef __MENU_H
#define __MENU_H

#include "main.h"

/**
 * @brief �˵��ṹ��
 */
typedef struct 
{
	uint8_t current;			//��ǰ��������
	uint8_t down;				//�·�������
	void (*operation)(void);	//����
}menu_table_t;

extern int modeFlag;

void mainMeun(void);

#endif //__MENU_H








