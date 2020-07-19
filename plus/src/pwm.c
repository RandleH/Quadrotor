#include "pwm.h"


#ifdef __cplusplus
extern "C" {
#endif



static void PWM_IO_Config(void){
	gpio_pin_config_t PWM_pin_config;
	PWM_pin_config.direction     = kGPIO_DigitalOutput;
 	PWM_pin_config.outputLogic   = 1;
 	PWM_pin_config.interruptMode = kGPIO_NoIntmode;
 	GPIO_PinInit(GPIO3,12U, &PWM_pin_config);
 	GPIO_PinInit(GPIO3,13U, &PWM_pin_config);
 	GPIO_PinInit(GPIO3,14U, &PWM_pin_config);
 	GPIO_PinInit(GPIO3,15U, &PWM_pin_config);
}

static void PWM_IOMUXC_Config(void){
	IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00, 0U); 
	IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00, 0U); 
	IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01, 0U); 
	IOMUXC_SetPinMux   (IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01, 0U); 
                                     
 	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00, PWM_PAD_CONFIG_DATA);
 	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00, PWM_PAD_CONFIG_DATA);
 	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01, PWM_PAD_CONFIG_DATA);
 	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01, PWM_PAD_CONFIG_DATA);
}

void MOTOR_PWM_Init(void){
	pwm_config_t       pwmConfig;       
	pwm_signal_param_t pwmSignal;
	uint32_t           pwmSourceClockInHz;

	PWM_IO_Config();
	PWM_IOMUXC_Config();
	CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3);

	//XBARA_Init(XBARA1);
	//XBARA_SetSignalsConnection(XBARA1,kXBARA1_InputFlexpwm1Pwm1OutTrig01,


	PWM_GetDefaultConfig(&pwmConfig);
	pwmConfig.reloadLogic      = kPWM_ReloadPwmFullCycle; 
    pwmConfig.pairOperation    = kPWM_Independent;     
    pwmConfig.enableDebugMode  = false;   
    pwmConfig.prescale         = kPWM_Prescale_Divide_8;     
    PWM_Init(PWM1, kPWM_Module_0, &pwmConfig);
    PWM_Init(PWM1, kPWM_Module_1, &pwmConfig);
    PWM1->SM[0].DISMAP[0]=0;
    PWM1->SM[1].DISMAP[0]=0;

    pwmSignal.level            = kPWM_HighTrue;
    pwmSignal.dutyCyclePercent = 30;   
    pwmSignal.deadtimeValue    = 0;
    pwmSourceClockInHz         = CLOCK_GetFreq(kCLOCK_IpgClk);

    pwmSignal.pwmChannel       = kPWM_PwmA;
    PWM_SetupPwm(PWM1, kPWM_Module_0,&pwmSignal,1, kPWM_SignedEdgeAligned, __300Hz__,
                 pwmSourceClockInHz);
    pwmSignal.pwmChannel       = kPWM_PwmB;
    PWM_SetupPwm(PWM1, kPWM_Module_0,&pwmSignal,1, kPWM_SignedEdgeAligned, __300Hz__,
                 pwmSourceClockInHz);
    PWM_SetPwmLdok(PWM1,kPWM_Control_Module_0,true);
    PWM_StartTimer(PWM1,kPWM_Control_Module_0);

    pwmSignal.pwmChannel       = kPWM_PwmA;
    PWM_SetupPwm(PWM1, kPWM_Module_1,&pwmSignal,1, kPWM_SignedEdgeAligned, __300Hz__,
                 pwmSourceClockInHz);
    pwmSignal.pwmChannel       = kPWM_PwmB;
    PWM_SetupPwm(PWM1, kPWM_Module_1,&pwmSignal,1, kPWM_SignedEdgeAligned, __300Hz__,
                 pwmSourceClockInHz);
    PWM_SetPwmLdok(PWM1,kPWM_Control_Module_1,true);
    PWM_StartTimer(PWM1,kPWM_Control_Module_1);
}

void updatePWM_Duty(PWM_Type* PWMx,pwm_channels_t pwm_channel,pwm_submodule_t pwm_module,uint16_t cnt){
	int16_t modulo = 27500;
	if (pwm_channel == kPWM_PwmA){
        PWMx->SM[pwm_module].VAL2 = (-modulo);
        PWMx->SM[pwm_module].VAL3 = (-modulo + cnt);
    }else{
        PWMx->SM[pwm_module].VAL4 = (-modulo);
        PWMx->SM[pwm_module].VAL5 = (-modulo + cnt);
    }
    PWMx->MCTRL |= PWM_MCTRL_LDOK((int)(1<<pwm_module));
}


#ifdef __cplusplus
}
#endif



