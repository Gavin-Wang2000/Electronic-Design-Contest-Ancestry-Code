#ifndef __KEY_H 
#define __KEY_H 
 
#include "main.h"

/*����ɨ���Ƿ�ʹ�ö�ʱ��*/
#define USE_TIME 0

struct keys
{
	unsigned char keyFlag;
	unsigned char keyTime;
	bool keyState;
	bool keyShortDown;
	bool keyLongDown;
};

extern struct keys key[2];


void key_init(void);
void keyScan(void);

 
 #endif //__KEY_H
