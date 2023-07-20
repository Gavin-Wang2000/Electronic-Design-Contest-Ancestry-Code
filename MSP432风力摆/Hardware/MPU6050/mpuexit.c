#include "mpuexit.h"

float pitch,roll,yaw;//�Ƕ�
short aacx,aacy,aacz;//���ٶ�
short gyrox,gyroy,gyroz;//������ԭʼ����
float filterPitch;

/**
 * @brief �������ⲿ�жϳ�ʼ��
 * @param  None
 * @retval  None
 **/
void MPU6050_EXTI_Init(void)
{
	/*����6.4��Ϊ��������*/
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
	/*���IO�ڵ��жϱ�־λ*/
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
	/*�����ж�ģʽ*/
	MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P6,GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
	/*ʹ��P6.4���ⲿ�жϣ�����ֻ��һ�����ţ�*/
    MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
	/*ʹ��P1�˿ڵ��жϣ�������һ����˿ڣ�����������ţ�*/
    MAP_Interrupt_enableInterrupt(INT_PORT6);
}

/**
 * @brief �ⲿ�жϷ�����
 * @param  None
 * @retval  None
 **/
void PORT6_IRQHandler(void)
{
	uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    /*��⵽P6.4�����ⲿ�жϵĴ���*/
    if(status & GPIO_PIN4)
    {
		/*����ɨ��*/
		keyScan();
		
		/*��ȡ������ֵ*/
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
			
//		/*�Ƕ���ʾ*/
//		OLED_Print(1,1,"pitch:%.2f",pitch);
//		OLED_Print(2,1,"roll :%.2f",roll );
//		OLED_Print(3,1,"Target_X:%d",isBounds());
		
		/*�ȴ���Ʈ���������2��*/
		is_stable(2);
		if(stableFlag == 5)
		{
			Laser_On();
			mainControl();
		}
    }
}