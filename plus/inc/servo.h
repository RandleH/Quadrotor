#ifndef _SERVO_H
#define _SERVO_H 

#include "pwm.hpp"

#ifndef __20000us__
#define __20000us__		20000U
#endif

class SERVO{
public:
	SERVO(void){}

	void init(uint32_t levelHigh_min_us,uint32_t levelHigh_max_us){
		horizon.init (PWM1,kPWM_Module_1,kPWM_PwmA,__50Hz__,0);
		vertical.init(PWM1,kPWM_Module_1,kPWM_PwmB,__50Hz__,0);
		this->PWMx = PWM1;
		this->pulseCnt = this->horizon.getPulseCnt();
		this->modulo   = this->pulseCnt>>1;
		this->setTimeRange(levelHigh_min_us,levelHigh_max_us);
	}

	status_t updateServo(void);
	void     setTimeRange(uint32_t min_us,uint32_t max_us);
	void     setValueRange(uint32_t min,uint32_t max);
	uint32_t recordCnt(uint8_t which_servo);//Set center angle this time;

private:
   	PWM horizon;
   	PWM vertical;
   	PWM_Type* PWMx;
   	uint32_t levelHigh_max_us;
   	uint32_t levelHigh_min_us;
   	uint16_t max_cnt;
	uint16_t min_cnt;
	uint32_t max_value;
	uint32_t min_value;
	int16_t  modulo;
	uint16_t pulseCnt;
	void 	 timeToCntRange(void);
	void     valToCntRange(void);
public:
	uint32_t value[2];
};



#endif