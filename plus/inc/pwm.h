#ifndef _PWM_H
#define _PWM_H 
#ifndef __50Hz__
#define __50Hz__   50
#endif
#ifndef __100Hz__
#define __100Hz__  100
#endif
#ifndef __300Hz__
#define __300Hz__  300
#endif
#ifndef __500Hz__
#define __500Hz__  500
#endif
#ifndef __1KHz__
#define __1KHz__   1000
#endif
#ifndef __5KHz__
#define __5KHz__   5000
#endif
#ifndef __10KHz__
#define __10KHz__  10000
#endif
#ifndef __50KHz__
#define __50KHz__  50000
#endif
#ifndef __100KHz__
#define __100KHz__ 100000
#endif
#ifndef __500KHz__
#define __500KHz__ 500000
#endif
#ifndef __1MHz__
#define __1MHz__   1000000
#endif

#define PWM_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) 

#ifdef __cplusplus
extern "C"{
#endif
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_iomuxc.h"
#include "fsl_clock.h"
#include "pad_config.h"

void MOTOR_PWM_Init(void);
void updatePWM_Duty(PWM_Type* PWMx,pwm_channels_t pwm_channel,pwm_submodule_t pwm_module,uint16_t cnt);
#ifdef __cplusplus
}
#endif


#endif