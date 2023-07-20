#include "adc.h"

uint16_t ADCVaule;
/**
 * @brief ADC初始化
 * @param  None
 * @retval  None
 **/
 
 void ADC_init(void)
 {
	/* 启用浮点运算的FPU */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();
	 
	
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_4,ADC_DIVIDER_4,0);
	
	/* 14位分辨率  */
	ADC14_setResolution(ADC_14BIT);//14位
	
	/* 配置GPIO输入通道(5.5 A0),5.4(A1) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,GPIO_TERTIARY_MODULE_FUNCTION);
	 
	 
	/* 配置ADC转换存储器 */
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);//重复转换
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A0,false);

	/*  使能ADC触发器     */
  	ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
		
    /* 使能中断   */
    MAP_ADC14_enableInterrupt(ADC_INT0);
	 
    MAP_ADC14_enableConversion();//使能转换
	ADC14_toggleConversionTrigger();

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();
 }
 
 /**
 * @brief ADC中断服务函数
 * @param  None
 * @retval  None
 **/
 void ADC14_IRQHandler(void)
 {
	uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();//获取使能中断状态
    MAP_ADC14_clearInterruptFlag(status);//清除标志

    if (status & ADC_INT0)
    {
       ADCVaule = MAP_ADC14_getResult(ADC_MEM0);//得到转换结果
	   ADC14_toggleConversionTrigger();
    }
 }
 