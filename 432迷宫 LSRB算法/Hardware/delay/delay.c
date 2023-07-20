#include "delay.h"

static uint8_t fac_us = 0;	


void delay_init()
{
	//系统时钟
	fac_us = CS_getMCLK() / 1000000;	
	//内部时钟源	
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; 
}


void delay_us(uint32_t t)
{
	uint32_t temp;
	
	//时间加载
	SysTick->LOAD = t * fac_us;		
	//清空计数器
	SysTick->VAL = 0x00;			
	//开始倒数
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 
	//等待时间到达
	do
	{
		temp = SysTick->CTRL;
		
	} while ((temp & 0x01) && !(temp & (1 << 16))); 
	//关闭计数器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		
	//清空计数器
	SysTick->VAL = 0X00;							
}



void delay_ms(uint32_t t)
{
	while(t--)
	{
		delay_us(1000);
	}
}
