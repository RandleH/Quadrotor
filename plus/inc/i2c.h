#ifndef _I2C_H
#define _I2C_H 

#include "fsl_lpi2c.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"

#define USE_LPI2C1_GROUP1
#define USE_LPI2C2_GROUP1
#define USE_LPI2C3_GROUP2 

#if (!defined ( LPI2C1_SCL ))||(!defined( LPI2C1_SDA ))
  #ifdef USE_LPI2C1_GROUP1
	#define  LPI2C1_SCL IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
	#define  LPI2C1_SDA IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA
  #elif defined USE_LPI2C1_GROUP2
	#define  LPI2C1_SCL IOMUXC_GPIO_SD_B1_04_LPI2C1_SCL
	#define  LPI2C1_SDA IOMUXC_GPIO_SD_B1_05_LPI2C1_SDA
  #else
	#warning "Ambiguous [LPI2C1] Pin Group Detected! Please Add Defination at "i2c.h"
	#define  LPI2C1_SCL IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
	#define  LPI2C1_SDA IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA
  #endif
#endif

#if (!defined ( LPI2C2_SCL ))||(!defined( LPI2C2_SCL ))
  #ifdef USE_LPI2C2_GROUP1
	#define  LPI2C2_SCL IOMUXC_GPIO_B0_04_LPI2C2_SCL
	#define  LPI2C2_SDA IOMUXC_GPIO_B0_05_LPI2C2_SDA
  #elif defined USE_LPI2C2_GROUP2
	#define  LPI2C2_SCL IOMUXC_GPIO_SD_B1_11_LPI2C2_SCL// B HARDWARE FAULT
	#define  LPI2C2_SDA IOMUXC_GPIO_SD_B1_10_LPI2C2_SDA// B HARDWARE FAULT
  #else
	#warning "Ambiguous [LPI2C2] Pin Group Detected! Please Add Defination at "i2c.h""
	#define  LPI2C2_SCL IOMUXC_GPIO_B0_04_LPI2C2_SCL
	#define  LPI2C2_SDA IOMUXC_GPIO_B0_05_LPI2C2_SDA
  #endif
#endif

#if (!defined ( LPI2C3_SCL ))||(!defined( LPI2C3_SCL ))
  #ifdef USE_LPI2C3_GROUP1
	#define  LPI2C3_SCL IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL
	#define  LPI2C3_SDA IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA
  #elif defined USE_LPI2C3_GROUP2
	#define  LPI2C3_SCL IOMUXC_GPIO_SD_B0_00_LPI2C3_SCL
	#define  LPI2C3_SDA IOMUXC_GPIO_SD_B0_01_LPI2C3_SDA
  #else
	#warning "Ambiguous [LPI2C3] Pin Group Detected! Please Add Defination at "i2c.h""
	#define  LPI2C3_SCL IOMUXC_GPIO_AD_B1_07_LPI2C3_SCL
	#define  LPI2C3_SDA IOMUXC_GPIO_AD_B1_06_LPI2C3_SDA
  #endif
#endif

#define  LPI2C4_SCL IOMUXC_GPIO_AD_B0_12_LPI2C4_SCL
#define  LPI2C4_SDA IOMUXC_GPIO_AD_B0_13_LPI2C4_SDA

#define LPI2C_CLOCK_SOURCE_SELECT     (0U)
#define LPI2C_CLOCK_SOURCE_DIVIDER    (5U)
#define LPI2C_CLOCK_FREQUENCY         ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define LPI2C_MASTER_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

#define I2C_PAD_CONFIG_DATA         (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_1_OPEN_DRAIN_ENABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED) 

#ifndef __100KHz__
#define __100KHz__ 100000U
#endif
#ifndef __200KHz__
#define __200KHz__ 200000U
#endif
#ifndef __300KHz__
#define __300KHz__ 300000U
#endif
#ifndef __400KHz__
#define __400KHz__ 400000U
#endif

class I2C{
public:
	I2C(void){}
	
	I2C(LPI2C_Type* LPI2Cx , uint8_t hw_Addr ,lpi2c_master_config_t* p){
		this->LPI2Cx  = LPI2Cx;
		this->hw_Addr = hw_Addr;
		this->IOMUXC_MUX_PAD_RESOURCE_Config(p);
	}
	void init(LPI2C_Type* LPI2Cx, uint8_t hw_Addr ,uint32_t baudRate_Hz);
protected:
	LPI2C_Type* LPI2Cx;
	uint8_t     hw_Addr;
	uint32_t    baudRate_Hz;
	status_t readReg(uint32_t reg,uint8_t* data);
	status_t readReg(uint32_t reg,uint8_t* dataBuffer,size_t num);
	status_t writeReg(uint32_t reg,uint8_t  data);
	status_t writeReg(uint32_t reg,uint8_t* dataBuffer,size_t num);
private:
	void IOMUXC_MUX_PAD_RESOURCE_Config(const lpi2c_master_config_t* p);
};

#endif