#ifndef __LED_H
#define __LED_H
#include "sys.h"


#define LED_RED_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1)
#define LED_RED_OFF  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0)

#define LED_BLUE_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2)
#define LED_BLUE_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0)

#define LED_GREEN_ON  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3)
#define LED_GREEN_OFF GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0)


//ºì
#define LED_RED LED_RED_ON;LED_GREEN_OFF;LED_BLUE_OFF
					
//ÂÌ
#define LED_GREEN LED_RED_OFF;LED_GREEN_ON;LED_BLUE_OFF
					

//À¶
#define LED_BLUE LED_RED_OFF;LED_GREEN_OFF;LED_BLUE_ON	
						
//»Æ(ºì+ÂÌ)					
#define LED_YELLOW LED_RED_ON;LED_GREEN_ON;LED_BLUE_OFF
					
//×Ï(ºì+À¶)
#define LED_PURPLE LED_RED_ON;LED_GREEN_OFF;LED_BLUE_ON			
					
//Çà(ÂÌ+À¶)
#define LED_CYAN LED_RED_OFF;LED_GREEN_ON;LED_BLUE_ON
							
//°×(ºì+ÂÌ+À¶)
#define LED_WHITE LED_RED_ON;LED_GREEN_ON;LED_BLUE_ON			
								
//ºÚ(È«²¿¹Ø±Õ)
#define LED_RGBOFF LED_RED_OFF;LED_GREEN_OFF;LED_BLUE_OFF	

void led_Init(void);

#endif
