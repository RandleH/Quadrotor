
#include "nvic.h"
#include "core_cm7.h"

#if defined(__cplusplus)
extern "C" {
#endif

void set_IRQn_Priority(IRQn_Type IRQn,PreemptPriority_Type PreemptPriority, SubPriority_Type SubPriorit){
  uint32_t PriorityGroup = 0;   
  uint32_t Priority_Encode = 0; 
  PriorityGroup = NVIC_GetPriorityGrouping();
  Priority_Encode = NVIC_EncodePriority (PriorityGroup,(uint32_t)PreemptPriority,(uint32_t)SubPriorit);
  
  NVIC_SetPriority(IRQn, Priority_Encode);
}

void Set_NVIC_PriorityGroup(PriorityGroup_Type PriorityGroup){
  NVIC_SetPriorityGrouping((uint32_t)PriorityGroup); 
}
	

#if defined(__cplusplus)
}
#endif

