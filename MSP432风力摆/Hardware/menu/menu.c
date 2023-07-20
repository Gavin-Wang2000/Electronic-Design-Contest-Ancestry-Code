#include "menu.h"

/*ģʽ��־λ*/
int modeFlag = -1;

/*�˵�������*/
menu_table_t table[];


/**
 * @brief ���˵�����
 * @param  None
 * @retval  None
 **/
void mainMeun(void)
{
	static uint8_t index = 0;
	
	while(1)
	{
		//����ɨ��
		keyScan();
		
		//�·���������
		if(key[1].keyShortDown == 1)
		{
			index = table[index].down;
			key[1].keyShortDown = 0;
		}
		//ȷ�ϰ�������
		if(key[0].keyShortDown == 1)
		{
			modeFlag = table[index].current;
			key[0].keyShortDown = 0;
			break;
		}
		table[index].operation();
	}
	OLED_Clear();
	OLED_ShowString(1,1,"OK!    ",0);
}

/**
 * @brief һ���˵�
 * @param  None
 * @retval  None
 **/
void menu1_1(void)
{
	OLED_ShowString(1,1,"Simple   ",1);
	OLED_ShowString(2,1,"Stabilize",0);
	OLED_ShowString(3,1,"Circle   ",0);
	OLED_ShowString(4,1,"Free     ",0);
}

/**
 * @brief һ���˵�
 * @param  None
 * @retval  None
 **/
void menu1_2(void)
{
	OLED_ShowString(1,1,"Simple   ",0);
	OLED_ShowString(2,1,"Stabilize",1);
	OLED_ShowString(3,1,"Circle   ",0);
	OLED_ShowString(4,1,"Free     ",0);
}

/**
 * @brief һ���˵�
 * @param  None
 * @retval  None
 **/
void menu1_3(void)
{
	OLED_ShowString(1,1,"Simple   ",0);
	OLED_ShowString(2,1,"Stabilize",0);
	OLED_ShowString(3,1,"Circle   ",1);
	OLED_ShowString(4,1,"Free     ",0);
}

/**
 * @brief һ���˵�
 * @param  None
 * @retval  None
 **/
void menu1_4(void)
{
	OLED_ShowString(1,1,"Simple   ",0);
	OLED_ShowString(2,1,"Stabilize",0);
	OLED_ShowString(3,1,"Circle   ",0);
	OLED_ShowString(4,1,"Free     ",1);
}

/**
 * @brief �˵�����
 */
menu_table_t table[]=
{
	//��һ��
	{0,1,menu1_1},
	{1,2,menu1_2},
	{2,3,menu1_3},
	{3,0,menu1_4}
};

