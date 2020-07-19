#include "mathematic.h"


#ifdef __cplusplus
extern "C"{
#endif

uint32_t map_ZERO_UINT(register uint32_t value,register uint32_t i_max,register uint32_t o_max){
	return (uint32_t)(value*o_max/(i_max));
}

float map_ZERO_FLOAT(register float value,register float i_max,register float o_max){
	return (float)(value*o_max/(i_max));
}

#ifdef __cplusplus
}
#endif
#if 0
uint32_t map_INTERVAL_UINT(register uint32_t value,register uint32_t i_max,register uint32_t o_min,register uint32_t o_max){
	return (uint32_t)(o_min+value*(o_max-o_min)/(i_max));
}

uint32_t map_INTERVAL_UINT(register uint32_t value,register uint32_t i_min,register uint32_t i_max,register uint32_t o_min,register uint32_t o_max){
	return (uint32_t)(o_min+value*(o_max-o_min)/(i_max-i_min));
}
#endif