#include "adc.h"

uint16_t ADCVaule;
/**
 * @brief ADC��ʼ��
 * @param  None
 * @retval  None
 **/
 
 void ADC_init(void)
 {
	/* ���ø��������FPU */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();
	 
	
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_4,ADC_DIVIDER_4,0);
	
	/* 14λ�ֱ���  */
	ADC14_setResolution(ADC_14BIT);//14λ
	
	/* ����GPIO����ͨ��(5.5 A0),5.4(A1) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,GPIO_TERTIARY_MODULE_FUNCTION);
	 
	 
	/* ����ADCת���洢�� */
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);//�ظ�ת��
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A0,false);

	/*  ʹ��ADC������     */
  	ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
		
    /* ʹ���ж�   */
    MAP_ADC14_enableInterrupt(ADC_INT0);
	 
    MAP_ADC14_enableConversion();//ʹ��ת��
	ADC14_toggleConversionTrigger();

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();
 }
 
 /**
 * @brief ADC�жϷ�����
 * @param  None
 * @retval  None
 **/
 void ADC14_IRQHandler(void)
 {
	uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();//��ȡʹ���ж�״̬
    MAP_ADC14_clearInterruptFlag(status);//�����־

    if (status & ADC_INT0)
    {
       ADCVaule = MAP_ADC14_getResult(ADC_MEM0);//�õ�ת�����
	   ADC14_toggleConversionTrigger();
    }
 }
 