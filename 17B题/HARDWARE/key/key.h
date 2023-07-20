#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"
struct keys
{
	unsigned char keyFlag;
	unsigned int keyTime;
	bool keyState;
	bool keyShortDown;
	bool keyLongDown;
};

extern struct keys key[2];

void Key_Init(void);
void keyScan(void);

#endif
