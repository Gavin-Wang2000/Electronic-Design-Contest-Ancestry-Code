#include "sys.h"

/**
 * @brief 系统初始化
 * @param  None
 * @retval  None
 **/
void System_Init(void)
{
    /*开启浮点数计算*/
    FPULazyStackingEnable();
	FPUEnable();

	
	/*
	 设置系统时钟（80M）
	 Setup the system clock to run at 80 Mhz from PLL with crystal reference
	 SYSCTL_OSC_MAIN: Using Main OSC, hence we have VCO frequency=400MHz
	 SYSCTL_USE_PLL：When using the PLL, the VCO frequency is predivided by 2
	 SYSCTL_SYSDIV_2_5,the divisor SYSDIV is set to 2.5
	 The frequency at which the system clock runs can be calculated as follows:
	 400/2/2.5=80(MHz)
	*/
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);	
}