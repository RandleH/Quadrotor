#include "delay.h"


bool DELAY::core_delay_init_flag = false;

void (*API_DELAY_ms_Handle)(uint32_t) = DELAY::ms;
void (*API_DELAY_us_Handle)(uint32_t) = DELAY::us;

