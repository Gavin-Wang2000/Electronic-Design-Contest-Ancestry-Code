#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

/*PID����*/
#define VeloctityKp 10
#define VeloctityKi 3
#define VeloctityKd 0

#define lineKp -3
#define lineKi 0
#define lineKd -60//-25

#define distanceKp 20
#define distanceKi 0
#define distanceKd 0

/*�˶�����*/
#define goInner -4  //����Ȧ��ֵ
#define goLine  0   //����Ȧ
#define TargetDistance 400 //�������

/*ģʽѡ��*/
#define question 0

extern uint8_t stopFlag;
extern uint8_t turnFlag;
extern int turnNum ;

void mainControl(void);
#endif