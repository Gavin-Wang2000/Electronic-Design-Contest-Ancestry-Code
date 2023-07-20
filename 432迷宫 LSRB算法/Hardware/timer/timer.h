#ifndef __TIMER_H 
#define __TIMER_H 

#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "control.h"


void timer_init(uint32_t psc,uint32_t arr);

#endif 




