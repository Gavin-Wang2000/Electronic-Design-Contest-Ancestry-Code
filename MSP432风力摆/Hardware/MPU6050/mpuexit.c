#include "mpuexit.h"

float pitch,roll,yaw;//角度
short aacx,aacy,aacz;//加速度
short gyrox,gyroy,gyroz;//陀螺仪原始数据
float filterPitch;

/**
 * @brief 陀螺仪外部中断初始化
 * @param  None
 * @retval  None
 **/
void MPU6050_EXTI_Init(void)
{
	/*配置6.4口为上拉输入*/
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
	/*清除IO口的中断标志位*/
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
	/*配置中断模式*/
	MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P6,GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
	/*使能P6.4的外部中断（这里只是一个引脚）*/
    MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
	/*使能P1端口的中断（这里是一个大端口，包括多个引脚）*/
    MAP_Interrupt_enableInterrupt(INT_PORT6);
}

/**
 * @brief 外部中断服务函数
 * @param  None
 * @retval  None
 **/
void PORT6_IRQHandler(void)
{
	uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    /*检测到P6.4产生外部中断的处理*/
    if(status & GPIO_PIN4)
    {
		/*按键扫描*/
		keyScan();
		
		/*获取陀螺仪值*/
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
			
//		/*角度显示*/
//		OLED_Print(1,1,"pitch:%.2f",pitch);
//		OLED_Print(2,1,"roll :%.2f",roll );
//		OLED_Print(3,1,"Target_X:%d",isBounds());
		
		/*等待零飘消除，误差2度*/
		is_stable(2);
		if(stableFlag == 5)
		{
			Laser_On();
			mainControl();
		}
    }
}