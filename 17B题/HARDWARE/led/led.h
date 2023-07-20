#ifndef __LED_H_
#define __LED_H_

#include "sys.h"

#define LED1_ON  GPIO_ResetBits(GPIOA,GPIO_Pin_8) 
#define LED2_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_2) 
#define LED3_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_3) 

#define LED1_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_8) 
#define LED2_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_2) 
#define LED3_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_3) 

void Led_Init(void);
#endif



