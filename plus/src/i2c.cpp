#include "i2c.h"


void I2C::IOMUXC_MUX_PAD_RESOURCE_Config(const lpi2c_master_config_t* p){
	switch((uint32_t)this->LPI2Cx){
		case LPI2C1_BASE:		IOMUXC_SetPinMux   (LPI2C1_SCL, 1U);                                   
								IOMUXC_SetPinMux   (LPI2C1_SDA, 1U);
								IOMUXC_SetPinConfig(LPI2C1_SCL, I2C_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPI2C1_SDA, I2C_PAD_CONFIG_DATA);break;
			case LPI2C2_BASE:	IOMUXC_SetPinMux   (IOMUXC_GPIO_B0_04_LPI2C2_SCL, 1U);                                   
								IOMUXC_SetPinMux   (IOMUXC_GPIO_B0_05_LPI2C2_SDA, 1U);
								IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LPI2C2_SCL, I2C_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LPI2C2_SDA, I2C_PAD_CONFIG_DATA);break;
			case LPI2C3_BASE:	IOMUXC_SetPinMux   (LPI2C3_SCL, 1U);                                   
								IOMUXC_SetPinMux   (LPI2C3_SDA, 1U);
								IOMUXC_SetPinConfig(LPI2C3_SCL, I2C_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPI2C3_SDA, I2C_PAD_CONFIG_DATA);break;
			case LPI2C4_BASE:	IOMUXC_SetPinMux   (LPI2C4_SCL, 1U);                                   
								IOMUXC_SetPinMux   (LPI2C4_SDA, 1U);
								IOMUXC_SetPinConfig(LPI2C4_SCL, I2C_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPI2C4_SDA, I2C_PAD_CONFIG_DATA);break;
	}
	CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);	// 0 = PLL3_SW_CLK(60MHz); 1 = EXTERN_OSC(24MHz); 
	CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);	// 60MHz/(5+1) = 10MHz;
	
	if(p==NULL){
		lpi2c_master_config_t masterConfig;
		LPI2C_MasterGetDefaultConfig(&masterConfig);
		masterConfig.baudRate_Hz = this->baudRate_Hz;
		LPI2C_MasterInit(this->LPI2Cx, &masterConfig, LPI2C_CLOCK_FREQUENCY);
	}else{
		LPI2C_MasterInit(this->LPI2Cx, p, LPI2C_CLOCK_FREQUENCY);
	}

}

status_t I2C::readReg(uint32_t reg,uint8_t* data){
	return this->readReg(reg,data,1);
}

status_t I2C::readReg(uint32_t reg,uint8_t* dataBuffer,size_t num){
	lpi2c_master_transfer_t masterXfer = {0};
  	masterXfer.slaveAddress   = this->hw_Addr;
	masterXfer.direction      = kLPI2C_Read;
	masterXfer.subaddress     = (uint32_t)reg;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = dataBuffer;		
	masterXfer.dataSize       = num;		
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;

	return LPI2C_MasterTransferBlocking(this->LPI2Cx, &masterXfer);
}

status_t I2C::writeReg(uint32_t reg,uint8_t  data){
	return this->writeReg(reg,&data,1);
}

status_t I2C::writeReg(uint32_t reg,uint8_t* dataBuffer,size_t num){
	lpi2c_master_transfer_t masterXfer = {0};
 	masterXfer.slaveAddress   = this->hw_Addr;
	masterXfer.direction      = kLPI2C_Write;			
	masterXfer.subaddress     = reg;				
	masterXfer.subaddressSize = 1;						
	masterXfer.data           = dataBuffer;							
	masterXfer.dataSize       = num;			
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;
  
	return LPI2C_MasterTransferBlocking(this->LPI2Cx, &masterXfer);
}
