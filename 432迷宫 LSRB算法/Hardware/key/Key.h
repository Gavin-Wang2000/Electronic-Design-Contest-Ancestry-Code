#ifndef __KEY_H 
#define __KEY_H 
 
#include "main.h"

struct keys
{
	unsigned char keyFlag;
	unsigned char keyTime;
	bool keyState;
	bool keyShortDown;
	bool keyLongDown;
};

extern struct keys key[4];


void key_init(void);
void keyScan(void);

 
 #endif //__KEY_H