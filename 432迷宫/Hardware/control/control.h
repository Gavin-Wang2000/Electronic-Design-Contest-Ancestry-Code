#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

/*PID参数*/
#define VeloctityKp -7  //-7  -0.2
#define VeloctityKi -0.2
#define VeloctityKd 0

#define yawKp -0.35
#define yawKi 0
#define yawKd -0.25

#define turnKp 0.1
#define turnKi 0
#define turnKd 0

/*运动模式*/
#define goLeft 1
#define goRight 0
#define goBack 2

/*是否使用陀螺仪转向*/
#define isYaw 0

struct Main_date
{
    int dir;
	int Sp_Flag;
	int Nums;
};
extern struct Main_date Road_date[50];
extern uint8_t backMoveMode;


void mainControl(void);
int velocityPid(int nowSpeed,int targetSpeed);
int yawPid(float nowYaw,float targetYaw);
uint8_t judgeCrossing(void);
void goLine(void);
int MyAbs(int p);
uint8_t Back_judgeCrossing(void);
void Back_mainControl(void);
#endif