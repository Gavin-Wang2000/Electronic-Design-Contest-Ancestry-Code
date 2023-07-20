#include "sys.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif
float Pitch,Roll,Yaw;//角度
short aacx,aacy,aacz;//加速度传感器原始数据
short gyrox,gyroy,gyroz;//陀螺仪原始数据


int main(void)
{
	System_Init();
	delay_Init();
	delay_ms(1000);//给陀螺仪上电时间
	
	led_Init();
//	key_Init();
	OLED_Init();
	InitI2c0();  //使能iic端口
	motor_Init();
	motor_pwm_Init();
	Encoder_Init();
    MPU_Init();//初始化6050
    while (mpu_dmp_init())//配置dmp库
        ;
	mpuExit_Init();
	//Timer_Init();
	
    while(1)
    {
		
    }
}
