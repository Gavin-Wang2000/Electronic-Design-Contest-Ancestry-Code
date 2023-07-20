#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

/*ֱ����* -4 -0.03*/
#define Vertical_Kp -4
#define Vertical_Kd -0.03

/*�ٶȻ� 0.6 0.0045*/
#define Velocity_Kp 0.6
#define Velocity_Ki 0.0045

/*ת��*/
#define turnKp 0

void mainControl(void);
float Kalman_filtering(float AOA_angle);

#endif