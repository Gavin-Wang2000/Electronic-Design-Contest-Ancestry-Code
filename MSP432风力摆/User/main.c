#include "main.h"
int main()
{
	SysInit();//打开时钟
	delay_init();
	
	delay_ms(1000);//给陀螺仪上电时间
	
	LED_Init();
	Laser_Init();
	key_init();
	OLED_Init();
	OLED_Clear();
	MPU_Init();
	while(mpu_dmp_init())
			;
	/*菜单交互*/
	mainMeun();
	
	//MPU6050_EXTI_Init();
	motor_pwm_init();
	
	while(1)
	{
		
	}
}
