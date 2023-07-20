#ifndef __BUZZER_H 
#define __BUZZER_H

#include "main.h"

#define buzzer_on() 	GPIO_setOutputLowOnPin(GPIO_PORT_P7,GPIO_PIN7)
#define buzzer_off() 	GPIO_setOutputHighOnPin(GPIO_PORT_P7,GPIO_PIN7)

void buzzer_init(void);

#endif //__BUZZER_H




