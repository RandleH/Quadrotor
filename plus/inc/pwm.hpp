#ifndef _PWM_HPP
#define _PWM_HPP 

#include "fsl_pwm.h"
#include "fsl_iomuxc.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_xbara.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#ifndef __50Hz__
#define __50Hz__   50
#endif
#ifndef __100Hz__
#define __100Hz__  100
#endif
#ifndef __300Hz__
#define __300Hz__  300
#endif
#ifndef __500Hz__
#define __500Hz__  500
#endif
#ifndef __1KHz__
#define __1KHz__   1000
#endif
#ifndef __5KHz__
#define __5KHz__   5000
#endif
#ifndef __10KHz__
#define __10KHz__  10000
#endif
#ifndef __50KHz__
#define __50KHz__  50000
#endif
#ifndef __100KHz__
#define __100KHz__ 100000
#endif
#ifndef __500KHz__
#define __500KHz__ 500000
#endif
#ifndef __1MHz__
#define __1MHz__   1000000
#endif


static const PWM_Type* PWM_BASE[] = {PWM1,PWM2,PWM3,PWM4};

typedef enum {
	PWM1_B00,PWM1_B01,PWM1_B02,PWM1_B03,
	PWM1_A00,PWM1_A01,PWM1_A02,PWM1_A03,
	PWM1_X00,PWM1_X01,PWM1_X02,PWM1_X03,

	PWM2_B00,PWM2_B01,PWM2_B02,PWM2_B03,
	PWM2_A00,PWM2_A01,PWM2_A02,PWM2_A03,
	PWM2_X00,PWM2_X01,PWM2_X02,PWM2_X03,

	PWM3_B00,PWM3_B01,PWM3_B02,PWM3_B03,	
	PWM3_A00,PWM3_A01,PWM3_A02,PWM3_A03,
	PWM3_X00,PWM3_X01,PWM3_X02,PWM3_X03,

	PWM4_B00,PWM4_B01,PWM4_B02,PWM4_B03,
	PWM4_A00,PWM4_A01,PWM4_A02,PWM4_A03,
	PWM4_X00,PWM4_X01,PWM4_X02,PWM4_X03,
}PWMx_Xxx_t;

#define __GET_PWMx_Xxx_BASE(X)         ((PWM_Type*)PWM_BASE[(uint8_t)((X>>2)/3)])  // [0] = PWM1;[1] = PWM2;[2] = PWM3;[3] = PWM4;
#define __GET_PWMx_Xxx_SUBMODULE(X)    ((pwm_submodule_t)(X%4))                    // [0] = _00 ;[1] = _01 ;[2] = _02 ;[3] = _03 ; 
#define __GET_PWMx_Xxx_CHANNEL(X)      ((pwm_channels_t)((X>>2)%3))                // [0] = A   ;[1] = B   ;[2] = X   ;

#define PWM1_PWMA00_GPIO       GPIO3
#define PWM1_PWMA00_GPIO_PIN   (12U)
#define PWM1_PWMA00_IOMUXC     IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00  
#define PWM1_PWMB00_GPIO       GPIO3
#define PWM1_PWMB00_GPIO_PIN   (13U)
#define PWM1_PWMB00_IOMUXC     IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00  
#define PWM1_PWMX00_GPIO       GPIO1
#define PWM1_PWMX00_GPIO_PIN   (2U)
#define PWM1_PWMX00_IOMUXC     IOMUXC_GPIO_AD_B0_02_FLEXPWM1_PWMX00 

#define PWM1_PWMA01_GPIO       GPIO3
#define PWM1_PWMA01_GPIO_PIN   (14U)
#define PWM1_PWMA01_IOMUXC     IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01 
#define PWM1_PWMB01_GPIO       GPIO3
#define PWM1_PWMB01_GPIO_PIN   (15U)
#define PWM1_PWMB01_IOMUXC     IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01
#define PWM1_PWMX01_GPIO       GPIO1
#define PWM1_PWMX01_GPIO_PIN   (3U)
#define PWM1_PWMX01_IOMUXC     IOMUXC_GPIO_AD_B0_03_FLEXPWM1_PWMX01

