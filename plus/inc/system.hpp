#ifndef _SYS_H
#define _SYS_H 

#include "jy901s.h"
#include "motor.h"
#include "nrf24l01.h"
#include "camera.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "adc.h"
#include "CMD_Desk.h"
#include "led.h"
#include "os.hpp"
#include "pid.h"
#include "logger.h"
#include <stack>
using namespace std;

typedef enum{
	kCommandDeskSetting	,
	kCommandDeskOnline	,
	kCommandDeskOffline	,
	kSystemUninitialized,
	kSystemInitializing	,
	kSystemStandBy		,
	kSystemLowBettary	,
	kEmptyStatus  , 
}SystemStatus_t;

class System{

#ifdef CMD_DESK_SUPPORT
friend class CMD;
#endif 

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

		LED::RGB_Init();

#ifdef SYSTEM_UART_DEBUG_CONSOLE
		PRINTF("System created successfully.\n");
#endif
	}
	
	static JY901  			gyro;
	static PWM_MOTOR    	motor;
	static NRF24L01     	radio;
	static OV5640       	camera;
	static OperatingSystem 	OS;
	static PID_Module		PID;
	static DataLogger       Log;

	static void init(void);
	static void setStatus(SystemStatus_t new_status);
	static void setStatus(void);
	static SystemStatus_t getStatus(void);
private:
	static SystemStatus_t	     status;
	static stack<SystemStatus_t> state;
public:
	static bool   initGryoSensorFlag;
	static bool   initMotorFlag;
	static bool   initCameraFlag;
	static bool   initRadioFlag;
	static bool   initPressureSensorFlag;
	static bool   initWifiFlag;
#ifdef CMD_DESK_SUPPORT
	static bool   cmdDeskOnlineFlag;
#endif 
	static bool   enablePIDFlag;
	static bool   enableLEDFlag;
	static bool   enableCameraFlag;
	static bool   enableMotorFlag;
	
	static void updateGyroData(void* param);
	static void showLEDStatus(void* param);
	static void readRadioData(void* param);
	static void updateMotorSpeed(void* param);
	static void ADCtestMotor(void *param);
	static void pidControl(void* param);
};

#endif