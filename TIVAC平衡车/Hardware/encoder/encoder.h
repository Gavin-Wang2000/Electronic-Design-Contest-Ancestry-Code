#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "sys.h"


void Encoder_Init(void);
void speedRead(int32_t *a,int32_t *b);
void QEI0_IRQHandler(void);


#endif
