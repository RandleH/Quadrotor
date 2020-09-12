
#include "CMD_Desk.h"
#include "system.hpp"
#include "delay.h"
#include "task.h"
#include "utility.h"
#include <math.h>

extern System QUADROTOR;


status_t CMD::analize(uint8_t* recvMessage,size_t num){
	assert(num <= 32);
	status_t result = kStatus_Fail;
	//size_t   cnt = 0;
	switch(recvMessage[PACKAGE_FUNC_INDEX]){
case QUADROTOR_CMD_CONNECT:
		System::setStatus(kCommandDeskOnline);
		this->do_Connect(recvMessage);
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_DISCONNECT:
		System::setStatus(kCommandDeskOffline);
		this->do_Disconnect(recvMessage);
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_TESTMOTOR:
		System::setStatus(kCommandDeskSetting);
		this->do_MotorControl(recvMessage);
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_PIDQUERY:
		this->do_PIDSendParam(recvMessage);
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_PIDADJUSTER:
		System::setStatus(kCommandDeskSetting);
		this->do_PIDSetParam(recvMessage);
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_PIDTHROTTLE:
		System::setStatus(kCommandDeskSetting);
		this->do_PIDSetThrottle(recvMessage);
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_EXIT_TESTMOTOR:
		this->do_ExitSpeedCTRL(recvMessage);
		System::setStatus();
		result = kStatus_Success;
		break;
case QUADROTOR_CMD_EXIT_PIDADJUSTER:
case QUADROTOR_CMD_EXIT_PIDTHROTTLE:
		System::setStatus();
		result = kStatus_Success;
		break;

case QUADROTOR_CMD_KILLMOTOR:
		this->do_KillMotor(recvMessage);
		result = kStatus_Success;
		break;

default:result = kStatus_InvalidArgument;
		break;
	} 


	if(this->responseFlag == true){
		System::radio.txMode();
		vTaskDelay(20);//At least 1.5ms
		while(System::radio.send(this->txBuffer,this->bufferSize) != kStatus_Success){
			//cnt++;
			//if(cnt > 2000){
			//	result = kStatus_Timeout;
			//	QUADROTOR.cmdDeskOnlineFlag = false;
			//	break;
			//}
		}
		System::radio.rxMode();
		this->responseFlag = false;
	}
	return result;
}

void CMD::do_Connect(uint8_t* buffer){
	memset(this->txBuffer,'\0',this->bufferSize);
	this->txBuffer[0]  = QUADROTOR_PACKAGE_HEAD;
	this->txBuffer[1]  = QUADROTOR_CMD_CONNECT;
	this->txBuffer[2]  = 'R';
	this->txBuffer[3]  = 'O';
	this->txBuffer[4]  = 'T';
	this->txBuffer[5]  = 'O';
	this->txBuffer[6]  = 'R';
	this->txBuffer[7]  = 'D';
	this->txBuffer[8]  = 'A';
	this->txBuffer[9]  = 'U';
	this->txBuffer[10] = 'Q';
	
	this->do_AddACK(this->txBuffer);
	this->responseFlag = true;
	QUADROTOR.radio.direction = kSlaveMode;
}

void CMD::do_Disconnect(uint8_t*){
	memset(this->txBuffer,'\0',this->bufferSize);
	this->responseFlag = false;
	QUADROTOR.radio.direction = kSlaveMode;
}

void CMD::do_MotorControl(uint8_t* buffer){
	this->responseFlag = false;
	ASSERT_SHAKEHAND(buffer);

	uint16_t value = (uint16_t)((buffer[4]<<8)|buffer[3]);

	switch(buffer[2]){
		case 0:// Motor ALL
			System::motor.speed[0] = \
			System::motor.speed[1] = \
			System::motor.speed[2] = \
			System::motor.speed[3] = value;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			System::motor.speed[buffer[2]-1] = value;
			break;
		default:
			break;
	}
	System::enablePIDFlag = false;
}

void CMD::do_PIDModuleSwitch(uint8_t* buffer){
	
}

void CMD::do_PIDMotorSwitch(uint8_t* buffer){
	
}

