#ifndef _SERVO_H
#define _SERVO_H 

#include "pwm.h"


class SERVO{
public:
	SERVO(uint32_t levelHigh_min_ms,uint32_t levelHigh_max_ms):
	horizon  (PWM1,kPWM_Module_1,kPWM_PwmA,__50Hz__,0),\
	vertical (PWM1,kPWM_Module_1,kPWM_PwmB,__50Hz__,0){
		
	}
private:
   	PWM horizon;
   	PWM vertical;
   	
};

#endif