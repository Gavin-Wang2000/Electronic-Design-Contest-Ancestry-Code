#ifndef __MAIN_H
#define __MAIN_H

/*ϵͳͷ�ļ�*/
#include "driverlib.h"
#include "System.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*����ͷ�ļ�*/
#include "Led.h"
#include "Key.h"
#include "Servo.h"
#include "motor.h"
#include "oled.h"
#include "delay.h"
#include "encoder.h"
#include "timer.h"
#include "control.h"
#include "zigbee.h"
#include "sensor.h"
#include "buzzer.h"
#include "menu.h"
#include "ultrasonic.h"

/*����ȫ�ֱ���*/
extern int leftSpeed,rightSpeed;
extern int speedOut; 
extern int speedSum;
extern int pathLenth;
extern int targtSpeed;
extern uint8_t startFlag;
extern uint8_t sensor[9];

#endif