void CMD::do_PIDSendParam(uint8_t* buffer){
	float kp=0.0,ti=0.0,td=0.0;
	xxPID_Limit_t limit;
	switch(buffer[2]){
		case 0://Thrust PID Module
			break;
		case 1://Pitch PID Module
			kp    = System::PID.pitch.Kp();
			ti    = System::PID.pitch.Ti();
			td    = System::PID.pitch.Td();
			limit = System::PID.pitch.Limitation();					
			break;
		case 2://Roll PID Module
		 	kp    = System::PID.roll.Kp();
		 	ti    = System::PID.roll.Ti();
		 	td    = System::PID.roll.Td();
		 	limit = System::PID.roll.Limitation();
			break;
		case 3://Yaw PID Module
			kp    = System::PID.yaw.Kp();
		 	ti    = System::PID.yaw.Ti();
		 	td    = System::PID.yaw.Td();
		 	limit = System::PID.yaw.Limitation();
			break;
		default:
			break;
	}

    struct Note xxxx_10E;

    xxxx_10E = NOTATION( kp, 4 );
    txBuffer[3] = (uint8_t)xxxx_10E.E;
    txBuffer[4] = (uint8_t)xxxx_10E.value;
	txBuffer[5] = ((uint16_t)(xxxx_10E.value)) >> 8;

	xxxx_10E = NOTATION( ti, 4 );
    txBuffer[6] = (uint8_t)xxxx_10E.E;
    txBuffer[7] = (uint8_t)xxxx_10E.value;
	txBuffer[8] = ((uint16_t)(xxxx_10E.value)) >> 8;

	xxxx_10E = NOTATION( td, 4 );
    txBuffer[9] = (uint8_t)xxxx_10E.E;
    txBuffer[10] = (uint8_t)xxxx_10E.value;
	txBuffer[11] = ((uint16_t)(xxxx_10E.value)) >> 8;

	txBuffer[12] = (uint8_t)limit.p;
	txBuffer[13] = ((uint16_t)(limit.p))>>8;

	txBuffer[14] = (uint8_t)limit.i;
	txBuffer[15] = ((uint16_t)(limit.i))>>8;

	txBuffer[16] = (uint8_t)limit.d;
	txBuffer[17] = ((uint16_t)(limit.d))>>8;

	this->responseFlag = true;
}

void CMD::do_PIDSetParam(uint8_t* buffer){
	float kp,ti,td;
	xxPID_Limit_t limit;
	// kp = ((buffer[5]<<8)|buffer[4])   / ((float)pow(10,buffer[3]));
	// ti = ((buffer[8]<<8)|buffer[7])   / ((float)pow(10,buffer[6]));
	// td = ((buffer[11]<<8)|buffer[10]) / ((float)pow(10,buffer[9]));

	struct Note xxxx_10E;

	xxxx_10E.E     = (int8_t)(buffer[3]);
	xxxx_10E.value = (double)((buffer[5]<<8)|buffer[4]); 
	kp = NOTATION(xxxx_10E);

	xxxx_10E.E     = (int8_t)(buffer[6]);
	xxxx_10E.value = (double)((buffer[8]<<8)|buffer[7]); 
	ti = NOTATION(xxxx_10E);

	xxxx_10E.E     = (int8_t)(buffer[9]);
	xxxx_10E.value = (double)((buffer[11]<<8)|buffer[10]); 
	td = NOTATION(xxxx_10E);

	limit.p = ((buffer[13]<<8)|buffer[12]);
	limit.i = ((buffer[15]<<8)|buffer[14]);
	limit.d = ((buffer[17]<<8)|buffer[16]);

	switch(buffer[2]){
		case 0://Thrust PID Module
			break; 
		case 1://Pitch PID Module
			System::PID.pitch.Kp(kp);
			System::PID.pitch.Ti(ti);
			System::PID.pitch.Td(td);
			System::PID.pitch.Limitation(limit);			
			break;
		case 2://Roll PID Module
			System::PID.roll.Kp(kp);
			System::PID.roll.Ti(ti);
			System::PID.roll.Td(td);
			System::PID.roll.Limitation(limit);
			break;
		case 3://Yaw PID Module
			System::PID.yaw.Kp(kp);
			System::PID.yaw.Ti(ti);
			System::PID.yaw.Td(td);
			System::PID.yaw.Limitation(limit);
			break;
		default:
			break;
	}
	this->responseFlag = false;
}

void CMD::do_PIDSetThrottle(uint8_t* buffer){
	this->responseFlag = false;	
	ASSERT_SHAKEHAND(buffer);

	switch(buffer[2]){
		case 1:
			break;
		default:
			System::PID.resetSpeedFlag = true;
			System::PID.averageSpeed = (float)((buffer[4]<<8)|buffer[3]);
			break;
	}
}

void CMD::do_ExitSpeedCTRL(uint8_t* buffer){
	this->responseFlag = false;
	if(buffer[3] == 1){
		switch(buffer[2]){
		case 0:// Motor ALL
			System::motor.speed[0] = \
			System::motor.speed[1] = \
			System::motor.speed[2] = \
			System::motor.speed[3] = 0;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			System::motor.speed[buffer[2]-1] = 0;
			break;
		default:
			break;
		}

	}
	System::enablePIDFlag = true;
}

void CMD::do_KillMotor(uint8_t* buffer){
	uint16_t cnt = (uint16_t)System::PID.averageSpeed;
	cnt/=100;
	cnt*=100;
	while(cnt-=100){
		System::motor.speed[0] = \
		System::motor.speed[1] = \
		System::motor.speed[2] = \
		System::motor.speed[3] = (float)cnt;
		System::motor.updateSpeed();
		DELAY::ms(100);
	}
	System::motor.outputEnable(false);
	while(1);
} 

void CMD::do_AddACK(uint8_t* buffer){
	this->txBuffer[this->bufferSize-1] = QUADROTOR_PACKAGE_ACK;
}






