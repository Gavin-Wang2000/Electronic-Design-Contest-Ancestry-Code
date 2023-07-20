#include "main.h"

float pitch,roll,yaw;
int leftSpeed,rightSpeed;

int main()
{
	SysInit();//打开时钟
	delay_init();
	LED_Init();
	buzzer_init();
	key_init();
	OLED_Init();
	OLED_Clear();
#if isYaw
	MPU_Init();
	while(mpu_dmp_init())
			;
#endif
	motor_init();
	motor_pwm_init();
	encoder_init();
	sensor_init();
	timer_init(TIMER32_PRESCALER_1,119999);//2.5ms
	zigbee_init();
	int num;
	
	while(1)
	{
	  main_menu();
	  if(startCaculate == 1)
	  {
		Timer32_disableInterrupt(TIMER32_0_BASE);
		/*计算最短路径*/
		caculateShortPath(mazeArray,roadIndex + 1);	
		startFlag = 2;
		LED_W_On();
		Timer32_enableInterrupt(TIMER32_0_BASE);
      }
	}
}