#define PWM1_PWMA02_GPIO       GPIO3
#define PWM1_PWMA02_GPIO_PIN   (16U)
#define PWM1_PWMA02_IOMUXC     IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02
#define PWM1_PWMB02_GPIO       GPIO3
#define PWM1_PWMB02_GPIO_PIN   (17U)
#define PWM1_PWMB02_IOMUXC     IOMUXC_GPIO_SD_B0_05_FLEXPWM1_PWMB02
#define PWM1_PWMX02_GPIO       GPIO1
#define PWM1_PWMX02_GPIO_PIN   (12U)
#define PWM1_PWMX02_IOMUXC     IOMUXC_GPIO_AD_B0_12_FLEXPWM1_PWMX02

#define PWM1_PWMA03_GPIO       GPIO2
#define PWM1_PWMA03_GPIO_PIN   (16U)
#define PWM1_PWMA03_IOMUXC     IOMUXC_GPIO_B1_00_FLEXPWM1_PWMA03
#define PWM1_PWMB03_GPIO       GPIO2
#define PWM1_PWMB03_GPIO_PIN   (17U)
#define PWM1_PWMB03_IOMUXC     IOMUXC_GPIO_B1_01_FLEXPWM1_PWMB03
#define PWM1_PWMX03_GPIO       GPIO1
#define PWM1_PWMX03_GPIO_PIN   (13U)
#define PWM1_PWMX03_IOMUXC     IOMUXC_GPIO_AD_B0_13_FLEXPWM1_PWMX03

#define PWM2_PWMA00_GPIO       GPIO2
#define PWM2_PWMA00_GPIO_PIN   (6U)
#define PWM2_PWMA00_IOMUXC     IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00
#define PWM2_PWMB00_GPIO       GPIO2
#define PWM2_PWMB00_GPIO_PIN   (7U)
#define PWM2_PWMB00_IOMUXC     IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00
#define PWM2_PWMX00_GPIO       
#define PWM2_PWMX00_GPIO_PIN   
#define PWM2_PWMX00_IOMUXC     

#define PWM2_PWMA01_GPIO       GPIO2
#define PWM2_PWMA01_GPIO_PIN   (8U)
#define PWM2_PWMA01_IOMUXC     IOMUXC_GPIO_B0_08_FLEXPWM2_PWMA01
#define PWM2_PWMB01_GPIO       GPIO2
#define PWM2_PWMB01_GPIO_PIN   (9U)
#define PWM2_PWMB01_IOMUXC     IOMUXC_GPIO_B0_09_FLEXPWM2_PWMB01
#define PWM2_PWMX01_GPIO       
#define PWM2_PWMX01_GPIO_PIN   
#define PWM2_PWMX01_IOMUXC     

#define PWM2_PWMA02_GPIO       GPIO2
#define PWM2_PWMA02_GPIO_PIN   (10U)
#define PWM2_PWMA02_IOMUXC     IOMUXC_GPIO_B0_10_FLEXPWM2_PWMA02
#define PWM2_PWMB02_GPIO       GPIO2
#define PWM2_PWMB02_GPIO_PIN   (11U)
#define PWM2_PWMB02_IOMUXC     IOMUXC_GPIO_B0_11_FLEXPWM2_PWMB02
#define PWM2_PWMX02_GPIO       
#define PWM2_PWMX02_GPIO_PIN   
#define PWM2_PWMX02_IOMUXC    

