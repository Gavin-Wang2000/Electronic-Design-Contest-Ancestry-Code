#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define Integraldead_zone 15

#define XKP 4
#define XKI 0.02 //0.02
#define XKD 25  

#define YKP 4
#define YKI 0.007//0.007
#define YKD 30

#define servoxRight 1450
#define servoyRight 1500

#define zone1X 30
#define zone1Y 15

#define zone2X 15
#define zone2Y 15

#define zone3X 5
#define zone3Y 15

#define zone4X 30
#define zone4Y -10

#define zone5X 15
#define zone5Y -10

#define zone6X 5
#define zone6Y -10

#define zone7X 30
#define zone7Y -30

#define zone8X 15
#define zone8Y -30

#define zone9X 5
#define zone9Y -30

extern float xKp;
extern float xKi;
extern float xKd;

extern float yKp;
extern float yKi;
extern float yKd;


int xPid(int nowX,int targetX);
int yPid(int nowY,int targetY);
void mainControl(void);
void questionOne(void);
void questionTwo(void);
void questionThree(void);
void questionFour(void);
#endif
