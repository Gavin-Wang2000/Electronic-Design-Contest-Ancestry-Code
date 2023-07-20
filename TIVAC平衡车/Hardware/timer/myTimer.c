#include "myTimer.h"
int myTime;

/**
 * @brief ��ʱ��
 * @param  
 * @retval  
 **/
void Timer_Init(void)
{
    //��ʱ��0ʹ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);	
	//32λ���ڶ�ʱ��(ȫ������)
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);	
	//�趨װ��ֵ,��80M/50��*1/80M=20ms		
    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/50);		
	//���ж�ʹ��	
    IntEnable(INT_TIMER0A);	
	//�ж����, ����ģʽ;	
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	//�жϺ���ע��
    TimerIntRegister(TIMER0_BASE,TIMER_A,TIMER0A_Handler);		
    IntMasterEnable();		
	//��ʱ��ʹ�ܿ�ʼ����
    TimerEnable(TIMER0_BASE,TIMER_A); 												
}

void TIMER0A_Handler(void)
{
	myTime++;

	
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}
