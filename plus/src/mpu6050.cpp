#include "mpu6050.hpp"
#include "delay.h"

LPI2C_Type* MPU6050_DMP::LPI2Cx_Current = NULL;

status_t MPU6050_DMP::init(void){
	this->writeReg(MPU6050_RA_PWR_MGMT_1   , 0x80);		//Reset all register	    

	//DELAY::core_us(100000);		//Doesn't work
	DELAY::ms(100);					//Very important
	this->writeReg(MPU6050_RA_PWR_MGMT_1   , 0x00);		//Wake up	
	this->setGyroFsr(3);								//FS_SEL : 2000˚/S
	this->setAccFsr(0);   								//AFS_SEL: 2g
	this->setSampleRate(__50Hz__);
	this->writeReg(MPU6050_RA_INT_ENABLE   , 0X00); 	//Interrupt
	this->writeReg(MPU6050_RA_USER_CTRL    , 0x00);		//
	this->writeReg(MPU6050_RA_FIFO_EN      , 0x00); 	//
	this->writeReg(MPU6050_RA_INT_PIN_CFG  , 0x80); 	//
	if(kStatus_Success == this->readID()){
		this->writeReg(MPU6050_RA_PWR_MGMT_1,0x01);		//CLK Select: PLL with X Axis Gyroscope Reference
		this->writeReg(MPU6050_RA_PWR_MGMT_2,0x00);
	}
	else
		return kStatus_Fail;
	
	this->setSampleRate(__50Hz__); //Sample Rate: Gyroscope_Output_Rate_1KHz / (1 + SMPLRT_DIV)
	this->setDLPF      (__25Hz__); //Digital LPF
	//DELAY::core_us(50);			//Doesn't work
	DELAY::us(50);					//Very important
	return kStatus_Success;
}

status_t MPU6050_DMP::readID(void){
	uint8_t result = 0;
	this->readReg(MPU6050_RA_WHO_AM_I,&result,1);
	if(result == MPU6050_DEFAULT_ADDRESS)
		return kStatus_Success;
	return kStatus_Fail;
}

status_t MPU6050_DMP::updateData(void){
	uint8_t buf[6] = {0};
	status_t state = kStatus_Fail;
	state = this->readReg(MPU6050_GYRO_OUT,&buf[0],6);
	if(state == kStatus_Fail) 
		return state;
	this->data[gyro_X] = ((buf[0]<<8)|buf[1]);
	this->data[gyro_Y] = ((buf[2]<<8)|buf[3]);
	this->data[gyro_Z] = ((buf[4]<<8)|buf[5]);
	state = this->readReg(MPU6050_ACC_OUT,buf,6);
	this->data[acc_X] = ((buf[0]<<8)|buf[1]);
	this->data[acc_Y] = ((buf[2]<<8)|buf[3]);
	this->data[acc_Z] = ((buf[4]<<8)|buf[5]);
	return state;
}

uint8_t  MPU6050_DMP::updateQuad(void){
#ifdef _MPU6050_DMP_SUPPORT
	return mpu_dmp_get_data(&this->quad[0],&this->quad[1],&this->quad[2]);
#elif
	#error "Software converting raw data currently is not supported!"
#endif
}

float    MPU6050_DMP::temperature(void){
	short tmp;
	uint8_t buf[2] = {0};
	this->readReg(MPU6050_RA_TEMP_OUT_H,buf,2);
	tmp = (buf[0]<<8)|(buf[1]);

	return (float)((tmp/340.0)+36.53);
}

int16_t  MPU6050_DMP::get(MPU6050_DataContent_t value){
	assert(value < 6);
	return this->data[value];
}

float    MPU6050_DMP::get(MPU6050_DMP_DataContent_t value){
	return this->quad[value];
}

status_t MPU6050_DMP::setSampleRate(uint16_t Hz){
	assert(Hz < 1000 && Hz > 4);
	uint16_t SMPLRT_DIV = MPU6050_CONVERT_TO_SMPDIV(Hz);

	return this->writeReg(MPU6050_RA_SMPLRT_DIV , SMPLRT_DIV);
}

status_t MPU6050_DMP::setDLPF(uint16_t Hz){
	assert(Hz < 500);
	uint8_t data=0;
	if(Hz>=188)    data=1;
	else if(Hz>=98)data=2;
	else if(Hz>=42)data=3;
	else if(Hz>=20)data=4;
	else if(Hz>=10)data=5;
	else data=6;

	return this->writeReg(MPU6050_RA_CONFIG , data);
}

status_t MPU6050_DMP::setGyroFsr(uint8_t fs_sel){
	assert(fs_sel <= 3);

	return this->writeReg(MPU6050_RA_GYRO_CONFIG , fs_sel<<3);
}

status_t MPU6050_DMP::setAccFsr(uint8_t fs_sel){
	assert(fs_sel <= 3);

	return this->writeReg(MPU6050_RA_ACCEL_CONFIG , fs_sel<<3);
}

