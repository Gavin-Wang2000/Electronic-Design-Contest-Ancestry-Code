#include "sys.h"

int main()
{
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	Delay_Init();
	Led_Init();
	OLED_Init();
	Servo_Init();
	Key_Init();
	Openmv_usart_init(115200);
	Buletooth_Init(9600);
	
	TIM_SetCompare2(TIM8,servoyRight); //y归位
	TIM_SetCompare1(TIM8,servoxRight); //x归位
	
	while(ball.cx == 0 || ball.cy == 0)
		;
	
	while(ballMove.mode == 0)
	    ;
	
	TIM4_Init();//20ms中断

	
	while(1)
	{
		/*界面显示*/
		static unsigned char oledIndex = 0;
		if(oledIndex == 0)
		{
			oledPrint(1,1,"x:%3d",ball.cx);
			oledPrint(2,1,"y:%3d",ball.cy);
			oledPrint(3,1,"bluetooth:%d",ballMove.mode);
			oledPrint(4,1,"Time:%d   s",oneSecond);
		}
		else if(oledIndex == 1)
		{
			oledPrint(1,1,"xKp:%.1f",xKp);
			oledPrint(2,1,"xKi:%.2f",xKi);
			oledPrint(3,1,"xKd:%.1f",xKd);
		}else if(oledIndex == 2)
		{
			oledPrint(1,1,"yKp:%.1f",yKp);
			oledPrint(2,1,"yKi:%.2f",yKi);
			oledPrint(3,1,"yKd:%.1f",yKd);
		}
		
		/*按键切换界面*/
		if(key[0].keyShortDown == 1)
		{
			oledIndex ++;
			if(oledIndex > 2)
			{
				oledIndex = 0;
			}
			OLED_Clear();
			key[0].keyShortDown = 0;
			
		}
		
	}
}