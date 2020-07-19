#ifndef _ADC_H
#define _ADC_H 
#ifdef __cplusplus
extern "C"{
#endif

#include <cstdint>
#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "pad_config.h"
#include "nvic.h"

extern volatile bool     ADC_ConversionDoneFlag;
extern volatile uint32_t ADC_ConvertedValue;

#define ADC_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) 

typedef uint8_t ADC_Channel_t;
#define ADC1_IN0 	ADC1,(ADC_Channel_t)0U
#define ADC1_IN1 	ADC1,(ADC_Channel_t)1U
#define ADC1_IN2 	ADC1,(ADC_Channel_t)2U
#define ADC1_IN3 	ADC1,(ADC_Channel_t)3U
#define ADC1_IN4 	ADC1,(ADC_Channel_t)4U
#define ADC1_IN5 	ADC1,(ADC_Channel_t)5U
#define ADC1_IN6 	ADC1,(ADC_Channel_t)6U
#define ADC1_IN7 	ADC1,(ADC_Channel_t)7U
#define ADC1_IN8 	ADC1,(ADC_Channel_t)8U
#define ADC1_IN9 	ADC1,(ADC_Channel_t)9U
#define ADC1_IN10 	ADC1,(ADC_Channel_t)10U
#define ADC1_IN11 	ADC1,(ADC_Channel_t)11U
#define ADC1_IN12 	ADC1,(ADC_Channel_t)12U
#define ADC1_IN13 	ADC1,(ADC_Channel_t)13U
#define ADC1_IN14 	ADC1,(ADC_Channel_t)14U
#define ADC1_IN15 	ADC1,(ADC_Channel_t)15U

#define CORE_BOARD_ADC_GPIO 		GPIO1
#define CORE_BOARD_ADC_GPIO_PIN		(22U)
#define CORE_BOARD_ADC_IOMUXC       IOMUXC_GPIO_AD_B1_06_GPIO1_IO22

#define ADCx              ADC1
#define DEMO_ADC_CHANNEL_GROUP 		0U //ADC 通道组，最多只能使用8个通道组（0到7）
#define DEMO_ADC_USER_CHANNEL 		11U  //ADC 通道宏定义

void BOARD_InitADC(void);



#ifdef __cplusplus
}
#endif
#endif  