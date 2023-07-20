#ifndef __SERVO_H
#define __SERVO_H

#include "sys.h"
#define straight 85

void Servo_Init(void);
void Servo1_SetAngle(float Angle);
void Servo2_SetAngle(float Angle);

#endif
