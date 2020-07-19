#ifndef _LED_H
#define _LED_H 


#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_iomuxc.h"

#define LED_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) 

#define CORE_BOARD_LED_GPIO             GPIO1
#define CORE_BOARD_LED_GPIO_PIN         (9U)
#define CORE_BOARD_LED_IOMUXC           IOMUXC_GPIO_AD_B0_09_GPIO1_IO09

#define RGB_RED_LED_GPIO                GPIO1
#define RGB_RED_LED_GPIO_PIN            (24U)
#define RGB_RED_LED_IOMUXC              IOMUXC_GPIO_AD_B1_08_GPIO1_IO24

#define RGB_GREEN_LED_GPIO              GPIO1
#define RGB_GREEN_LED_GPIO_PIN          (25U)
#define RGB_GREEN_LED_IOMUXC            IOMUXC_GPIO_AD_B1_09_GPIO1_IO25

#define RGB_BLUE_LED_GPIO               GPIO1
#define RGB_BLUE_LED_GPIO_PIN           (10U)
#define RGB_BLUE_LED_IOMUXC             IOMUXC_GPIO_AD_B0_10_GPIO1_IO10

class LED{
public:
	LED(void):GPIOx(CORE_BOARD_LED_GPIO),Pin(CORE_BOARD_LED_GPIO_PIN){
		assert(this->Pin < 32);
		this->IOMUXC_MUX_PAD_RESOURCE_Config();
		LED::RGB_Init();
	}
	void IOMUXC_MUX_PAD_RESOURCE_Config(void){
		gpio_pin_config_t config;
		config.direction     = kGPIO_DigitalOutput; 
		config.outputLogic   =  1;         
		config.interruptMode = kGPIO_NoIntmode; 

		IOMUXC_SetPinMux(CORE_BOARD_LED_IOMUXC, 0U);
		IOMUXC_SetPinConfig(CORE_BOARD_LED_IOMUXC, LED_PAD_CONFIG_DATA);  
		GPIO_PinInit(CORE_BOARD_LED_GPIO, CORE_BOARD_LED_GPIO_PIN, &config);
		
	}
	void set(uint8_t value){
		value==0?this->GPIOx->DR &= ~(1U << this->Pin):this->GPIOx->DR |= (1U << this->Pin);
	}
	static void RGB_Init(void){  
		gpio_pin_config_t config;
		config.direction     = kGPIO_DigitalOutput; 
		config.outputLogic   =  1;         
		config.interruptMode = kGPIO_NoIntmode; 
		IOMUXC_SetPinMux(RGB_RED_LED_IOMUXC   , 0U); 
		IOMUXC_SetPinMux(RGB_GREEN_LED_IOMUXC , 0U);
		IOMUXC_SetPinMux(RGB_BLUE_LED_IOMUXC  , 0U);  
		IOMUXC_SetPinConfig(RGB_RED_LED_IOMUXC   , LED_PAD_CONFIG_DATA); 
		IOMUXC_SetPinConfig(RGB_GREEN_LED_IOMUXC , LED_PAD_CONFIG_DATA); 
		IOMUXC_SetPinConfig(RGB_BLUE_LED_IOMUXC  , LED_PAD_CONFIG_DATA);
		GPIO_PinInit(RGB_RED_LED_GPIO   , RGB_RED_LED_GPIO_PIN   , &config);
		GPIO_PinInit(RGB_GREEN_LED_GPIO , RGB_GREEN_LED_GPIO_PIN , &config);
		GPIO_PinInit(RGB_BLUE_LED_GPIO  , RGB_BLUE_LED_GPIO_PIN  , &config); 
	}
	static void RGB_Set(uint8_t RGB){
		(RGB&0x04)!=0?RGB_RED_LED_GPIO->DR &= ~(1U << RGB_RED_LED_GPIO_PIN):RGB_RED_LED_GPIO->DR |= (1U << RGB_RED_LED_GPIO_PIN);
		(RGB&0x02)!=0?RGB_GREEN_LED_GPIO->DR &= ~(1U << RGB_GREEN_LED_GPIO_PIN):RGB_GREEN_LED_GPIO->DR |= (1U << RGB_GREEN_LED_GPIO_PIN);
		(RGB&0x01)!=0?RGB_BLUE_LED_GPIO->DR &= ~(1U << RGB_BLUE_LED_GPIO_PIN):RGB_BLUE_LED_GPIO->DR |= (1U << RGB_BLUE_LED_GPIO_PIN);
	}
private:
	GPIO_Type* GPIOx;
	uint32_t   Pin;
};

#endif			