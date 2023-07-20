#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "stdio.h"	
#include "sys.h" 

#pragma pack(1)
typedef struct
{
	uint8_t mode;
}bluetoothData;
#pragma pack()

extern bluetoothData ballMove;
extern int blueTooth[4];

void Buletooth_Init(u32 bound);
void UART4_IRQHandler(void);

#endif


