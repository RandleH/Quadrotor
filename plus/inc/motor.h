
#ifndef _MOTOR_H
#define _MOTOR_H 
#include "pwm.hpp"
#include "utility.h"
#include "mathematic.h"

#define  M1 	0
#define  M2 	1
#define  M3 	2
#define  M4 	3

#define  USE_SPEED_VECTOR

#ifdef   USE_SPEED_VECTOR
#undef   USE_SPEED_ARRAY
#else
#define  USE_SPEED_ARRAY
#endif


#define OE_PIN_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) 


#define MOTOR_OE_GPIO					GPIO1
#define MOTOR_OE_PIN					(9U)//(4U)
#define MOTOR_OE_IOMUXC 				IOMUXC_GPIO_AD_B0_09_GPIO1_IO09//IOMUXC_GPIO_AD_B0_04_GPIO1_IO04

#define MAX_SAFETY_SPEED_OF_MOTOR              (2048U)
#define MIN_RUNING_SPEED_OF_MOTOR              (500U) //

#define MIN_SPEED_OF_MOTOR                     (0U)
#define MAX_SPEED_OF_MOTOR                     (4096U)

#ifdef SYSTEM_UART_DEBUG_CONSOLE
  #include "fsl_debug_console.h"
#endif


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
	PWM_MOTOR(void):max_speedLimit(800),max_accelerLimit(150),speed{0},delta_speed{0}{}
	void init(uint8_t min_duty,uint8_t max_duty,int32_t min_speed,int32_t max_speed);

	status_t outputEnable(bool value);
	status_t calibrate(uint8_t min_duty,uint8_t max_duty);
	status_t calibrate(void);
	status_t updateSpeed(void);
	status_t resetSpeed(uint16_t value);
	
	uint32_t speedLimit(void);
	void 	 speedLimit(uint32_t upper_bound);

	int32_t  accelerLimit(void);
	void 	 accelerLimit(int32_t upper_bound);

	void     kill(void);

	float  	 speed[4];
	int32_t  delta_speed[4];
	uint32_t cnt[4];
private:
	PWM M[4];
	uint8_t  max_duty;
	uint8_t  min_duty;
	uint16_t max_cnt;
	uint16_t min_cnt;
	uint32_t max_speed;	// It is different from "max_speedLimit"
	uint32_t min_speed;
	int16_t  modulo;
	uint16_t pulseCnt;
	uint32_t max_speedLimit;
	int32_t  max_accelerLimit;	
	float    internal_speed[4];
	float    internal_delta_speed[4];	
	inline   void setSpeedRange(uint32_t min,uint32_t max);
	void 	 convertToCntRange(void);
	inline 	 void convertSpeed(void);
	status_t updateSpeed(const char* opt);// This is a final speed-control function.
	status_t OE_init(void);
	PWM_Type* PWMx;
};





#endif


