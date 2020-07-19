#include "motor.h"
#include "delay.h"


status_t PWM_MOTOR::calibrate(uint8_t min_duty,uint8_t max_duty){
    DELAY::ms(2000);
    //LED::RGB_Set(0x04);//R
    this->M[1-1].duty(max_duty);
    this->M[2-1].duty(max_duty);
    this->M[3-1].duty(max_duty);
    this->M[4-1].duty(max_duty);

    DELAY::ms(2000);
    DELAY::ms(2000);

    this->M[1-1].duty(min_duty);
    this->M[2-1].duty(min_duty);
    this->M[3-1].duty(min_duty);
    this->M[4-1].duty(min_duty);
    DELAY::ms(2000);
    DELAY::ms(2000);

    return kStatus_Success;
}

status_t PWM_MOTOR::updateSpeed(uint8_t module_1_to_4,uint32_t value,uint32_t val_min,uint32_t val_max){
	//assert(0 < module_1_to_4 && module_1_to_4 < 5);
    //assert(val_min < val_max);
    //assert(val_min < value && value < val_max);

	this->M[module_1_to_4-1].duty(value,val_min,val_max);
	return kStatus_Success;
}

status_t PWM_MOTOR::updateSpeed(void){
    register int16_t modulo = this->modulo;
    this->convertSpeed();
    this->PWMx->SM[this->M[0].pwm_module].VAL2 = (-modulo);//kPWM_PwmA
    this->PWMx->SM[this->M[0].pwm_module].VAL3 = (-modulo + this->speed[0]);

    this->PWMx->SM[this->M[1].pwm_module].VAL4 = (-modulo);//kPWM_PwmB
    this->PWMx->SM[this->M[1].pwm_module].VAL5 = (-modulo + this->speed[1]);

    this->PWMx->SM[this->M[2].pwm_module].VAL2 = (-modulo);//kPWM_PwmA
    this->PWMx->SM[this->M[2].pwm_module].VAL3 = (-modulo + this->speed[2]);

    this->PWMx->SM[this->M[3].pwm_module].VAL4 = (-modulo);//kPWM_PwmB
    this->PWMx->SM[this->M[3].pwm_module].VAL5 = (-modulo + this->speed[3]);

    this->PWMx->MCTRL |= PWM_MCTRL_LDOK( (1<<this->M[0].pwm_module)
                                        |(1<<this->M[1].pwm_module)
                                        |(1<<this->M[2].pwm_module)
                                        |(1<<this->M[3].pwm_module));
    return kStatus_Success;

}

void PWM_MOTOR::setSpeedRange(uint32_t min,uint32_t max){
    this->max_speed = max;
    this->min_speed = min;
}


void PWM_MOTOR::convertToCntRange(void){
    this->min_cnt = map_ZERO_UINT(this->min_duty,100,this->pulseCnt);
    this->max_cnt = map_ZERO_UINT(this->max_duty,100,this->pulseCnt);
}

void PWM_MOTOR::convertSpeed(void){
    for(uint8_t i=0;i<4;i++){
        this->speed[i] = map_INTERVAL_UINT(this->speed[i],this->min_speed,this->max_speed,this->min_cnt,this->max_cnt);
    }
}


#if defined USE_C_MODE_INIT

#ifdef __cplusplus
extern "C"{
#endif
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_iomuxc.h"
#include "fsl_clock.h"
#include "pad_config.h"

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

static void MOTOR_PWM_Init(void){
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

#ifdef __cplusplus
}
#endif

#endif
