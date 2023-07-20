#include "main.h"
int main()
{
	SysInit();
	delay_init();
	LED_Init();
	buzzer_init();
	key_init();
	OLED_Init();
	OLED_Clear();
	motor_init();
	motor_pwm_init();
	encoder_init();
	sensor_init();
	zigbee_init();
	ultrasonic_init();
	timer_init(TIMER32_PRESCALER_1,119999);//2.5ms
	
	while(1)
	{
		 mainMenu();
	}
}
