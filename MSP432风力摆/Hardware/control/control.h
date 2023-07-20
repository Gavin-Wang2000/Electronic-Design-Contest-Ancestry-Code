#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

int myAbs(int x);
void is_stable(uint8_t standard);
void mainControl(void);
uint8_t isBounds(void);

/*��������*/
#define PERIOD 1300 
/*��ֹʱ�����ǽǶ�*/
#define START_ROLL -1
#define START_PITCH 1
/*�ڵ�������߶�*/
#define HEIGHT 750
/*Բ����*/
#define PI 3.14159265
/*����2*/
#define COEFFICIENT 1.4142
/*PID�������*/
//�� -0.3 0 -23.5
#define xKp 0 
#define xKi 0
#define xKd 0

//�� -0.4 0 -18.5
#define yKp -1.5
#define yKi 0
#define yKd 0

extern float Measure_X,Measure_Y;
extern int Motor_X,Motor_Y;
extern float Target_X,Target_Y;
extern float Amplitude_x,Amplitude_y,Phase,Alpha;  

#endif