#define PWM2_PWMA03_GPIO       GPIO3
#define PWM2_PWMA03_GPIO_PIN   (2U)
#define PWM2_PWMA03_IOMUXC     IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03  
//#define PWM2_PWMA03_GPIO       GPIO2
//#define PWM2_PWMA03_GPIO_PIN   (18U)
//#define PWM2_PWMA03_IOMUXC     IOMUXC_GPIO_B1_02_FLEXPWM2_PWMA03
//#define PWM2_PWMA03_GPIO       GPIO4
//#define PWM2_PWMA03_GPIO_PIN   (19U)
//#define PWM2_PWMA03_IOMUXC     IOMUXC_GPIO_EMC_19_FLEXPWM2_PWMA03
#define PWM2_PWMB03_GPIO       GPIO3
#define PWM2_PWMB03_GPIO_PIN   (3U)
#define PWM2_PWMB03_IOMUXC     IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03 
//#define PWM2_PWMB03_GPIO       GPIO2
//#define PWM2_PWMB03_GPIO_PIN   (19U)
//#define PWM2_PWMB03_IOMUXC     IOMUXC_GPIO_B1_03_FLEXPWM2_PWMB03
//#define PWM2_PWMB03_GPIO       GPIO4
//#define PWM2_PWMB03_GPIO_PIN   (20U)
//#define PWM2_PWMB03_IOMUXC     IOMUXC_GPIO_EMC_20_FLEXPWM2_PWMB03
#define PWM2_PWMX03_GPIO       
#define PWM2_PWMX03_GPIO_PIN   
#define PWM2_PWMX03_IOMUXC

#define PWM_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)

typedef enum{
	__PWM_INDEPENDENT__,
	__PWM_COMPLEMENTARY__,
}PWM_PAIR_t;

class PWM{
friend class PWM_MOTOR;
public:
	PWM(void):min_duty(0),max_duty(100)
	{}
	
	void init( PWM_Type * PWMx                         ,\
           pwm_submodule_t submodule                   ,\
           pwm_channels_t  channel                     ,\
           uint32_t        freq_Hz                     ,\
           uint8_t         duty   = 0                  ,\
           PWM_PAIR_t      pair   = __PWM_INDEPENDENT__,\
           uint8_t    deadTimeVal = 0);

	void duty(uint8_t  value_0_100);
	void duty(uint32_t value,uint32_t val_max);
	void duty(uint32_t value,uint32_t val_min,uint32_t val_max);
	uint16_t getPulseCnt(void);
	void     setDutyRange(uint8_t min_duty,uint8_t max_duty);
	PWM_Type *   	   PWMx;
	uint32_t     	   freq_Hz;
	pwm_submodule_t    pwm_module;
    pwm_channels_t     pwm_channel;
    
protected:
	int16_t      	   modulo;
	uint16_t     	   pulseCnt;
private:
    pwm_config_t       pwm_cfg;
    pwm_signal_param_t pwm_param;
    
    pwm_mode_t         pwm_mode;
    PWM_PAIR_t         pwm_pair;

    uint8_t  max_duty;
	uint8_t  min_duty;
	uint16_t max_cnt;
	uint16_t min_cnt;

