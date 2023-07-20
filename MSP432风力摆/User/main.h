#ifndef __MAIN_H
#define __MAIN_H

/*系统头文件*/
#include "driverlib.h"
#include "System.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

/*个人头文件*/
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
#include "buzzer.h"
#include "menu.h"
#include "openmv.h"
#include "Mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpui2c.h"
#include "mpuexit.h"
#include "filter.h"
#include "laser.h"

/*定义全局变量*/
extern float pitch,roll,yaw;
extern uint8_t stableFlag;
extern uint32_t TimeCnt;
extern int modeFlag;
#endif
