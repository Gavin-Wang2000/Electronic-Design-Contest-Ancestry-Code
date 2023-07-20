#include "myeeprom.h"

#define WP_FLASH_BASE PARAMETER_TABLE_STARTADDR
FLIGHT_PARAMETER Trackless_Params;

/**
 * @brief ��Ƭ���Դ�2K����EEPROM��ʼ��
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
 * @brief EEPROMд��
 * @param ������ �洢ֵ��ַ
 * @retval  None
 **/
void EEPROM_Write(int index,float* writeData)
{
	EEPROMProgram((uint32_t *)(writeData),(4 * index),4);
}

/**
 * @brief EEPROM��ȡ
 * @param ������ ��ȡ��ŵ�ַ
 * @retval  None
 **/
void EEPROM_Read(int index,float* readData)
{
	EEPROMRead((uint32_t *)(readData),(4 * index),4);
}