	void IOMUXC_MUX_PAD_RESOURCE_Config(void){
		gpio_pin_config_t  config;
		config.direction     = kGPIO_DigitalOutput;
		config.outputLogic   = 1;
		config.interruptMode = kGPIO_NoIntmode;

        switch((uint32_t)this->PWMx){
		case PWM1_BASE:
		    switch(this->pwm_module){
				case kPWM_Module_0:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
			    	    switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM1_PWMA00_IOMUXC, 0U);
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMA00_GPIO  , PWM1_PWMA00_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM1_PWMB00_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMB00_GPIO  , PWM1_PWMB00_GPIO_PIN, &config);break;
			    	    }
			    	}else{
			    		IOMUXC_SetPinMux   (PWM1_PWMA00_IOMUXC, 0U); 
			    		IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    		GPIO_PinInit       (PWM1_PWMA00_GPIO  , PWM1_PWMA00_GPIO_PIN, &config);
			    		IOMUXC_SetPinMux   (PWM1_PWMB00_IOMUXC, 0U); 
			    	    IOMUXC_SetPinConfig(PWM1_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM1_PWMB00_GPIO  , PWM1_PWMB00_GPIO_PIN, &config);
			    	}break;
				case kPWM_Module_1:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
			    	    switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM1_PWMA01_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMA01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMA01_GPIO  , PWM1_PWMA01_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM1_PWMB01_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMB01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMB01_GPIO  , PWM1_PWMB01_GPIO_PIN, &config);break;
			    	    }
			    	}else{
			    		IOMUXC_SetPinMux   (PWM1_PWMA01_IOMUXC, 0U); 
			    		IOMUXC_SetPinConfig(PWM1_PWMA01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    		GPIO_PinInit       (PWM1_PWMA01_GPIO  , PWM1_PWMA01_GPIO_PIN, &config);
			    		IOMUXC_SetPinMux   (PWM1_PWMB01_IOMUXC, 0U); 
			    	    IOMUXC_SetPinConfig(PWM1_PWMB01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM1_PWMB01_GPIO  , PWM1_PWMB01_GPIO_PIN, &config);
			    	}break;
				case kPWM_Module_2:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
			    	    switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM1_PWMA02_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMA02_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMA02_GPIO  , PWM1_PWMA02_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM1_PWMB02_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMB02_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMB02_GPIO  , PWM1_PWMB02_GPIO_PIN, &config);break;
			    	    }
			    	}else{
			    		IOMUXC_SetPinMux   (PWM1_PWMA02_IOMUXC, 0U);
			    		IOMUXC_SetPinConfig(PWM1_PWMA02_IOMUXC, PWM_PAD_CONFIG_DATA);
			    		GPIO_PinInit       (PWM1_PWMA02_GPIO  , PWM1_PWMA02_GPIO_PIN, &config);
			    		IOMUXC_SetPinMux   (PWM1_PWMB02_IOMUXC, 0U);
						IOMUXC_SetPinConfig(PWM1_PWMB02_IOMUXC, PWM_PAD_CONFIG_DATA);
						GPIO_PinInit       (PWM1_PWMB02_GPIO  , PWM1_PWMB02_GPIO_PIN, &config);
			    	}break;
				case kPWM_Module_3:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
			    	    switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM1_PWMA03_IOMUXC, 0U);
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMA03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMA03_GPIO  , PWM1_PWMA03_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM1_PWMB03_IOMUXC, 0U);
			    	    	               IOMUXC_SetPinConfig(PWM1_PWMB03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM1_PWMB03_GPIO  , PWM1_PWMB03_GPIO_PIN, &config);break;
			    	    }
			    	}else{
			    		IOMUXC_SetPinMux   (PWM1_PWMA03_IOMUXC, 0U);
			    	    IOMUXC_SetPinConfig(PWM1_PWMA03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM1_PWMA03_GPIO  , PWM1_PWMA03_GPIO_PIN, &config);
			    	    IOMUXC_SetPinMux   (PWM1_PWMB03_IOMUXC, 0U);
			    	    IOMUXC_SetPinConfig(PWM1_PWMB03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM1_PWMB03_GPIO  , PWM1_PWMB03_GPIO_PIN, &config);
			    	}break;
		    }break;
		case PWM2_BASE:
			switch(this->pwm_module){
				case kPWM_Module_0:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
						switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM2_PWMA00_IOMUXC, 0U);
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMA00_GPIO  , PWM2_PWMA00_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM2_PWMB00_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMB00_GPIO  , PWM2_PWMB00_GPIO_PIN, &config);break;
			    	    }
					}else{
						IOMUXC_SetPinMux   (PWM2_PWMA00_IOMUXC, 0U); 
			    		IOMUXC_SetPinConfig(PWM2_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    		GPIO_PinInit       (PWM2_PWMA00_GPIO  , PWM2_PWMA00_GPIO_PIN, &config);
			    		IOMUXC_SetPinMux   (PWM2_PWMB00_IOMUXC, 0U); 
			    	    IOMUXC_SetPinConfig(PWM2_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM2_PWMB00_GPIO  , PWM2_PWMB00_GPIO_PIN, &config);
					}break;
				case kPWM_Module_1:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
						switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM2_PWMA01_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMA01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMA01_GPIO  , PWM2_PWMA01_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM2_PWMB01_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMB01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMB01_GPIO  , PWM2_PWMB01_GPIO_PIN, &config);break;
			    	    }
					}else{
						IOMUXC_SetPinMux   (PWM2_PWMA01_IOMUXC, 0U); 
			    		IOMUXC_SetPinConfig(PWM2_PWMA01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    		GPIO_PinInit       (PWM2_PWMA01_GPIO  , PWM2_PWMA01_GPIO_PIN, &config);
			    		IOMUXC_SetPinMux   (PWM2_PWMB01_IOMUXC, 0U); 
			    	    IOMUXC_SetPinConfig(PWM2_PWMB01_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM2_PWMB01_GPIO  , PWM2_PWMB01_GPIO_PIN, &config);
					}break;
				case kPWM_Module_2:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
						switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM2_PWMA02_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMA02_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMA02_GPIO  , PWM2_PWMA02_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM2_PWMB02_IOMUXC, 0U); 
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMB02_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMB02_GPIO  , PWM2_PWMB02_GPIO_PIN, &config);break;
			    	    }
			    	}else{
			    		IOMUXC_SetPinMux   (PWM2_PWMA02_IOMUXC, 0U);
			    		IOMUXC_SetPinConfig(PWM2_PWMA02_IOMUXC, PWM_PAD_CONFIG_DATA);
			    		GPIO_PinInit       (PWM2_PWMA02_GPIO  , PWM2_PWMA02_GPIO_PIN, &config);
			    		IOMUXC_SetPinMux   (PWM2_PWMB02_IOMUXC, 0U);
						IOMUXC_SetPinConfig(PWM2_PWMB02_IOMUXC, PWM_PAD_CONFIG_DATA);
						GPIO_PinInit       (PWM2_PWMB02_GPIO  , PWM2_PWMB02_GPIO_PIN, &config);
			    	}break;
				case kPWM_Module_3:
					if(this->pwm_pair == __PWM_INDEPENDENT__){
						switch(this->pwm_channel){
			    	    	case kPWM_PwmA:IOMUXC_SetPinMux   (PWM2_PWMA03_IOMUXC, 0U);
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMA03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMA03_GPIO  , PWM2_PWMA03_GPIO_PIN, &config);break;
			    	    	case kPWM_PwmB:IOMUXC_SetPinMux   (PWM2_PWMB03_IOMUXC, 0U);
			    	    	               IOMUXC_SetPinConfig(PWM2_PWMB03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    	               GPIO_PinInit       (PWM2_PWMB03_GPIO  , PWM2_PWMB03_GPIO_PIN, &config);break;
			    	    }
			    	}else{
			    		IOMUXC_SetPinMux   (PWM2_PWMA03_IOMUXC, 0U);
			    	    IOMUXC_SetPinConfig(PWM2_PWMA03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM2_PWMA03_GPIO  , PWM2_PWMA03_GPIO_PIN, &config);
			    	    IOMUXC_SetPinMux   (PWM2_PWMB03_IOMUXC, 0U);
			    	    IOMUXC_SetPinConfig(PWM2_PWMB03_IOMUXC, PWM_PAD_CONFIG_DATA);
			    	    GPIO_PinInit       (PWM2_PWMB03_GPIO  , PWM2_PWMB03_GPIO_PIN, &config);
			    	}break;
			}break;
		case PWM3_BASE:break;
		case PWM4_BASE:break;
		default:break;
		}
	}
	void setHighPulseCnt(uint16_t value);
	void convertToCntRange();
	
};


#endif

