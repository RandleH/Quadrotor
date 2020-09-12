#ifndef _CMD_DESK
#define _CMD_DESK 

#define CMD_DESK_SUPPORT

#define QUADROTOR_PACKAGE_HEAD 				0x51
#define QUADROTOR_PACKAGE_ACK       		0xac
#define PACKAGE_ACK_INDEX					(this->bufferSize-1)
#define PACKAGE_FUNC_INDEX					1
		
//Package Function:		
#define QUADROTOR_CMD_CONNECT				0x00
#define QUADROTOR_CMD_TESTMOTOR     		0x01
#define QUADROTOR_CMD_PIDQUERY				0x10
#define QUADROTOR_CMD_PIDADJUSTER   		0x11
#define QUADROTOR_CMD_PIDTHROTTLE   		0x12
#define QUADROTOR_CMD_DISCONNECT			0xf0
#define QUADROTOR_CMD_KILLMOTOR 			0xf1

#define QUADROTOR_CMD_EXIT_TESTMOTOR 		0xd1
#define QUADROTOR_CMD_EXIT_PIDADJUSTER		0xe1
#define QUADROTOR_CMD_EXIT_PIDTHROTTLE		0xe2

#define MK_FUNC(pArray,index)				pArray[1]  = index
#define MK_PKGHEAD(pArray)       			pArray[0]  = QUADROTOR_PACKAGE_HEAD
#define MK_PKGACK(pArray)           		pArray[31] = QUADROTOR_PACKAGE_ACK
#define MK_SHAKEHAND(pArray)				pArray[30] = 0x00
#define RM_SHAKEHAND(pArray)				pArray[30] = 0xff
	
#define ASSERT_PKGHEAD(pArray)				if(pArray[0]  != QUADROTOR_PACKAGE_HEAD ) return
#define ASSERT_PKGACK(pArray)   			if(pArray[31] != QUADROTOR_PACKAGE_ACK  ) return 
#define ASSERT_SHAKEHAND(pArray)			if(pArray[30] == 0x00)                    return


#include <cstdint>
#include <climits>
#include <cstddef>
#include "fsl_common.h"
class CMD{
public:
	CMD(void):bufferSize(32),responseFlag(false){}
	status_t analize(uint8_t* buffer,size_t num);
private:
	void do_Connect 		(uint8_t*);
	void do_MotorControl	(uint8_t*);
	void do_MotorSwitch     (uint8_t*);
	void do_STOP			(uint8_t*);
	void do_PIDModuleSwitch (uint8_t*);
	void do_PIDMotorSwitch  (uint8_t*);
	void do_PIDSendParam	(uint8_t*);
	void do_PIDSetParam		(uint8_t*);
	void do_PIDSetThrottle  (uint8_t*);
	void do_KillMotor       (uint8_t*); // [!] This is a non-callback function. System will halt immediately.
	void do_Disconnect		(uint8_t*);
	void do_ExitSpeedCTRL   (uint8_t*);

	inline void do_AddACK   (uint8_t* buffer);
	const size_t bufferSize;
	bool responseFlag;
	uint8_t txBuffer[32];
};

#endif


