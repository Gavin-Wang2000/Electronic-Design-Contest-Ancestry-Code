#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

void motor_Init(void);
void motor_pwm_Init(void);
void Load(int16_t A, int16_t B);
void Limit(int *motoA,int *motoB);
void brake(void);

#endif