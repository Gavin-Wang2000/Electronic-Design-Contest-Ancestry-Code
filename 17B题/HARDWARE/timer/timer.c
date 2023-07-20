#include "timer.h"

void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	// ������ʱ��ʱ��,���ڲ�ʱ�� CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
	TIM_TimeBaseStructure.TIM_Period=199;
	// �ۼ� TIM_Period ��Ƶ�ʺ����һ�����»����ж�
	// ʱ��Ԥ��Ƶ��Ϊ 71����������������ʱ�� CK_CNT = CK_INT / (71+1)=1M
	TIM_TimeBaseStructure.TIM_Prescaler= 7199;
	// ʱ�ӷ�Ƶ���� 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	// ����������ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// �ظ���������ֵ
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	
	
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
	// ���������ȼ�Ϊ 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	// ���������ȼ�Ϊ 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	// ����������жϱ�־λ
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	// �����������ж�
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	// ʹ�ܼ�����
	TIM_Cmd(TIM4, ENABLE);
}


unsigned int oneSecond;

/**
 * @brief ��ʱ��4�жϷ�����
 * @param None
 * @retval None
 */
void TIM4_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM4,TIM_IT_Update) != RESET ) 
	{	
		/*ÿ���ʱ*/
		static unsigned int myTime;
		myTime++;
		if(myTime >= 50)
		{
			oneSecond++;
			myTime = 0;
		}
		
		/*����ɨ��*/
		keyScan();
		
		/*������ִ��*/
		mainControl();
		
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
	}
}

