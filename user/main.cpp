
#include "main.h"
#include "led.h"
#include "mathematic.h"
#include "delay.h"
#include "jy901s.h"
#include "nrf24l01.h"
#include "adc.h"
#include "motor.h"
#include "os.hpp"
#include "CMD_Desk.h"

System QUADROTOR;
CMD    COMMAND;

extern "C"{
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        signed char *pcTaskName ){
#ifdef SYSTEM_UART_DEBUG_CONSOLE
        PRINTF("Stack overflow at ""%s""\n",(const char*)pcTaskName);
#endif
    }
}

int main(void){
    
    //freeRTOS Page: 153
    //QUADROTOR.initRadioFlag      = false;
    //QUADROTOR.initGryoSensorFlag = false;
    System::initCameraFlag     = false;
    //QUADROTOR.initMotorFlag      = false;
    System::init();
    System::OS.assignTask(Task_1,System::showLEDStatus     ,"LED Task"     ,2);
    System::OS.assignTask(Task_2,System::updateGyroData    ,"Gyroscope"    ,3);
    System::OS.assignTask(Task_3,System::readRadioData     ,"Radio Connect",3);
    System::OS.assignTask(Task_5,System::updateMotorSpeed  ,"Motor Task"   ,3);
    System::OS.assignTask(Task_6,System::pidControl        ,"PID Ctrl"     ,3);
    
    System::OS.start();
    while(1);


}

