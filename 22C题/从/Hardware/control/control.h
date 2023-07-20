#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

/*PID参数*/
#define VeloctityKp 10
#define VeloctityKi 3
#define VeloctityKd 0

#define lineKp -3
#define lineKi 0
#define lineKd -60//-25

#define distanceKp 20
#define distanceKi 0
#define distanceKd 0

/*运动参数*/
#define goInner -4  //走内圈中值
#define goLine  0   //非内圈
#define TargetDistance 400 //两车间距

/*模式选择*/
#define question 0

extern uint8_t stopFlag;
extern uint8_t turnFlag;
extern int turnNum ;

void mainControl(void);
#endif