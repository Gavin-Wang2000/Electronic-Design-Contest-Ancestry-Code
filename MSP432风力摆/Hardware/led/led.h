#ifndef __LED_H
#define __LED_H
#include "main.h"

// 位带操作
#define LED_RED BITBAND_PERI(P1OUT, 0)
#define LED_R BITBAND_PERI(P2OUT, 0)
#define LED_G BITBAND_PERI(P2OUT, 1)
#define LED_B BITBAND_PERI(P2OUT, 2)

void LED_Init(void); // LED初始化

/*********右边为固定红灯*****************/
void LED_RED_On(void);  //开
void LED_RED_Off(void); //关
void LED_RED_Tog(void); //翻转

/*********左边为可变色灯*****************/
void LED_Y_On(void);
void LED_C_On(void);
void LED_P_On(void);

void LED_R_On(void);
void LED_G_On(void);
void LED_B_On(void);

void LED_R_Off(void);
void LED_G_Off(void);
void LED_B_Off(void);

void LED_R_Tog(void);
void LED_G_Tog(void);
void LED_B_Tog(void);

void LED_W_On(void);
void LED_W_Off(void);
void LED_W_Tog(void);

#endif
