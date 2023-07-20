#ifndef __MAIN_H
#define __MAIN_H

/*系统头文件*/
#include "driverlib.h"
#include "System.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "sensor.h"
#include "buzzer.h"
#include "adc.h"
#include "Mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpui2c.h"
#include "mpuexit.h"
#include "menu.h"

/*定义全局变量*/
extern float pitch,roll,yaw;
extern int leftSpeed,rightSpeed;
extern int speedOut; 
extern int speedSum;
extern int pathLenth;
extern int targtSpeed;
extern int startFlag;
extern uint8_t sensor[8];
extern int myTime;
extern bool turnFlag;
extern uint8_t moveMode;
extern int showIndex ;
extern int Road_indexs,Stop_Time,Now_Time,Last_Time,Time_Diff;
#endif