#include "sys.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif
float Pitch,Roll,Yaw;//�Ƕ�
short aacx,aacy,aacz;//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;//������ԭʼ����


int main(void)
{
	System_Init();
	delay_Init();
	delay_ms(1000);//���������ϵ�ʱ��
	
	led_Init();
//	key_Init();
	OLED_Init();
	InitI2c0();  //ʹ��iic�˿�
	motor_Init();
	motor_pwm_Init();
	Encoder_Init();
    MPU_Init();//��ʼ��6050
    while (mpu_dmp_init())//����dmp��
        ;
	mpuExit_Init();
	//Timer_Init();
	
    while(1)
    {
		
    }
}
