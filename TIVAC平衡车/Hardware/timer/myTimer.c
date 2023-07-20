#include "myTimer.h"
int myTime;

/**
 * @brief 定时器
 * @param  
 * @retval  
 **/
void Timer_Init(void)
{
    //定时器0使能
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);	
	//32位周期定时器(全宽周期)
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);	
	//设定装载值,（80M/50）*1/80M=20ms		
    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/50);		
	//总中断使能	
    IntEnable(INT_TIMER0A);	
	//中断输出, 设置模式;	
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	//中断函数注册
    TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		
    IntMasterEnable();		
	//定时器使能开始计数
    TimerEnable(TIMER0_BASE,TIMER_A); 												
}

void TIMER0A_Handler(void)
{
	myTime++;

	
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}
