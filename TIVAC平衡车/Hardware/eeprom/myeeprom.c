#include "myeeprom.h"

#define WP_FLASH_BASE PARAMETER_TABLE_STARTADDR
FLIGHT_PARAMETER Trackless_Params;

/**
 * @brief 单片机自带2K容量EEPROM初始化
 * @param  None
 * @retval  None
 **/
void EEPROM_Init(void)
{
  /* EEPROM SETTINGS */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
  EEPROMInit(); // EEPROM start
	//EEPROMMassErase();
}


/**
 * @brief EEPROM写入
 * @param 索引号 存储值地址
 * @retval  None
 **/
void EEPROM_Write(int index,float* writeData)
{
	EEPROMProgram((uint32_t *)(writeData),(4 * index),4);
}

/**
 * @brief EEPROM读取
 * @param 索引号 读取存放地址
 * @retval  None
 **/
void EEPROM_Read(int index,float* readData)
{
	EEPROMRead((uint32_t *)(readData),(4 * index),4);
}



