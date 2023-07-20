#ifndef __DELAY_H
#define __DELAY_H
#include "sys.h"

typedef struct
{
  float Last_Time;
  float Now_Time;
  float Time_Delta;
  uint16_t Time_Delta_INT;//µ¥Î»ms
}Testime;

void delay_Init(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
uint32_t millis(void);
uint32_t micros(void);
void Delay_Ms(uint32_t x);
void Delay_Us(uint32_t x);

void delay_ms(uint32_t x);
void delay_us(uint32_t x);
void Test_Period(Testime *Time_Lab);

#endif