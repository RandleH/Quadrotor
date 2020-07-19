#include "jy901s.h"
#include "mathematic.h"
uint8_t buffer[30] = {0};

status_t JY901::init(LPI2C_Type* LPI2Cx,LPUART_Type* LPUARTx){
	JY901(LPI2Cx,LPUARTx); 
	return kStatus_Success;
}

status_t JY901::updateData(void){
	status_t result = this->readReg(AX,&buffer[0],24);

	this->angle.pitch = (float)(CHAR2SHORT(&buffer[18])/32768.0*180);
	this->angle.roll  = (float)(CHAR2SHORT(&buffer[20])/32768.0*180);
	this->angle.yaw   = (float)(CHAR2SHORT(&buffer[22])/32768.0*180);
	return result;
}

#ifdef _JY901_DEMO_ENABLE
#include "lcd.h"
#include "delay.h"

void JY901::DEMO(void){
	GT911 screen;
    JY901 sensor(LPI2C3);
    while(1){
        sensor.updateData();
        screen.printf("[pitch] = %d ,[roll] = %d ,[yaw] = %d\n",    (int32_t)(sensor.angle.pitch),\
                                                                    (int32_t)(sensor.angle.roll),\
                                                                    (int32_t)(sensor.angle.yaw));
        DELAY::ms(100);
    }
}


#endif



