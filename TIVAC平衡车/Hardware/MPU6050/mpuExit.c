#include "mpuExit.h"

/**
 * @brief mpu6050�ⲿ�жϳ�ʼ��
 * @param  None
 * @retval  None
 **/
void mpuExit_Init(void)
{
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
   //����PinΪ�ж���������
   GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
   //���������жϴ�������
   GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
   //ע��GPIO�ⲿ�жϵ��жϷ������
   GPIOIntRegister(GPIO_PORTE_BASE, mpuExit_Handler);
   //ʹ��ָ�����ŵ��ж�
   GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0);
   //ʹ��һ���ж�Դ���жϣ��жϿ����������ʹ�ܣ�
   IntEnable(INT_GPIOE);
   //ʹ��һ���ж�Դ���жϣ������������ʹ�ܣ�
   IntMasterEnable();
   //���ָ���ж�Դ�ı�־λ
   GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);
   //�����ж����ȼ�
   IntPrioritySet(INT_GPIOE,0x00);
}


/**
 * @brief mpu�ⲿ�жϷ�����
 * @param  None
 * @retval  None
 **/
void mpuExit_Handler(void)
{
   uint32_t status;
   status = GPIOIntStatus(GPIO_PORTE_BASE, true);
   GPIOIntClear(GPIO_PORTE_BASE, status);
   if ((status & GPIO_PIN_0) == GPIO_PIN_0)
   {
	 	/*����ɨ��*/
		keyScan();
	
		/*�Ƕȶ�ȡ*/
		mpu_dmp_get_data(&Pitch, &Roll, &Yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	   
	    /*��������ȡ*/
	    speedRead(&leftSpeed,&rightSpeed);
	   
	   /*��ӡ����*/
		//OLED_Print(1,1,"Pitch:%.2f",Kalman_filtering(Pitch));
	    
	   
	   /*�����ƺ���*/
	   mainControl();
	   
   }
}