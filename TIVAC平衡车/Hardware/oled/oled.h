#ifndef __OLED_H
#define __OLED_H

#include "sys.h"


#define SCL_PORT 	GPIO_PORTA_BASE
#define SCL_PIN		GPIO_PIN_5
#define SDA_PORT	GPIO_PORTA_BASE
#define SDA_PIN		GPIO_PIN_4


#define OLED_W_SCL(x)	(x>0?GPIOPinWrite(SCL_PORT, SCL_PIN, SCL_PIN):GPIOPinWrite(SCL_PORT, SCL_PIN, 0))
#define OLED_W_SDA(x)	(x>0?GPIOPinWrite(SDA_PORT, SDA_PIN, SDA_PIN):GPIOPinWrite(SDA_PORT, SDA_PIN, 0))


void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t Y, uint8_t X);
void OLED_WriteCommand(uint8_t Command);
void OLED_ShowChar(uint8_t row,uint8_t col,char Chr,uint8_t inv);
void OLED_ShowString(uint8_t row,uint8_t col,char *str,uint8_t inv);
int OLED_Print(uint8_t row,uint8_t col,const char *formate,...);
int OLED_loadbar(uint8_t page,uint8_t percent);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
int showCHN(unsigned char row,unsigned char col,uint8_t *chn);


#endif /*__OLED_H*/