status_t MPU6050_DMP::sensorStandBy(void){
	uint8_t data = 0;

	this->sleep();
	data |= (uint8_t)(1U << MPU6050_PWR2_STBY_XG_BIT);
	data |= (uint8_t)(1U << MPU6050_PWR2_STBY_YG_BIT);
	data |= (uint8_t)(1U << MPU6050_PWR2_STBY_ZG_BIT);
	data |= (uint8_t)(1U << MPU6050_PWR2_STBY_XA_BIT);
	data |= (uint8_t)(1U << MPU6050_PWR2_STBY_YA_BIT);
	data |= (uint8_t)(1U << MPU6050_PWR2_STBY_ZA_BIT);

	this->writeReg(MPU6050_RA_PWR_MGMT_2 , data);
	return this->writeReg(MPU6050_RA_PWR_MGMT_1,0x01);		//CLK Select: PLL with X Axis Gyroscope Reference
}

status_t MPU6050_DMP::sleep(void){
	return this->writeReg(MPU6050_RA_PWR_MGMT_1 , (uint8_t)(1 << MPU6050_PWR1_SLEEP_BIT) );
}

status_t MPU6050_DMP::reset(void){
	return this->writeReg(MPU6050_RA_PWR_MGMT_1 , (uint8_t)(1 << MPU6050_PWR1_DEVICE_RESET_BIT) );
}



#ifdef _MPU6050_DMP_SUPPORT
uint8_t (*DMP_API_I2C_WriteDMP_Handle)(uint8_t,uint8_t,uint8_t,unsigned char*) = MPU6050_DMP::API_I2C_WriteDMP;
uint8_t (*DMP_API_I2C_ReadDMP_Handle)(uint8_t,uint8_t ,uint8_t,unsigned char*) = MPU6050_DMP::API_I2C_ReadDMP;

uint8_t MPU6050_DMP::API_I2C_WriteDMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* write){
	lpi2c_master_transfer_t masterXfer = {0};

	masterXfer.slaveAddress   = MPU6050_DEFAULT_ADDRESS;
	masterXfer.direction      = kLPI2C_Write;			
	masterXfer.subaddress     = (uint32_t)reg_add;				
	masterXfer.subaddressSize = 1;						
	masterXfer.data           = write;							
	masterXfer.dataSize       = num;			
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;

	return LPI2C_MasterTransferBlocking(LPI2Cx_Current, &masterXfer);
}

uint8_t MPU6050_DMP::API_I2C_ReadDMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* read){
	lpi2c_master_transfer_t masterXfer = {0};

	masterXfer.slaveAddress   = MPU6050_DEFAULT_ADDRESS;
	masterXfer.direction      = kLPI2C_Read;
	masterXfer.subaddress     = (uint32_t)reg_add;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = read;		
	masterXfer.dataSize       = num;		
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;

	return LPI2C_MasterTransferBlocking(LPI2Cx_Current, &masterXfer);
}
#endif

#ifdef _MPU6050_DEMO_ENABLE
#include "led.h"
#include "lcd.h"
void MPU6050_DMP::DEMO_DMP_DATA(void){
	LED::RGB_Init();
    LED::RGB_Set(0x00);// R G B 
	GT911  screen(true);
    screen.setFont(Font16x32);
	MPU6050_DMP sensor(LPI2C1,"Enable DMP");
	while(1){
	    if(sensor.updateQuad()==0){
	        LED::RGB_Set(0x02);
	        screen.printf("Pitch :%4d Roll :%4d Yaw :%4d \n",(int32_t)(sensor.get(pitch)*1000),\
	                                                         (int32_t)(sensor.get(roll)*1000 ),\
	                                                         (int32_t)(sensor.get(yaw)*1000  ));
	    }
	}     
}

void MPU6050_DMP::DEMO_RAW_DATA(void){
	LED::RGB_Init();
    LED::RGB_Set(0x00);// R G B 
    GT911  screen(true);
    screen.setFont(Font16x32);
	MPU6050_DMP sensor(LPI2C1);

	int16_t gX=0,gY=0,gZ=0;
    int16_t aX=0,aY=0,aZ=0;
    float   T =0.0;
	while(1){
		if(sensor.updateData() == 0){
			gX = sensor.get(gyro_X);
	        gY = sensor.get(gyro_Y);
	        gZ = sensor.get(gyro_Z);
	        aX = sensor.get(acc_X);
	        aY = sensor.get(acc_Y);
	        aZ = sensor.get(acc_Z);
	        T  = sensor.temperature();
	        screen.printf("gX:%8d; gY:%8d; gZ:%8d\n",gX,gY,gZ);
	        screen.printf("aX:%8d; aY:%8d; aZ:%8d\n",aX,aY,aZ);
	        screen.printf("temperature:%f\n",T);
		}
	}
}
#endif

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif







