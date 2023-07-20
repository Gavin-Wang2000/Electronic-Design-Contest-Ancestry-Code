#ifndef __OPENMV_H
#define __OPENMV_H

#include "sys.h"

#pragma pack(1)
typedef struct
{
	int8_t cx;
	int8_t cy;
}openmvData;
#pragma pack()

extern openmvData trace;

void openmvUsart_init(int baud);
void USART0_IRQHandler(void);

#endif