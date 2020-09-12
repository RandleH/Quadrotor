
#include "fsl_camera.h"
#ifndef _CAMERA_H
#define _CAMERA_H 

#define CAMERA_RST_GPIO 		GPIO1
#define CAMERA_RST_GPIO_PIN 	1
#define CAMERA_PWR_GPIO 		GPIO1
#define CAMERA_PWR_GPIO_PIN 	0
#if defined(__cplusplus)
extern "C" {
#endif
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_lpi2c.h"
#include "fsl_clock.h"
#include "fsl_csi.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_csi_camera_adapter.h"
extern void CSI_DriverIRQHandler(void);
#if defined(__cplusplus)
}
#endif
class CAMERA{
public:
	CAMERA(void){}
	CAMERA(LPI2C_Type* LPI2Cx){
		CAMERA::IOMUXC_MUX_Config();
		CAMERA::IOMUXC_PAD_Config();
		CAMERA::RESOURCE_Config(LPI2Cx);
		this->LPI2Cx = LPI2Cx;
	}
protected:
	void IOMUXC_PAD_Config(void){    
		IOMUXC_SetPinConfig(
		  IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 PAD functional properties : */
		  0xD8B0u);                              
		IOMUXC_SetPinConfig(
		  IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 PAD functional properties : */
		  0xD8B0u);                                                       
	}
	void IOMUXC_MUX_Config(void){
		CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B0_00_GPIO1_IO00,        /* CSI_PWR GPIO_AD_B0_04 is configured as GPIO1_IO04 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B0_01_GPIO1_IO01,        /* CSI_RST GPIO_AD_B0_04 is configured as GPIO1_IO04 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */

		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 is configured as LPI2C1_SCL */
		  1U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 is configured as LPI2C1_SDA */
		  1U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_B1_12_CSI_PIXCLK,        /* GPIO_AD_B1_04 is configured as CSI_PIXCLK */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_06_CSI_VSYNC,         /* GPIO_AD_B1_06 is configured as CSI_VSYNC */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,         /* GPIO_AD_B1_07 is configured as CSI_HSYNC */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */

		//CSI[9:2]	
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_08_CSI_DATA09,        /* GPIO_AD_B1_08 is configured as CSI_DATA09 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_09_CSI_DATA08,        /* GPIO_AD_B1_09 is configured as CSI_DATA08 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_10_CSI_DATA07,        /* GPIO_AD_B1_10 is configured as CSI_DATA07 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_11_CSI_DATA06,        /* GPIO_AD_B1_11 is configured as CSI_DATA06 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_12_CSI_DATA05,        /* GPIO_AD_B1_12 is configured as CSI_DATA05 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_13_CSI_DATA04,        /* GPIO_AD_B1_13 is configured as CSI_DATA04 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_14_CSI_DATA03,        /* GPIO_AD_B1_14 is configured as CSI_DATA03 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
		  IOMUXC_GPIO_AD_B1_15_CSI_DATA02,        /* GPIO_AD_B1_15 is configured as CSI_DATA02 */
		  0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	}
	void RESOURCE_Config(LPI2C_Type* LPI2Cx){
		lpi2c_master_config_t masterConfig;
		uint32_t sourceClock;
		LPI2C_MasterGetDefaultConfig(&masterConfig);
		masterConfig.baudRate_Hz = 400000;
		masterConfig.debugEnable = true;
		masterConfig.ignoreAck = true;
		CLOCK_SetMux(kCLOCK_Lpi2cMux, 1);
		CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);
		sourceClock = CLOCK_GetOscFreq();

		LPI2C_MasterInit(LPI2Cx, &masterConfig, sourceClock);

		gpio_pin_config_t pinConfig = {
		    kGPIO_DigitalOutput, 1,
		};
		GPIO_PinInit(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, &pinConfig);
		GPIO_PinInit(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, &pinConfig);
	}
protected:
	LPI2C_Type*                LPI2Cx;
	camera_config_t            camera_cfg;
	camera_receiver_handle_t   camera_recv_handle;
	camera_device_handle_t     camera_dev_handle;
	camera_device_operations_t camera_ops;
	csi_resource_t             csi_resource;
	csi_private_data_t         csi_privateData;
	size_t                     bytesPerPixel;
};

#include "ov5640.h"
class OV5640:public CAMERA{
public:
	OV5640(){};
	status_t init(LPI2C_Type* LPI2Cx);
	void     reset(bool pullUp);
	void     pwr(bool pullUp);
	void     setImgSize(uint16_t x_st,uint16_t y_st,uint16_t width,uint16_t height);
    void     setOutSize(uint8_t scaling,uint16_t x_off,uint16_t y_off,uint16_t width,uint16_t height);
    void     brightness(int8_t value);
    void     saturation(int8_t value);
    void     contrast(int8_t value);
    void     exposure(int8_t value);
    void     lightMode(uint8_t mode);
    void     specialEffect(uint8_t effect);
    uint8_t  constantFocus(void);
    uint32_t activeFrameAddr;
    uint32_t inactiveFrameAddr;
private:
    OV5640_Param    param;
    //OV5640_Resource resource;

  
    void     hw_init(void);
    status_t initExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);
    status_t initAutoFocus(void);
    status_t start(camera_device_handle_t *handle);
    status_t stop(camera_device_handle_t *handle);
    status_t softwareReset(void);
    void     delay(uint32_t ms);
    void     setFrameRate(void);
    
    status_t readReg(uint32_t reg,uint8_t* data);
    status_t writeReg(uint32_t reg,uint8_t data);
    status_t writeReg(const OV5640_Reg_Data srcs[], size_t num);
    status_t writeFirmWork(uint8_t *pBuffer ,uint16_t BufferSize);

};

#endif