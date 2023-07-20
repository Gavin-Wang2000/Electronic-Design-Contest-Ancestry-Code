#ifndef __SYS_H
#define __SYS_H

/*ϵͳͷ�ļ�*/
#include <stdint.h>
#include <stdbool.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "stdio.h"
#include "stdarg.h"
#include "rt_heap.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "sysctl.h"
#include "systick.h"
#include "fpu.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "pwm.h"
#include "gpio.h"
#include "uart.h"
#include "pin_map.h"
#include "timer.h"
#include "ssi.h"
#include "uartstdio.h"
#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_sysctl.h"
#include "hw_types.h"
#include "debug.h"
#include "gpio.h"
#include "interrupt.h"
#include "cpu.h"
#include "rom.h"
#include "sysctl.h"
#include "hw_i2c.h"
#include "i2c.h"
#include "eeprom.h"
#include "qei.h"
#include "pin_map.h"
#include "hw_nvic.h"

/*�Զ����ļ�*/
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "key.h"
#include "myTimer.h"
#include "openmv.h"
#include "motor.h"
#include "encoder.h"
#include "6050.h"
#include "driver_iic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "myeeprom.h"
#include "mpuExit.h"
#include "control.h"

/*ȫ�ֱ���*/
extern float Pitch,Roll,Yaw;//�Ƕ�
extern short aacx,aacy,aacz;//���ٶȴ�����ԭʼ����
extern short gyrox,gyroy,gyroz;//������ԭʼ����
extern int leftSpeed,rightSpeed;//�������ٶ�

void System_Init(void);

#endif