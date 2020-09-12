#include "servo.h"
#include "mathematic.h"

void SERVO::timeToCntRange(void){
	this->min_cnt = map_INTERVAL_UINT(this->levelHigh_min_us,0,__20000us__,0,this->pulseCnt);
    this->max_cnt = map_INTERVAL_UINT(this->levelHigh_max_us,0,__20000us__,0,this->pulseCnt);
}

void SERVO::valToCntRange(void){
	this->min_cnt = map_INTERVAL_UINT(this->min_value,0,__20000us__,0,this->pulseCnt);
    this->max_cnt = map_INTERVAL_UINT(this->max_value,0,__20000us__,0,this->pulseCnt);
}

status_t SERVO::updateServo(void){
	register int16_t modulo = this->modulo;
    //this->convertValue();
    this->PWMx->SM[this->horizon.pwm_module].VAL2 = (-modulo);//kPWM_PwmA
    this->PWMx->SM[this->horizon.pwm_module].VAL3 = (-modulo + this->value[0]);

    this->PWMx->SM[this->vertical.pwm_module].VAL4 = (-modulo);//kPWM_PwmB
    this->PWMx->SM[this->vertical.pwm_module].VAL5 = (-modulo + this->value[1]);

    this->PWMx->MCTRL |= PWM_MCTRL_LDOK( (1<<this->horizon.pwm_module)
                                        |(1<<this->vertical.pwm_module));
    return kStatus_Success;
}

void SERVO::setValueRange(uint32_t min,uint32_t max){
	this->max_value = max;
	this->min_value = min;
	this->valToCntRange();
}

void SERVO::setTimeRange(uint32_t min_us,uint32_t max_us){
	this->levelHigh_max_us = max_us;
	this->levelHigh_min_us = min_us;
	this->timeToCntRange();
}	



