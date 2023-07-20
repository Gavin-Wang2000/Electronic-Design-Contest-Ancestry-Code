#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

/*PID����*/
#define VeloctityKp -7  //-7  -0.2
#define VeloctityKi -0.2
#define VeloctityKd 0

#define yawKp -0.35
#define yawKi 0
#define yawKd -0.25

#define turnKp 0.1
#define turnKi 0
#define turnKd 0

/*�˶�ģʽ*/
#define goLeft 1
#define goRight 2
#define goBack 3
#define goStraight 0

/*�Ƿ�ʹ��������ת��*/
#define isYaw 0

extern char mazeArray[100];//��ȥʱ��ס��·��
extern int roadIndex;//·������
extern uint8_t backMoveMode;
extern uint8_t startCaculate;
extern uint8_t turnRightGoLineTime;//��תǰֱ�о���
extern uint8_t turnLeftGoLineTime;//��תǰֱ�о���


void mainControl(void);
int velocityPid(int nowSpeed,int targetSpeed);
int yawPid(float nowYaw,float targetYaw);
uint8_t judgeCrossing(void);
void goLine(void);
int MyAbs(int p);
uint8_t Back_judgeCrossing(void);
void Back_mainControl(void);
void caculateShortPath(char MAZE_ARRAY[], int SIZE_OF_ARRAY);
void reArrange(char a[], int n, int i);
#endif