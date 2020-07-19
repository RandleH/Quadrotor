#ifndef _OV5640_H
#define _OV5640_H 
#include "fsl_sccb.h"

#define OV5640_SCCB_ADDR 0x3cU
#define OV5640_REVISION 0x5640U

#define OV5640_PID_REG          0x300AU    
#define OV5640_VER_REG          0x300BU  
#define OV5640_FRAME_RATE_30FPS	      0 
#define OV5640_FRAME_RATE_15FPS	      1 
#define OV5640_FRAME_WIDTH            800
#define OV5640_FRAME_HEIGHT           400

#define OV5640_DEVICE_ADDRESS    0x78

#define OV5640_DSP_R_BYPASS     0x05
#define OV5640_DSP_Qs           0x44
#define OV5640_DSP_CTRL         0x50
#define OV5640_DSP_HSIZE1       0x51
#define OV5640_DSP_VSIZE1       0x52
#define OV5640_DSP_XOFFL        0x53
#define OV5640_DSP_YOFFL        0x54
#define OV5640_DSP_VHYX         0x55
#define OV5640_DSP_DPRP         0x56
#define OV5640_DSP_TEST         0x57
#define OV5640_DSP_ZMOW         0x5A
#define OV5640_DSP_ZMOH         0x5B
#define OV5640_DSP_ZMHH         0x5C
#define OV5640_DSP_BPADDR       0x7C
#define OV5640_DSP_BPDATA       0x7D
#define OV5640_DSP_CTRL2        0x86
#define OV5640_DSP_CTRL3        0x87
#define OV5640_DSP_SIZEL        0x8C
#define OV5640_DSP_HSIZE2       0xC0
#define OV5640_DSP_VSIZE2       0xC1
#define OV5640_DSP_CTRL0        0xC2
#define OV5640_DSP_CTRL1        0xC3
#define OV5640_DSP_R_DVP_SP     0xD3
#define OV5640_DSP_IMAGE_MODE   0xDA
#define OV5640_DSP_RESET        0xE0
#define OV5640_DSP_MS_SP        0xF0
#define OV5640_DSP_SS_ID        0x7F
#define OV5640_DSP_SS_CTRL      0xF8
#define OV5640_DSP_MC_BIST      0xF9
#define OV5640_DSP_MC_AL        0xFA
#define OV5640_DSP_MC_AH        0xFB
#define OV5640_DSP_MC_D         0xFC
#define OV5640_DSP_P_STATUS     0xFE
#define OV5640_DSP_RA_DLMT      0xFF

#define OV5640_SENSOR_GAIN       0x00
#define OV5640_SENSOR_COM1       0x03
#define OV5640_SENSOR_REG04      0x04
#define OV5640_SENSOR_REG08      0x08
#define OV5640_SENSOR_COM2       0x09

#define OV5640_SENSOR_PIDH       0x300A
#define OV5640_SENSOR_PIDL       0x300B

#define OV5640_SENSOR_COM3       0x0C
#define OV5640_SENSOR_COM4       0x0D
#define OV5640_SENSOR_AEC        0x10
#define OV5640_SENSOR_CLKRC      0x11
#define OV5640_SENSOR_COM7       0x12
#define OV5640_SENSOR_COM8       0x13
#define OV5640_SENSOR_COM9       0x14
#define OV5640_SENSOR_COM10      0x15
#define OV5640_SENSOR_HREFST     0x17
#define OV5640_SENSOR_HREFEND    0x18
#define OV5640_SENSOR_VSTART     0x19
#define OV5640_SENSOR_VEND       0x1A


#define OV5640_SENSOR_MIDH       0x1C
#define OV5640_SENSOR_MIDL       0x1D
   
#define OV5640_SENSOR_AEW        0x24
#define OV5640_SENSOR_AEB        0x25
#define OV5640_SENSOR_W          0x26
#define OV5640_SENSOR_REG2A      0x2A
#define OV5640_SENSOR_FRARL      0x2B
#define OV5640_SENSOR_ADDVSL     0x2D
#define OV5640_SENSOR_ADDVHS     0x2E
#define OV5640_SENSOR_YAVG       0x2F
#define OV5640_SENSOR_REG32      0x32
#define OV5640_SENSOR_ARCOM2     0x34
#define OV5640_SENSOR_REG45      0x45
#define OV5640_SENSOR_FLL        0x46
#define OV5640_SENSOR_FLH        0x47
#define OV5640_SENSOR_COM19      0x48
#define OV5640_SENSOR_ZOOMS      0x49
#define OV5640_SENSOR_COM22      0x4B
#define OV5640_SENSOR_COM25      0x4E
#define OV5640_SENSOR_BD50       0x4F
#define OV5640_SENSOR_BD60       0x50
#define OV5640_SENSOR_REG5D      0x5D
#define OV5640_SENSOR_REG5E      0x5E
#define OV5640_SENSOR_REG5F      0x5F
#define OV5640_SENSOR_REG60      0x60
#define OV5640_SENSOR_HISTO_LOW  0x61
#define OV5640_SENSOR_HISTO_HIGH 0x62


#define OV5640_REG_SCCB_SYSTEM_CTRL1 0x3103
#define OV5640_REG_FORMAT_CONTROL_00 0x4300
#define OV5640_REG_JPEG_CTRL00       0x4400
#define OV5640_REG_JPEG_CTRL01       0x4401
#define OV5640_REG_JPEG_CTRL02       0x4402
#define OV5640_REG_JPEG_CTRL03       0x4403
#define OV5640_REG_JPEG_CTRL04       0x4404

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

typedef struct{	
	uint8_t  frame_rate;	
	
	uint16_t cam_isp_sx; 
	uint16_t cam_isp_sy; 

	uint16_t cam_isp_width; 
	uint16_t cam_isp_height; 

	uint8_t  scaling;				
	
	uint16_t cam_out_sx; 
	uint16_t cam_out_sy;
	
	uint16_t cam_out_width;
	uint16_t cam_out_height;
	
	int      lcd_sx;
	int      lcd_sy;
	uint8_t  lcd_scan;
	
	uint8_t  light_mode;
	int8_t   saturation;  
	int8_t   brightness;
	int8_t   contrast;
	uint8_t  effect;		
	int8_t   exposure;

	uint8_t  auto_focus;
}OV5640_Param;

typedef struct {
    sccb_i2c_t sccbI2C;                    
    void (*pullResetPin)(bool pullUp);    
    void (*pullPowerDownPin)(bool pullUp); 
    uint32_t inputClockFreq_Hz;
}OV5640_Resource;

typedef struct{
    uint16_t reg;
    uint8_t  data;
}OV5640_Reg_Data;

extern const uint8_t         OV5640_saturationRegs[7][6];
extern const uint8_t         OV5640_exposureRegs[7][6];
extern const uint8_t         OV5640_lightModeRegs[5][7];
extern const uint8_t         OV5640_effectRegs[9][4];
extern const OV5640_Reg_Data OV5640_initRegs[];
extern const size_t          OV5640_NumOfInitRegs;
extern const uint8_t         OV5640_contrastRegs[7][2];
extern const uint8_t         OV5640_AF_FW[];
extern const size_t          OV5640_NumOfAF_FW;

extern OV5640_Param ov5640_param_640_480;

#ifdef __cplusplus
}
#endif
#endif
