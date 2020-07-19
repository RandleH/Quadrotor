#ifndef _DELAY_H
#define _DELAY_H


#include "fsl_clock.h"
#include "fsl_common.h"
#include "core_cm7.h"

#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC

#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


extern uint32_t SystemCoreClock;



class DELAY{
public:
	static void core_delay_init(void){
		DEM_CR |= (uint32_t)DEM_CR_TRCENA;
		DWT_CYCCNT = (uint32_t)0u;
		DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
	}
	static void core_ms(uint32_t xx_ms){
		DELAY::core_us(xx_ms*1000);
	}
	static void core_us(uint32_t xx_us){
		DELAY::core_delay_init();
		uint32_t ticks;
 		uint32_t t_old,t_now,tcnt=0;
  
		ticks = xx_us * ((uint32_t)CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000);    
		tcnt  = 0;
		t_old  = DELAY::core_read_timer();
		while(1){
			t_now = DELAY::core_read_timer();
			if(t_now != t_old){
				if(t_now > t_old)
					tcnt += t_now - t_old;
				else
					tcnt += UINT32_MAX - t_old + t_now; 

				if(tcnt >= ticks)
					break;
			}
		}
	}
	static void ms(uint32_t xx_ms){
		SysTick_Config(SystemCoreClock/1000);
		DELAY::wait(xx_ms);
	}
	static void us(uint32_t xx_us){
		SysTick_Config(SystemCoreClock/1000000);
		DELAY::wait(xx_us);
	}
private:
	static void wait(uint32_t value){
		for(uint32_t i=0;i<value;i++){
			while( !((SysTick->CTRL)&(1<<16)) );
		}
		SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	}
	static uint32_t core_read_timer(void){return (uint32_t)DWT_CYCCNT;}
	static bool core_delay_init_flag;
};




#endif			