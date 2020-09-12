#include "pwm.hpp"
#include "mathematic.h"

void PWM::init( PWM_Type * PWMx       ,\
           pwm_submodule_t submodule  ,\
           pwm_channels_t  channel    ,\
           uint32_t        freq_Hz    ,\
           uint8_t         duty       ,\
           PWM_PAIR_t      pair       ,\
           uint8_t    deadTimeVal     ){

    uint8_t  NumOfChannel    = 1;
    uint32_t srcClockFreq_Hz = CLOCK_GetFreq(kCLOCK_IpgClk);
    uint8_t  Div      = 0;

    this->PWMx        = PWMx;
    this->pwm_module  = submodule;
    this->pwm_channel = channel;
    this->pwm_mode    = kPWM_SignedEdgeAligned;
    this->pwm_pair    = pair;
    this->freq_Hz     = freq_Hz;
    this->IOMUXC_MUX_PAD_RESOURCE_Config();

    CLOCK_SetDiv(kCLOCK_IpgDiv,0x03);

    PWM_GetDefaultConfig(&this->pwm_cfg);
    this->pwm_cfg.clockSource     = kPWM_BusClock;//132MHz

    Div = 0;
    while(Div<8){
        if(
            this->freq_Hz < (uint32_t)(srcClockFreq_Hz>>Div)
          &&this->freq_Hz > (uint32_t)(srcClockFreq_Hz>>(Div+16)))
            break;
        Div++;
    }
    switch(Div){ 
        case 0:this->pwm_cfg.prescale = kPWM_Prescale_Divide_1;  break;
        case 1:this->pwm_cfg.prescale = kPWM_Prescale_Divide_2;  break;
        case 2:this->pwm_cfg.prescale = kPWM_Prescale_Divide_4;  break;
        case 3:this->pwm_cfg.prescale = kPWM_Prescale_Divide_8;  break;
        case 4:this->pwm_cfg.prescale = kPWM_Prescale_Divide_16; break;
        case 5:this->pwm_cfg.prescale = kPWM_Prescale_Divide_32; break;
        case 6:this->pwm_cfg.prescale = kPWM_Prescale_Divide_64; break;
        case 7:this->pwm_cfg.prescale = kPWM_Prescale_Divide_128;break;
    }
    this->pwm_cfg.reloadLogic     = kPWM_ReloadPwmFullCycle; 
    if(pair == __PWM_INDEPENDENT__)
        this->pwm_cfg.pairOperation   = kPWM_Independent; 
    else{
        NumOfChannel = 2;  
        if(this->pwm_channel == kPWM_PwmA)
            this->pwm_cfg.pairOperation = kPWM_ComplementaryPwmA;
        else
            this->pwm_cfg.pairOperation = kPWM_ComplementaryPwmB;
    }
    this->pwm_cfg.enableDebugMode = false;
    PWM_Init(this->PWMx,this->pwm_module, &this->pwm_cfg);
    this->PWMx->SM[this->pwm_module].DISMAP[0]=0;


    duty%=100;
    this->pwm_param.pwmChannel       = this->pwm_channel;
    this->pwm_param.level            = kPWM_HighTrue;
    this->pwm_param.dutyCyclePercent = duty;
    this->pwm_param.deadtimeValue    = 0U;
    PWM_SetupPwm(this->PWMx,this->pwm_module,&this->pwm_param,NumOfChannel,kPWM_SignedEdgeAligned,freq_Hz,CLOCK_GetFreq(kCLOCK_IpgClk));
    PWM_SetPwmLdok(this->PWMx,(int)(1U<<this->pwm_module) , true);
    PWM_StartTimer(this->PWMx,(int)(1U<<this->pwm_module));

    this->modulo   = this->PWMx->SM[this->pwm_module].VAL1;
    this->pulseCnt = this->modulo<<1;
    //DELAY
    this->duty(0);
    this->setDutyRange(0,100);
}

void PWM::duty(uint8_t value_0_100){
    uint16_t cnt = 0;
    if( value_0_100 < this->min_duty )
        value_0_100 = this->min_duty;
    if( value_0_100 > this->max_duty )
        value_0_100 = this->max_duty;

    cnt = map_ZERO_UINT(value_0_100,100,this->pulseCnt);
    if(this->pwm_mode == kPWM_SignedEdgeAligned){
        if(this->pwm_channel == kPWM_PwmA){
            this->PWMx->SM[this->pwm_module].VAL2 = (-this->modulo);//kPWM_PwmA00
            this->PWMx->SM[this->pwm_module].VAL3 = (-this->modulo + cnt);
        }else{
            this->PWMx->SM[this->pwm_module].VAL4 = (-this->modulo);//kPWM_PwmB00
            this->PWMx->SM[this->pwm_module].VAL5 = (-this->modulo + cnt);
        }
    }

    this->PWMx->MCTRL |= PWM_MCTRL_LDOK(1<<this->pwm_module);
}

void PWM::setDutyRange(uint8_t min_duty,uint8_t max_duty){
    this->min_duty = min_duty;
    this->max_duty = max_duty;
    this->convertToCntRange();
}

void PWM::setHighPulseCnt(uint16_t value){
	assert(value < this->pulseCnt);
	if (this->pwm_channel == kPWM_PwmA){
        this->PWMx->SM[this->pwm_module].VAL2 = (-this->modulo);
        this->PWMx->SM[this->pwm_module].VAL3 = (-this->modulo + value);
    }else{
        this->PWMx->SM[this->pwm_module].VAL4 = (-this->modulo);
        this->PWMx->SM[this->pwm_module].VAL5 = (-this->modulo + value);
    }
    PWMx->MCTRL |= PWM_MCTRL_LDOK((int)(1<<this->pwm_module));
}

uint16_t PWM::getPulseCnt(void){
    return this->pulseCnt;
}

void PWM::convertToCntRange(void){
    this->min_cnt = map_ZERO_UINT(this->min_duty,100,this->pulseCnt);
    this->max_cnt = map_ZERO_UINT(this->max_duty,100,this->pulseCnt);
}







