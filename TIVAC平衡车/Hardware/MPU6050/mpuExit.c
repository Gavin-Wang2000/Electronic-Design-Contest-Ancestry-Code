#include "mpuExit.h"

/**
 * @brief mpu6050外部中断初始化
 * @param  None
 * @retval  None
 **/
void mpuExit_Init(void)
{
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
   //设置Pin为中断输入引脚
   GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
   //设置引脚中断触发类型
   GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
   //注册GPIO外部中断的中断服务程序
   GPIOIntRegister(GPIO_PORTE_BASE, mpuExit_Handler);
   //使能指定引脚的中断
   GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0);
   //使能一个中断源的中断（中断控制器层面的使能）
   IntEnable(INT_GPIOE);
   //使能一个中断源的中断（处理器层面的使能）
   IntMasterEnable();
   //清除指定中断源的标志位
   GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);
   //设置中断优先级
   IntPrioritySet(INT_GPIOE,0x00);
}


/**
 * @brief mpu外部中断服务函数
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
	 	/*按键扫描*/
		keyScan();
	
		/*角度读取*/
		mpu_dmp_get_data(&Pitch, &Roll, &Yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	   
	    /*编码器读取*/
	    speedRead(&leftSpeed,&rightSpeed);
	   
	   /*打印数据*/
		//OLED_Print(1,1,"Pitch:%.2f",Kalman_filtering(Pitch));
	    
	   
	   /*主控制函数*/
	   mainControl();
	   
   }
}