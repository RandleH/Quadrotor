#ifndef _RADIO_H
#define _RADIO_H 
#include <cstring>

#define __PRIVATE_RADIO_PROTOCAL
//#define __SBUS_RADIO_PROTOCAL
//#define __MAVLINK_RADIO_PROTOCAL

typedef enum{
	__RADIO_DEBUG_MODE__,
	__STD_CTRL_MODE__,
	__RADIO_CTRL_MODE__,
	__DISCONNECT_MODE__
}RadioMode_t;



class RadioMessage{
public:
	RadioMessage(RadioMode_t mode):mode(mode),dataUpdated(false){

	}
	String  response;
	uint8_t buffer[32];
	bool    dataUpdated;
private:
	RadioMode_t mode;
	uint8_t     CKNUM;
};

#endif