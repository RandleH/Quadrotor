
#include "main.h"
#include "led.h"
#include "mathematic.h"
#include "delay.h"
#include "jy901s.h"
#include "nrf24l01.h"
#include "adc.h"
#include "motor.h"
#include "system.hpp"


System QUADROTOR;

int main(void){
    
    //freeRTOS Page: 153
    
    

    GT911       screen(true);
    QUADROTOR.sensor.init(LPI2C4);
    QUADROTOR.radio.init(LPSPI4,PCS0,GPIO2,24U,GPIO2,25U,GPIO2,20U);
    LED::RGB_Init();
    LED::RGB_Set(0x04);
    DELAY::ms(100);
    if(QUADROTOR.radio.check() == kStatus_Success)
        LED::RGB_Set(0x02);
    QUADROTOR.radio.rxMode();
    
    screen.printf("APP Start\n");

    QUADROTOR.motor.init(30,70,0,4096);
    DELAY::ms(10);

    while(1){
        if(ADC_ConversionDoneFlag == true){
            ADC_ConversionDoneFlag = false;
            screen.printf("value = %d\n",ADC_ConvertedValue);
            QUADROTOR.motor.speed[0]=QUADROTOR.motor.speed[1]=QUADROTOR.motor.speed[2]=QUADROTOR.motor.speed[3]=ADC_ConvertedValue;
            QUADROTOR.motor.updateSpeed();
            //pwm.duty(ADC_ConvertedValue,0,4096);
            //motor.speed(1,ADC_ConvertedValue,0,4096);
            //motor.speed(2,ADC_ConvertedValue,0,4096);
            //motor.speed(3,ADC_ConvertedValue,0,4096);
            //motor.speed(4,ADC_ConvertedValue,0,4096);
            
        }
        

    }

}

