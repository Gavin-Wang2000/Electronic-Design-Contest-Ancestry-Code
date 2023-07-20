#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

void motor_pwm_init(void);
void Load(int16_t Motor_X, int16_t Motor_Y);
void Limit(int *motorA, int *motorB);

#endif //__MOTOR_H



