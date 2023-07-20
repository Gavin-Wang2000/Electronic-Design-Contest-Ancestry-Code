#ifndef __OPENMV_H 
#define __OPENMV_H

#include "main.h"

typedef struct
{
	uint8_t trace;
	uint8_t crossCount;
}reciveData;

extern reciveData storage;
	
void openmv_init(void);


#endif //__OPENMV_H




