#ifndef _MATHEMATIC_H
#define _MATHEMATIC_H

#include <climits>
#include <cstdint>

#define CHAR2SHORT(p) (short)((short)(*(p+1)<<8)|(*p))  

#ifdef __cplusplus
extern "C"{
#endif

uint32_t map_ZERO_UINT  (register uint32_t value,register uint32_t i_max,register uint32_t o_max);
float    map_ZERO_FLOAT (register float    value,register float    i_max,register float    o_max);
#ifdef __cplusplus
}
#endif
//uint32_t map_INTERVAL_UINT(register uint32_t value,register uint32_t i_max,register uint32_t o_min,register uint32_t o_max);
//uint32_t map_INTERVAL_UINT(register uint32_t value,register uint32_t i_min,register uint32_t i_max,register uint32_t o_min,register uint32_t o_max);

#define map_INTERVAL_UINT(value,i_min,i_max,o_min,o_max) (uint32_t)(o_min+value*(o_max-o_min)/(i_max-i_min))
#endif

