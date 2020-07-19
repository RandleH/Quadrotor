#ifndef _SYS_H
#define _SYS_H 

#include "jy901s.h"
#include "motor.h"
#include "nrf24l01.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "adc.h"

class System{
public: 
	System(void){
		/* 初始化内存保护单元 */
		BOARD_ConfigMPU();
		/* 初始化开发板引脚 */
		BOARD_InitPins();
		/* 初始化开发板时钟 */
		BOARD_BootClockRUN();
		/* 初始化调试串口 */
		BOARD_InitDebugConsole();

		BOARD_InitADC();
	}

	JY901  		sensor;
	PWM_MOTOR   motor;
	NRF24L01    radio;
private:

};

#endif