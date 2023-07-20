#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"



void motor_init(void);
void motor_pwm_init(void);
void Load(int16_t A,int16_t B);
void brake(void);
void Limit(int *motorA, int *motorB);

#endif //__MOTOR_H



