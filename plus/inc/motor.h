
#ifndef _MOTOR_H
#define _MOTOR_H 
#include "pwm.hpp"
#include "mathematic.h"

#define Motor_1 	1
#define Motor_2 	2
#define Motor_3 	3
#define Motor_4 	4


#if defined USE_C_MODE_INIT
  #ifdef __cplusplus
  extern "C"{
  #endif
static void MOTOR_PWM_Init(void);
  #ifdef __cplusplus
  }
  #endif
#endif


class PWM_MOTOR{
public:
	PWM_MOTOR(void){}
	PWM_MOTOR(uint8_t min_duty,uint8_t max_duty,uint32_t min_value,uint32_t max_value):
					speed{0},cnt{0} {
		this->M[0].init(PWM1,kPWM_Module_0,kPWM_PwmA,__300Hz__,0);
		this->M[1].init(PWM1,kPWM_Module_0,kPWM_PwmB,__300Hz__,0);
		this->M[2].init(PWM1,kPWM_Module_1,kPWM_PwmA,__300Hz__,0);
		this->M[3].init(PWM1,kPWM_Module_1,kPWM_PwmB,__300Hz__,0);
		assert(min_duty < max_duty);
		assert(max_duty < 100);
		assert(this->M[0].PWMx == this->M[1].PWMx);
		assert(this->M[1].PWMx == this->M[2].PWMx);
		assert(this->M[2].PWMx == this->M[3].PWMx);

		
		this->PWMx = PWM1;
		this->pulseCnt  = this->M[0].getPulseCnt();
		this->modulo    = this->pulseCnt>>1;
		this->min_duty  = min_duty;
		this->max_duty  = max_duty;
		this->max_speed = max_value;
		this->min_speed = min_value; 
		this->convertToCntRange();
		//this->calibrate(this->min_duty,this->max_duty);
	}
	void init(uint8_t min_duty,uint8_t max_duty,uint32_t min_value,uint32_t max_value){
		this->M[0].init(PWM1,kPWM_Module_0,kPWM_PwmA,__300Hz__,0);
		this->M[1].init(PWM1,kPWM_Module_0,kPWM_PwmB,__300Hz__,0);
		this->M[2].init(PWM1,kPWM_Module_1,kPWM_PwmA,__300Hz__,0);
		this->M[3].init(PWM1,kPWM_Module_1,kPWM_PwmB,__300Hz__,0);
		assert(min_duty < max_duty);
		assert(max_duty < 100);
		assert(this->M[0].PWMx == this->M[1].PWMx);
		assert(this->M[1].PWMx == this->M[2].PWMx);
		assert(this->M[2].PWMx == this->M[3].PWMx);

		
		this->PWMx = PWM1;
		this->pulseCnt  = this->M[0].getPulseCnt();
		this->modulo    = this->pulseCnt>>1;
		this->min_duty  = min_duty;
		this->max_duty  = max_duty;
		this->max_speed = max_value;
		this->min_speed = min_value; 
		this->convertToCntRange();
	}
#if defined USE_C_MODE_INIT
	PWM_MOTOR(uint8_t min_duty,uint8_t max_duty,uint32_t min_value,uint32_t max_value,const char*){
		assert(min_duty < max_duty);
		assert(max_duty < 100);

		MOTOR_PWM_Init();
		this->min_duty = min_duty;
		this->max_duty = max_duty;
		this->pulseCnt  = this->M[0].getPulseCnt();
		this->min_duty  = min_duty;
		this->max_duty  = max_duty;
		this->max_speed = max_value;
		this->min_speed = min_value; 
		//this->calibrate(this->min_duty,this->max_duty);
	}
#endif
	status_t calibrate(uint8_t min_duty,uint8_t max_duty);
	status_t updateSpeed(uint8_t module,uint32_t value,uint32_t val_min,uint32_t val_max);
	status_t updateSpeed(void);
	void     setSpeedRange(uint32_t min,uint32_t max);
	uint32_t speed[4];
	uint32_t cnt[4];
private:
	PWM M[4];
	uint8_t  max_duty;
	uint8_t  min_duty;
	uint16_t max_cnt;
	uint16_t min_cnt;
	uint32_t max_speed;
	uint32_t min_speed;
	int16_t  modulo;
	uint16_t pulseCnt;
	void convertToCntRange(void);
	inline void convertSpeed(void);
	PWM_Type* PWMx;
};





#endif


