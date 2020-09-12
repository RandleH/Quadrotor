#include "motor.h"
#include "delay.h"

void PWM_MOTOR::init(uint8_t min_duty,uint8_t max_duty,int32_t min_speed,int32_t max_speed){
#ifdef SYSTEM_UART_DEBUG_CONSOLE
    PRINTF("Initializing Motor...\n");
#endif
    this->M[M1].init(PWM1,kPWM_Module_0,kPWM_PwmA,__300Hz__,0);
    this->M[M2].init(PWM1,kPWM_Module_0,kPWM_PwmB,__300Hz__,0);
    this->M[M3].init(PWM1,kPWM_Module_1,kPWM_PwmA,__300Hz__,0);
    this->M[M4].init(PWM1,kPWM_Module_1,kPWM_PwmB,__300Hz__,0);
    assert(min_duty < max_duty);
    assert(max_duty < 100);
    assert(this->M[0].PWMx == this->M[1].PWMx);
    assert(this->M[1].PWMx == this->M[2].PWMx);
    assert(this->M[2].PWMx == this->M[3].PWMx);

    this->OE_init();
    this->PWMx = PWM1;
    this->pulseCnt  = this->M[0].getPulseCnt();
    this->modulo    = this->pulseCnt>>1;
    this->min_duty  = min_duty;
    this->max_duty  = max_duty;
    this->max_speed = max_speed;
    this->min_speed = min_speed; 
    this->convertToCntRange();

    this->outputEnable(true);
#ifdef SYSTEM_UART_DEBUG_CONSOLE
    PRINTF("Maxium Duty = %d%%\n",this->max_duty);
    PRINTF("Minium Duty = %d%%\n",this->min_duty);
    PRINTF("Input Value Range:[ %d : %d ]\n",this->min_speed,this->max_speed);
    PRINTF("Pulse Count = %d\n",0,this->pulseCnt);
    PRINTF("Count Range :[ %d : %d ]\n",this->min_cnt,this->max_cnt);
    PRINTF("Calibrating...\n");
#endif

    this->calibrate();
}

status_t PWM_MOTOR::calibrate(uint8_t min_duty,uint8_t max_duty){
    DELAY::ms(2000);
    
    this->speed[0]=this->speed[1]=this->speed[2]=this->speed[3]=this->max_speed;
    this->updateSpeed("Calibration Process");
#ifdef SYSTEM_UART_DEBUG_CONSOLE
    PRINTF("Input PWM at Maxium Duty...\n");
#endif
    DELAY::ms(2000);
    DELAY::ms(2000);

    this->speed[0]=this->speed[1]=this->speed[2]=this->speed[3]=this->min_speed;
    this->updateSpeed("Calibration Process");
#ifdef SYSTEM_UART_DEBUG_CONSOLE
    PRINTF("Input PWM at Minium Duty...\n");
#endif
    DELAY::ms(2000);
    DELAY::ms(2000);
#ifdef SYSTEM_UART_DEBUG_CONSOLE
    PRINTF("Calibration completed\n");
#endif
    return kStatus_Success;
}

status_t PWM_MOTOR::outputEnable(bool value){
    if (value == true)
        MOTOR_OE_GPIO->DR &= ~(1U << MOTOR_OE_PIN); /* Set pin output to low level.*/
    else
        MOTOR_OE_GPIO->DR |= (1U << MOTOR_OE_PIN); /* Set pin output to high level.*/
    return kStatus_Success; 
}


status_t PWM_MOTOR::calibrate(void){
    return this->calibrate(this->min_duty,this->max_duty);
}

#include <cstring>
status_t PWM_MOTOR::updateSpeed(void){
    memcpy(internal_speed,speed,4*sizeof(float));

    LIMIT(internal_speed[M1],    0,(float)max_speedLimit);
    LIMIT(internal_speed[M2],    0,(float)max_speedLimit);
    LIMIT(internal_speed[M3],    0,(float)max_speedLimit);
    LIMIT(internal_speed[M4],    0,(float)max_speedLimit);

    updateSpeed("Checked");
    return kStatus_Success;
}

status_t PWM_MOTOR::updateSpeed(const char* opt){
    register int16_t modulo = this->modulo;
    
    this->convertSpeed();
    this->PWMx->SM[this->M[0].pwm_module].VAL2 = (-modulo);//kPWM_PwmA00
    this->PWMx->SM[this->M[0].pwm_module].VAL3 = (-modulo + this->cnt[0]);

    this->PWMx->SM[this->M[1].pwm_module].VAL4 = (-modulo);//kPWM_PwmB00
    this->PWMx->SM[this->M[1].pwm_module].VAL5 = (-modulo + this->cnt[1]);

    this->PWMx->SM[this->M[2].pwm_module].VAL2 = (-modulo);//kPWM_PwmA01
    this->PWMx->SM[this->M[2].pwm_module].VAL3 = (-modulo + this->cnt[2]);

    this->PWMx->SM[this->M[3].pwm_module].VAL4 = (-modulo);//kPWM_PwmB01
    this->PWMx->SM[this->M[3].pwm_module].VAL5 = (-modulo + this->cnt[3]);

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
        this->cnt[i] = map_INTERVAL_UINT(   (int32_t)this->internal_speed[i],\
                                            (int32_t)this->min_speed,\
                                            (int32_t)this->max_speed,\
                                            this->min_cnt,\
                                            this->max_cnt);
    }
}

status_t PWM_MOTOR::OE_init(void){
    IOMUXC_SetPinMux   (MOTOR_OE_IOMUXC, 0U); 
    IOMUXC_SetPinConfig(MOTOR_OE_IOMUXC, PWM_PAD_CONFIG_DATA);

    gpio_pin_config_t io_config;
    io_config.direction     = kGPIO_DigitalOutput;
    io_config.interruptMode = kGPIO_NoIntmode;
    io_config.outputLogic   = 1;
    GPIO_PinInit(MOTOR_OE_GPIO, MOTOR_OE_PIN, &io_config);
    return kStatus_Success;
}

uint32_t PWM_MOTOR::speedLimit(void){
    return this->max_speedLimit;
}

void PWM_MOTOR::speedLimit(uint32_t value){
    LIMIT(value,0,this->max_speed); 
    this->max_speedLimit = value;
}


int32_t PWM_MOTOR::accelerLimit(void){
    return this->max_accelerLimit;
}

void PWM_MOTOR::accelerLimit(int32_t delta){
    LIMIT(delta,0,this->max_speed);
    this->max_accelerLimit = delta;
}

// This is a block function. Once enter, dead loop.
void PWM_MOTOR::kill(void){
    uint16_t cnt = (uint16_t)speed[0];
    cnt/=100;
    cnt*=100;
    while(cnt-=100){
        speed[0] = \
        speed[1] = \
        speed[2] = \
        speed[3] = (float)cnt;
        updateSpeed(" Skip the Limit ");
        DELAY::ms(100);
    }
    outputEnable(false);
    while(1);
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
