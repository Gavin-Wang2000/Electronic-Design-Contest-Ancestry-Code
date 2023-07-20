#ifndef __KEY_H
#define __KEY_H

#include "sys.h"


struct keys
{
	unsigned char keyFlag;
	unsigned char keyTime;
	bool keyState;
	bool keyShortDown;
	bool keyLongDown;
};

extern struct keys key[2];


void key_Init(void);
void keyScan(void);

#endif