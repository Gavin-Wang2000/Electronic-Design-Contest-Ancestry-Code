#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

/*直立环* -4 -0.03*/
#define Vertical_Kp -4
#define Vertical_Kd -0.03

/*速度环 0.6 0.0045*/
#define Velocity_Kp 0.6
#define Velocity_Ki 0.0045

/*转向环*/
#define turnKp 0

void mainControl(void);
float Kalman_filtering(float AOA_angle);

#endif