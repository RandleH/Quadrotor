#include "pwm.hpp"
#include "mathematic.h"


void PWM::duty(uint8_t value_0_100){
    if( value_0_100 < this->min_duty )
        value_0_100 = this->min_duty;
    if( value_0_100 > this->max_duty )
        value_0_100 = this->max_duty;
	PWM_UpdatePwmDutycycle(this->PWMx,this->pwm_module,this->pwm_channel,this->pwm_mode,value_0_100);
	PWM_SetPwmLdok(this->PWMx,(uint8_t)(1U<<this->pwm_module), true);
}

void PWM::duty(uint32_t value,uint32_t val_max){
    this->setHighPulseCnt(map_INTERVAL_UINT(value,   0   ,val_max,this->min_cnt,this->max_cnt));
}

void PWM::duty(uint32_t value,uint32_t val_min,uint32_t val_max){
    this->setHighPulseCnt(map_INTERVAL_UINT(value,val_min,val_max,this->min_cnt,this->max_cnt));
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







