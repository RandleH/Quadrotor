#include "system.hpp"
#include "CMD_Desk.h"
#include "delay.h"

// MODULEs =====================================================================//
JY901 						System::gyro;
PWM_MOTOR   				System::motor;
NRF24L01    				System::radio;
OV5640      				System::camera;
OperatingSystem 			System::OS;
SystemStatus_t				System::status;
stack<SystemStatus_t>		System::state;
PID_Module					System::PID;
extern CMD 					COMMAND;
//==============================================================================//

// FLAGs   =====================================================================//
bool   						System::initGryoSensorFlag     = true;
bool   						System::initMotorFlag          = true;
bool   						System::initCameraFlag         = true;
bool   						System::initRadioFlag          = true;
bool   						System::initPressureSensorFlag = true;
bool   						System::initWifiFlag           = true;
bool                        System::enablePIDFlag          = true;
bool                        System::enableLEDFlag          = true;
bool                        System::enableCameraFlag       = false;
bool                        System::enableMotorFlag[4]     = {true};
//==============================================================================//



void System::init(void){
	
	LED::RGB_Set(LED_YELLOW);
	setStatus(kSystemUninitialized);
	if( initGryoSensorFlag == true)	{	gyro.init(LPI2C1);			initGryoSensorFlag = false;}
	if( initMotorFlag      == true)	{	motor.init(30,70,0,4096);	initMotorFlag 	 = false;}
	if( initCameraFlag     == true)	{	camera.init(LPI2C1);		initCameraFlag 	 = false;}
	if( initRadioFlag  	== true)	{	radio.init(	LPSPI4,PCS0,\
																GPIO2,24U,\
																GPIO2,25U,\
																GPIO2,20U);		initRadioFlag 	 = false;
		//"RADIO_IRQn should also be changed at nrf24l01.h"
	}
	if(initPressureSensorFlag == true)	{initPressureSensorFlag = false;}
	if(initWifiFlag		== true)		{initWifiFlag	= false;}

	DELAY::ms(100);
	if(radio.check() != kStatus_Success){
#ifdef SYSTEM_UART_DEBUG_CONSOLE
		PRINTF("No Radio Hardware Found!\n");
#endif
		while(1);
	}
	radio.rxMode();
	DELAY::ms(10);

	setStatus(kSystemStandBy);
}

void System::setStatus(SystemStatus_t new_status){
	if(new_status != getStatus()){
		state.push(new_status);
		status = state.top();
	}
}

void System::setStatus(void){
	state.pop();
	status = state.top();
}

SystemStatus_t System::getStatus(void){
	if(state.empty()==true)
		return kEmptyStatus;
	
	return state.top();
}


/* TASK LIST =====================================================================
[ Task 1 ]
[ Task 2 ]
[ Task 3 ]
[ Task 4 ]
[ Task 4 ]
================================================================================*/
#include "task.h"
#include "led.h"
void System::updateGyroData(void* param){
	while(1){
		System::gyro.updateData();
		vTaskDelay(10);
	}
}

void System::showLEDStatus(void* param){
	while(1){
		if(enableLEDFlag == true){
			switch(System::status){
		case kCommandDeskSetting:		LED::RGB_Set(LED_WHITE);break;

		case kCommandDeskOnline:		LED::RGB_Set(LED_GREEN);
										vTaskDelay(50);
										LED::RGB_Set(LED_OFF);
										vTaskDelay(1000);break;

		case kCommandDeskOffline:		LED::RGB_Set(LED_RED);break;

		case kSystemUninitialized:		LED::RGB_Set(LED_BLUE);
										vTaskDelay(500);
										LED::RGB_Set(LED_OFF);
										vTaskDelay(500);break;
		case kSystemInitializing:		LED::RGB_Set(LED_YELLOW);break;

		case kSystemStandBy:			LED::RGB_Set(LED_GREEN);
										break;

		case kSystemLowBettary:			LED::RGB_Set(LED_RED);
										vTaskDelay(500);
										LED::RGB_Set(LED_OFF);
										vTaskDelay(500);break;
			}
		}else{
			LED::RGB_Set(LED_OFF);
		}

		vTaskDelay(100);
	}

}

void System::readRadioData(void* param){
	while(1){
		if(System::radio.recvFlag==true){
			taskENTER_CRITICAL(); 
			DisableIRQ(RADIO_IRQn);

            System::radio.recvFlag = false;

            if(COMMAND.analize(System::radio.rx_buffer,32) != kStatus_Success)
                System::setStatus(kCommandDeskOffline);
           	
            System::radio.clearIRQ();
            set_IRQn_Priority(RADIO_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
            EnableIRQ(RADIO_IRQn);
            taskEXIT_CRITICAL(); 
        }
        vTaskDelay(10);
	}
}


void System::updateMotorSpeed(void* param){
	while(1){
		System::motor.updateSpeed();
		vTaskDelay(100);
	}
}

extern volatile bool 	 ADC_ConversionDoneFlag;
extern volatile uint32_t ADC_ConvertedValue;
void System::ADCtestMotor(void *param){
	while(1){
		if(ADC_ConversionDoneFlag == true){
			ADC_ConversionDoneFlag = false;
			motor.speed[0] = motor.speed[1] = motor.speed[2] = motor.speed[3] = ADC_ConvertedValue;
			motor.updateSpeed();
		}
		vTaskDelay(100);
	}
}

void System::pidControl(void* param){
	while(1){
		if(enablePIDFlag == true){
			if( System::PID.resetSpeedFlag == false ){
		
#ifdef USE_INCREMENTAL_PID
				System::PID.clearIncData();
				System::PID.pitch.apply(gyro.angle.pitch , PID_Module::speedInc);
				System::PID.roll.apply (gyro.angle.roll  , PID_Module::speedInc);
				System::PID.yaw.apply();
#endif
#ifdef USE_POSITIONAL_PID
				System::PID.apply(gyro.angle);
#endif
				
				if(	   System::motor.speed[M1] > 500
					|| System::motor.speed[M2] > 500
					|| System::motor.speed[M3] > 500
					|| System::motor.speed[M4] > 500){

#ifdef USE_INCREMENTAL_PID
					System::motor.speed[M1] += PID.speedInc[M1];
					System::motor.speed[M2] += PID.speedInc[M2];
					System::motor.speed[M3] += PID.speedInc[M3];
					System::motor.speed[M4] += PID.speedInc[M4];
#endif

#ifdef USE_POSITIONAL_PID
					System::motor.speed[M1] = PID.averageSpeed + PID.speedPos[M1];
					System::motor.speed[M2] = PID.averageSpeed + PID.speedPos[M2];
					System::motor.speed[M3] = PID.averageSpeed + PID.speedPos[M3];
					System::motor.speed[M4] = PID.averageSpeed + PID.speedPos[M4];
#endif			
				}


			}else{
				System::PID.clearPosData();
				System::PID.resetSpeed();
				System::motor.speed[M1] = System::PID.speedPos[M1];
				System::motor.speed[M2] = System::PID.speedPos[M2];
				System::motor.speed[M3] = System::PID.speedPos[M3];
				System::motor.speed[M4] = System::PID.speedPos[M4];

				System::PID.resetSpeedFlag = false;
			}
		}

		vTaskDelay(10);
	}
}






#ifdef __cplusplus
extern "C"{
#endif


#ifdef __cplusplus
}
#endif
