#include "adc.h"
#include "nvic.h"

#ifdef __cplusplus
extern "C"{
#endif

volatile bool     ADC_ConversionDoneFlag = false;
volatile uint32_t ADC_ConvertedValue = 0;

static void ADC_IO_Config(void){
	gpio_pin_config_t adc_config; 
	/*ADC，GPIO配置*/   
	adc_config.direction = kGPIO_DigitalInput; //输入模式
	//adc_config.outputLogic =  1;                //默认高电平，在输入模式下配置该选项无效
	adc_config.interruptMode = kGPIO_NoIntmode; //不使用中断

	GPIO_PinInit(CORE_BOARD_ADC_GPIO, CORE_BOARD_ADC_GPIO_PIN, &adc_config);
}

static void ADC_IOMUXC_Config(void){
	IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC, 0U);  
	IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC, ADC_PAD_CONFIG_DATA);   
}

void BOARD_InitADC(void){
	adc_config_t 			adcConfigStrcut;
	adc_channel_config_t 	adcChannelConfigStruct;
	ADC_IO_Config();
	ADC_IOMUXC_Config();
	ADC_GetDefaultConfig(&adcConfigStrcut);
	adcConfigStrcut.enableContinuousConversion = true;
	adcConfigStrcut.resolution = kADC_Resolution12Bit;
	ADC_Init(ADCx, &adcConfigStrcut);

	ADC_SetHardwareAverageConfig(ADCx, kADC_HardwareAverageCount32);
	adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;//设置ADC转换通道对应的外部输入通道
	adcChannelConfigStruct.enableInterruptOnConversionCompleted = true; //使能转换完成中断
	ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
	while(kStatus_Success != ADC_DoAutoCalibration(ADCx));
	/*设置中断优先级,*/
	Set_NVIC_PriorityGroup(Group_4);
	set_IRQn_Priority(ADC1_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
	EnableIRQ(ADC1_IRQn);
}

void ADC1_IRQHandler(void){
  	adc_channel_config_t adcChannelConfigStruct; 
    adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = true;

    ADC_ConversionDoneFlag = true;
    ADC_ConvertedValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
    ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
}




#ifdef __cplusplus
}
#endif

