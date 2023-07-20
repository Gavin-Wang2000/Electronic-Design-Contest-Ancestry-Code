#include "timer.h"

/**
 * @brief 定时器32初始化
 * @param None
 * @retval None
 */
void timer_init(uint32_t psc, uint32_t arr)
{
	//初始化为32位周期计数模式
	Timer32_initModule(TIMER32_0_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

	//设置ARR自动重装载值
	Timer32_setCount(TIMER32_0_BASE, arr);

	//配置定时器32开始连续计数
	Timer32_startTimer(TIMER32_0_BASE, false);

	//清除中断标志位
	Timer32_clearInterruptFlag(TIMER32_0_BASE);

	//使能定时器32中断
	Timer32_enableInterrupt(TIMER32_0_BASE);

	//开启定时器32端口中断
	Interrupt_enableInterrupt(INT_T32_INT1);

	//开启总中断
	Interrupt_enableMaster();
}

/**
 * @brief 定时器32中断服务函数
 * @param None
 * @retval None
 */
int Stop_Flag, Stop_Time, Now_Time, Last_Time, Time_Diff;
void T32_INT1_IRQHandler(void)
{
	Timer32_clearInterruptFlag(TIMER32_0_BASE);

	/*按键读取*/
	keyScan();

	/*红外读取*/
	sensorRade();

	/*运算*/
	encoder_read(&leftSpeed, &rightSpeed);
	speedSum = leftSpeed + rightSpeed;
	myTime++;
	Stop_Time++;
	/*主控制函数*/
	if (startFlag == 1)
	{
		mainControl();
	}
	else if (startFlag == 2)
	{
		Stop_Flag++;
		if (moveMode == 6 && Stop_Flag == 1)
		{
			showIndex = 2;
			moveMode = 0;
			Stop_Flag++;
		}
		Back_mainControl();
	}
}
