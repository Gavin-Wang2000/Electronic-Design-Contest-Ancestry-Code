#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

int myAbs(int x);
void is_stable(uint8_t standard);
void mainControl(void);
uint8_t isBounds(void);

/*单摆周期*/
#define PERIOD 1300 
/*静止时陀螺仪角度*/
#define START_ROLL -1
#define START_PITCH 1
/*节点距离地面高度*/
#define HEIGHT 750
/*圆周率*/
#define PI 3.14159265
/*根号2*/
#define COEFFICIENT 1.4142
/*PID参数相关*/
//振荡 -0.3 0 -23.5
#define xKp 0 
#define xKi 0
#define xKd 0

//振荡 -0.4 0 -18.5
#define yKp -1.5
#define yKi 0
#define yKd 0

extern float Measure_X,Measure_Y;
extern int Motor_X,Motor_Y;
extern float Target_X,Target_Y;
extern float Amplitude_x,Amplitude_y,Phase,Alpha;  

#endif
