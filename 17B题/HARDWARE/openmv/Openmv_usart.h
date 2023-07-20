#ifndef __OPENMV_USART_H
#define __OPENMV_USART_H

#include "sys.h"

#pragma pack(1)
typedef struct
{
	int8_t cx;
	int8_t cy;
}reciveData;
#pragma pack()

extern reciveData ball;

void Openmv_usart_init(u32 bound);
void USART3_IRQHandler(void);


#endif


