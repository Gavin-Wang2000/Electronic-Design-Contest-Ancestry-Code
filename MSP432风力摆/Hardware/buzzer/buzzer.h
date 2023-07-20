#ifndef __BUZZER_H 
#define __BUZZER_H

#include "main.h"

#define buzzer_on() 	GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN3)
#define buzzer_off() 	GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN3)

void buzzer_init(void);

#endif //__BUZZER_H




