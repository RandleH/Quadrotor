
#include "ov5640.h"

const uint8_t OV5640_saturationRegs[7][6]={ 
    0X0C,0x30,0X3D,0X3E,0X3D,0X01,//-3 
    0X10,0x3D,0X4D,0X4E,0X4D,0X01,//-2  
    0X15,0x52,0X66,0X68,0X66,0X02,//-1  
    0X1A,0x66,0X80,0X82,0X80,0X02,//0  
    0X1F,0x7A,0X9A,0X9C,0X9A,0X02,//+1  
    0X24,0x8F,0XB3,0XB6,0XB3,0X03,//+2
    0X2B,0xAB,0XD6,0XDA,0XD6,0X04,//+3
}; 

const uint8_t OV5640_contrastRegs[7][2] = {
    0x2C,0x1C,
    0x28,0x18,
    0x24,0x10,
    0x20,0x00,
    0x1C,0x1C,
    0x18,0x18,
    0x14,0x14,
};

const uint8_t OV5640_exposureRegs[7][6]={
    0x10,0x08,0x10,0x08,0x20,0x10,//-3  
    0x20,0x18,0x41,0x20,0x18,0x10,//-2
    0x30,0x28,0x61,0x30,0x28,0x10,//-1 
    0x38,0x30,0x61,0x38,0x30,0x10,//0  
    0x40,0x38,0x71,0x40,0x38,0x10,//+1 
    0x50,0x48,0x90,0x50,0x48,0x20,//+2   
    0x60,0x58,0xa0,0x60,0x58,0x20,//+3    
};

const uint8_t OV5640_lightModeRegs[5][7]={ 
    0x04,0X00,0X04,0X00,0X04,0X00,0X00,//Auto
    0x06,0X1C,0X04,0X00,0X04,0XF3,0X01,//Sunny
    0x05,0X48,0X04,0X00,0X07,0XCF,0X01,//Office
    0x06,0X48,0X04,0X00,0X04,0XD3,0X01,//Cloudy
    0x04,0X10,0X04,0X00,0X08,0X40,0X01,//Home
}; 

const uint8_t OV5640_effectRegs[9][4]={ 
    0X06,0x40,0X10,0X08,
    0X1E,0xA0,0X40,0X08,
    0X1E,0x80,0XC0,0X08,
    0X1E,0x80,0X80,0X08,
    0X1E,0x40,0XA0,0X08,
    0X40,0x40,0X10,0X08,
    0X1E,0x60,0X60,0X08,
    0X1E,0xF0,0XF0,0X08,
    0X06,0x40,0X10,0X09,
};

OV5640_Param ov5640_param_640_480 = {
    .frame_rate     = OV5640_FRAME_RATE_30FPS,	
	.cam_isp_sx     = 0,
	.cam_isp_sy     = 0,	
	
	.cam_isp_width  = 1920,
	.cam_isp_height = 1080,
	
	.scaling        = 1,      
	.cam_out_sx     = 16,	
	.cam_out_sy     = 4,	  
	.cam_out_width  = 640,
	.cam_out_height = 480,
	
	.lcd_sx         = -80,
	.lcd_sy         = 0,
	.lcd_scan       = 5,
	
	.light_mode     = 0,
	.saturation     = 0,	
	.brightness     = 0,
	.contrast       = 0,
	.effect         = 0,		
	.exposure       = 0,		

	.auto_focus     = 1,
};

const OV5640_Reg_Data OV5640_initRegs[] = {
    //15fps VGA RGB565 output
    // 24MHz input clock, 24MHz PCLK
    0x3103, 0x11, // system clock from pad, bit[1]
    0x3008, 0x82, // software reset, bit[7]
    // OV5640_DelayMs 5ms
    0x3008, 0x42, // software power down, bit[6]
    0x3103, 0x03, // system clock from PLL, bit[1]
    0x3017, 0xff, // FREX, Vsync, HREF, PCLK, D[9:6] output enable
    0x3018, 0xff, // D[5:0], GPIO[1:0] output enable
    0x3034, 0x1a, // MIPI 10-bit
    0x3037, 0x13, // PLL root divider, bit[4], PLL pre-divider, bit[3:0]
    0x3108, 0x01, // PCLK root divider, bit[5:4], SCLK2x root divider, bit[3:2]
    // SCLK root divider, bit[1:0]
    0x3630, 0x36,
    0x3631, 0x0e,
    0x3632, 0xe2,
    0x3633, 0x12,
    0x3621, 0xe0,
    0x3704, 0xa0,
    0x3703, 0x5a,
    0x3715, 0x78,
    0x3717, 0x01,
    0x370b, 0x60,
    0x3705, 0x1a,
    0x3905, 0x02,
    0x3906, 0x10,
    0x3901, 0x0a,
    0x3731, 0x12,
    0x3600, 0x08, // VCM control
    0x3601, 0x33, // VCM control
    0x302d, 0x60, // system control
    0x3620, 0x52,
    0x371b, 0x20,
    0x471c, 0x50,
    0x3a13, 0x43, // pre-gain = 1.047x
    0x3a18, 0x00, // gain ceiling
    0x3a19, 0xf8, // gain ceiling = 15.5x
    0x3635, 0x13,
    0x3636, 0x03,
    0x3634, 0x40,
    0x3622, 0x01,
    // 50/60Hz detection 50/60Hz µÆ¹âÌõÎÆ¹ýÂË
    0x3c01, 0x34, // Band auto, bit[7]
    0x3c04, 0x28, // threshold low sum
    0x3c05, 0x98, // threshold high sum
    0x3c06, 0x00, // light meter 1 threshold[15:8]
    0x3c07, 0x08, // light meter 1 threshold[7:0]
    0x3c08, 0x00, // light meter 2 threshold[15:8]
    0x3c09, 0x1c, // light meter 2 threshold[7:0]
    0x3c0a, 0x9c, // sample number[15:8]
    0x3c0b, 0x40, // sample number[7:0]
    0x3810, 0x00, // Timing Hoffset[11:8]
    0x3811, 0x10, // Timing Hoffset[7:0]
    0x3812, 0x00, // Timing Voffset[10:8]
    0x3708, 0x64,
    0x4001, 0x02, // BLC start from line 2
    0x4005, 0x1a, // BLC always update
    0x3000, 0x00, // enable blocks
    0x3004, 0xff, // enable clocks
    0x300e, 0x58, // MIPI power down, DVP enable
    0x302e, 0x00,

    0x4300, 0x6f, // RGB565
    0x501f, 0x01, // RGB565

    0x440e, 0x00,
    0x5000, 0xa7, // Lenc on, raw gamma on, BPC on, WPC on, CIP on
    // AEC target 
    0x3a0f, 0x30, // stable range in high
    0x3a10, 0x28, // stable range in low
    0x3a1b, 0x30, // stable range out high
    0x3a1e, 0x26, // stable range out low
    0x3a11, 0x60, // fast zone high
    0x3a1f, 0x14, // fast zone low
    // Lens correction for 
    0x5800, 0x23,
    0x5801, 0x14,
    0x5802, 0x0f,
    0x5803, 0x0f,
    0x5804, 0x12,
    0x5805, 0x26,
    0x5806, 0x0c,
    0x5807, 0x08,
    0x5808, 0x05,
    0x5809, 0x05,
    0x580a, 0x08,
    0x580b, 0x0d,
    0x580c, 0x08,
    0x580d, 0x03,
    0x580e, 0x00,
    0x580f, 0x00,
    0x5810, 0x03,
    0x5811, 0x09,
    0x5812, 0x07,
    0x5813, 0x03,
    0x5814, 0x00,
    0x5815, 0x01,
    0x5816, 0x03,
    0x5817, 0x08,
    0x5818, 0x0d,
    0x5819, 0x08,
    0x581a, 0x05,
    0x581b, 0x06,
    0x581c, 0x08,
    0x581d, 0x0e,
    0x581e, 0x29,
    0x581f, 0x17,
    0x5820, 0x11,
    0x5821, 0x11,
    0x5822, 0x15,
    0x5823, 0x28,
    0x5824, 0x46,
    0x5825, 0x26,
    0x5826, 0x08,
    0x5827, 0x26,
    0x5828, 0x64,
    0x5829, 0x26,
    0x582a, 0x24,
    0x582b, 0x22,
    0x582c, 0x24,
    0x582d, 0x24,
    0x582e, 0x06,
    0x582f, 0x22,
    0x5830, 0x40,
    0x5831, 0x42,
    0x5832, 0x24,
    0x5833, 0x26,
    0x5834, 0x24,
    0x5835, 0x22,
    0x5836, 0x22,
    0x5837, 0x26,
    0x5838, 0x44,
    0x5839, 0x24,
    0x583a, 0x26,
    0x583b, 0x28,
    0x583c, 0x42,
    0x583d, 0xce, // lenc BR offset
    // AWB 
    0x5180, 0xff, // AWB B block
    0x5181, 0xf2, // AWB control
    0x5182, 0x00, // [7:4] max local counter, [3:0] max fast counter
    0x5183, 0x14, // AWB advanced
    0x5184, 0x25,
    0x5185, 0x24,
    0x5186, 0x09,
    0x5187, 0x09,
    0x5188, 0x09,
    0x5189, 0x75,
    0x518a, 0x54,
    0x518b, 0xe0,
    0x518c, 0xb2,
    0x518d, 0x42,
    0x518e, 0x3d,
    0x518f, 0x56,
    0x5190, 0x46,
    0x5191, 0xf8, // AWB top limit
    0x5192, 0x04, // AWB bottom limit
    0x5193, 0x70, // red limit
    0x5194, 0xf0, // green limit
    0x5195, 0xf0, // blue limit
    0x5196, 0x03, // AWB control
    0x5197, 0x01, // local limit
    0x5198, 0x04,
    0x5199, 0x12,
    0x519a, 0x04,
    0x519b, 0x00,
    0x519c, 0x06,
    0x519d, 0x82,
    0x519e, 0x38, // AWB control
    // Gamma 
    0x5480, 0x01, // Gamma bias plus on, bit[0]
    0x5481, 0x08,
    0x5482, 0x14,
    0x5483, 0x28,
    0x5484, 0x51,
    0x5485, 0x65,
    0x5486, 0x71,
    0x5487, 0x7d,
    0x5488, 0x87,
    0x5489, 0x91,
    0x548a, 0x9a,
    0x548b, 0xaa,
    0x548c, 0xb8,
    0x548d, 0xcd,
    0x548e, 0xdd,
    0x548f, 0xea,
    0x5490, 0x1d,
    // color matrix É«²Ê¾ØÕó
    0x5381, 0x1e, // CMX1 for Y
    0x5382, 0x5b, // CMX2 for Y
    0x5383, 0x08, // CMX3 for Y
    0x5384, 0x0a, // CMX4 for U
    0x5385, 0x7e, // CMX5 for U
    0x5386, 0x88, // CMX6 for U
    0x5387, 0x7c, // CMX7 for V
    0x5388, 0x6c, // CMX8 for V
    0x5389, 0x10, // CMX9 for V
    0x538a, 0x01, // sign[9]
    0x538b, 0x98, // sign[8:1]
    // UV adjust UV É«²Ê±¥ºÍ¶Èµ÷Õû
    0x5580, 0x06, // saturation on, bit[1]
    0x5583, 0x40,
    0x5584, 0x10,
    0x5589, 0x10,
    0x558a, 0x00,
    0x558b, 0xf8,
    0x501d, 0x40, // enable manual offset of contrast
    // CIP Èñ»¯ºÍ½µÔë
    0x5300, 0x08, // CIP sharpen MT threshold 1
    0x5301, 0x30, // CIP sharpen MT threshold 2
    0x5302, 0x10, // CIP sharpen MT offset 1
    0x5303, 0x00, // CIP sharpen MT offset 2
    0x5304, 0x08, // CIP DNS threshold 1
    0x5305, 0x30, // CIP DNS threshold 2
    0x5306, 0x08, // CIP DNS offset 1
    0x5307, 0x16, // CIP DNS offset 2
    0x5309, 0x08, // CIP sharpen TH threshold 1
    0x530a, 0x30, // CIP sharpen TH threshold 2
    0x530b, 0x04, // CIP sharpen TH offset 1
    0x530c, 0x06, // CIP sharpen TH offset 2
    0x5025, 0x00,
    0x3008, 0x02, // wake up from standby, bit[6]
   
//    0x503d, 0x80,//²âÊÔ²ÊÌõ
//    0x4741, 0x00,


    // 30fps, night mode 5fps
    // input clock 24Mhz, PCLK 45.6Mhz
		//30fps
    0x3035, 0x21, // PLL
    0x3036, 0x72, // PLL
	
		//15fps
//	  0x3035, 0x41, // PLL
//    0x3036, 0x72, // PLL
	
    0x3c07, 0x08, // light meter 1 threshold[7:0]
//    0x3820, 0x42, // flip		ÉÏÏÂ¾µÏñ
//    0x3821, 0x07, // mirror	×óÓÒ¾µÏñ
    0x3814, 0x31, // timing X inc
    0x3815, 0x31, // timing Y inc
    0x3800, 0x00, // HS
    0x3801, 0x00, // HS
    0x3802, 0x00, // VS
    0x3803, 0xbe, // VS
    0x3804, 0x0a, // HW (HE)
    0x3805, 0x3f, // HW (HE)
    0x3806, 0x06, // VH (VE)
    0x3807, 0xe4, // VH (VE)
    0x3808, 0x03, // DVPHO
    0x3809, 0x56, // DVPHO
    0x380a, 0x01, // DVPVO
    0x380b, 0xe0, // DVPVO
    0x380c, 0x07, // HTS
    0x380d, 0x69, // HTS
    0x380e, 0x03, // VTS
    0x380f, 0x21, // VTS
    0x3813, 0x06, // timing V offset
    0x3618, 0x00,
    0x3612, 0x29,
    0x3709, 0x52,
    0x370c, 0x03,
    0x3a02, 0x09, // 60Hz max exposure, night mode 5fps
    0x3a03, 0x63, // 60Hz max exposure
    // banding filters are calculated automatically in camera driver
    //0x3a08, 0x00, // B50 step
    //0x3a09, 0x78, // B50 step
    //0x3a0a, 0x00, // B60 step
    //0x3a0b, 0x64, // B60 step
    //0x3a0e, 0x06, // 50Hz max band
    //0x3a0d, 0x08, // 60Hz max band
    0x3a14, 0x09, // 50Hz max exposure, night mode 5fps
    0x3a15, 0x63, // 50Hz max exposure
    0x4004, 0x02, // BLC line number
    0x3002, 0x1c, // reset JFIFO, SFIFO, JPG
    0x3006, 0xc3, // disable clock of JPEG2x, JPEG
    0x4713, 0x03, // JPEG mode 3
    0x4407, 0x04, // Quantization sacle
    0x460b, 0x35,
    0x460c, 0x22,
    0x4837, 0x22, // MIPI global timing
    0x3824, 0x02, // PCLK manual divider
    0x5001, 0xa3, // SDE on, CMX on, AWB on
    0x3503, 0x00, // AEC/AGC on//	  /* Initialize OV5640 */
    
    
//    0x3016, 0x01,
//    0x301C, 0x00,
//    0x3016, 0x01,
//    0x301C, 0x01,
//    0x3019, 0x01,

//    0x503d, 0x80,//²âÊÔ²ÊÌõ
//    0x4741, 0x00,
	  0x4740, 0x23,
};

const size_t OV5640_NumOfInitRegs = (size_t)((sizeof(OV5640_initRegs) / sizeof(OV5640_initRegs[0])));

const uint8_t OV5640_AF_FW[] =  {
    0x02, 0x0d, 0xf3, 0x02, 0x0a, 0x5f, 0xc2, 0x01, 0x22, 0x22, 0x00, 0x02, 0x0f, 0x31, 0x30, 0x01,
    0x03, 0x02, 0x03, 0x09, 0x30, 0x02, 0x03, 0x02, 0x03, 0x09, 0x90, 0x51, 0xa5, 0xe0, 0x78, 0xbb, 
    0xf6, 0xa3, 0xe0, 0x08, 0xf6, 0xa3, 0xe0, 0x08, 0xf6, 0xe5, 0x1f, 0x70, 0x45, 0x75, 0x1e, 0x20, 
    0xd2, 0x34, 0x12, 0x0c, 0x0c, 0x78, 0x9c, 0x12, 0x0b, 0xd2, 0x78, 0xa8, 0xa6, 0x14, 0x08, 0xa6, 
    0x15, 0x78, 0xb3, 0xa6, 0x09, 0x18, 0x76, 0x01, 0x78, 0x4c, 0xa6, 0x0a, 0x08, 0xa6, 0x0b, 0x78, 
    0x6c, 0xa6, 0x14, 0x08, 0xa6, 0x15, 0x78, 0xb3, 0xe6, 0x78, 0x8c, 0xf6, 0x75, 0x1f, 0x01, 0x78, 
    0xbb, 0xe6, 0x78, 0xb8, 0xf6, 0x78, 0xbc, 0xe6, 0x78, 0xb9, 0xf6, 0x78, 0xbd, 0xe6, 0x78, 0xba, 
    0xf6, 0x22, 0x79, 0xb8, 0xe7, 0xd3, 0x78, 0xbb, 0x96, 0x40, 0x05, 0xe7, 0x96, 0xff, 0x80, 0x08, 
    0xc3, 0x79, 0xbb, 0xe7, 0x78, 0xb8, 0x96, 0xff, 0x78, 0xa6, 0x76, 0x00, 0x08, 0xa6, 0x07, 0x79, 
    0xb9, 0xe7, 0xd3, 0x78, 0xbc, 0x96, 0x40, 0x05, 0xe7, 0x96, 0xff, 0x80, 0x08, 0xc3, 0x79, 0xbc, 
    0xe7, 0x78, 0xb9, 0x96, 0xff, 0x12, 0x0c, 0x13, 0x79, 0xba, 0xe7, 0xd3, 0x78, 0xbd, 0x96, 0x40, 
    0x05, 0xe7, 0x96, 0xff, 0x80, 0x08, 0xc3, 0x79, 0xbd, 0xe7, 0x78, 0xba, 0x96, 0xff, 0x12, 0x0c, 
    0x13, 0x78, 0xb2, 0xe6, 0x25, 0xe0, 0x24, 0x4c, 0xf8, 0xa6, 0x0a, 0x08, 0xa6, 0x0b, 0x78, 0xb2, 
    0xe6, 0x25, 0xe0, 0x24, 0x6c, 0xf8, 0xa6, 0x14, 0x08, 0xa6, 0x15, 0x78, 0xb2, 0xe6, 0x24, 0x8c, 
    0xf8, 0xa6, 0x09, 0x78, 0xb2, 0xe6, 0x24, 0x01, 0xff, 0xe4, 0x33, 0xfe, 0xd3, 0xef, 0x94, 0x0f, 
    0xee, 0x64, 0x80, 0x94, 0x80, 0x40, 0x04, 0x7f, 0x00, 0x80, 0x05, 0x78, 0xb2, 0xe6, 0x04, 0xff, 
    0x78, 0xb2, 0xa6, 0x07, 0xe5, 0x1f, 0xb4, 0x01, 0x0a, 0xe6, 0x60, 0x03, 0x02, 0x03, 0x09, 0x75, 
    0x1f, 0x02, 0x22, 0x12, 0x0c, 0x0c, 0x78, 0x9e, 0x12, 0x0b, 0xd2, 0x12, 0x0c, 0x0c, 0x78, 0xa0, 
    0x12, 0x0b, 0xff, 0x78, 0xaa, 0x12, 0x0b, 0xff, 0xff, 0x78, 0xac, 0xa6, 0x06, 0x08, 0xa6, 0x07, 
    0x78, 0x8c, 0xe6, 0x78, 0xb4, 0xf6, 0x78, 0x8c, 0xe6, 0x78, 0xb5, 0xf6, 0x7f, 0x01, 0xef, 0x25, 
    0xe0, 0x24, 0x4d, 0x78, 0x9f, 0x12, 0x0b, 0xc9, 0x50, 0x0a, 0x12, 0x0b, 0xab, 0x78, 0x9e, 0xa6, 
    0x04, 0x08, 0xa6, 0x05, 0xef, 0x25, 0xe0, 0x24, 0x6d, 0x78, 0xab, 0x12, 0x0b, 0xc9, 0x50, 0x0f, 
    0xef, 0x25, 0xe0, 0x24, 0x6c, 0x12, 0x0b, 0xb0, 0x78, 0xaa, 0xa6, 0x04, 0x08, 0xa6, 0x05, 0x74, 
    0x8c, 0x2f, 0xf9, 0x78, 0xb4, 0xe6, 0xc3, 0x97, 0x50, 0x08, 0x74, 0x8c, 0x2f, 0xf8, 0xe6, 0x78, 
    0xb4, 0xf6, 0xef, 0x25, 0xe0, 0x24, 0x4d, 0xf9, 0xd3, 0x78, 0xa1, 0x12, 0x0b, 0xcb, 0x40, 0x0a, 
    0x12, 0x0b, 0xab, 0x78, 0xa0, 0xa6, 0x04, 0x08, 0xa6, 0x05, 0xef, 0x25, 0xe0, 0x24, 0x6d, 0xf9, 
    0xd3, 0x78, 0xad, 0x12, 0x0b, 0xcb, 0x40, 0x0f, 0xef, 0x25, 0xe0, 0x24, 0x6c, 0x12, 0x0b, 0xb0, 
    0x78, 0xac, 0xa6, 0x04, 0x08, 0xa6, 0x05, 0x74, 0x8c, 0x2f, 0xf9, 0x78, 0xb5, 0xe6, 0xd3, 0x97, 
    0x40, 0x08, 0x74, 0x8c, 0x2f, 0xf8, 0xe6, 0x78, 0xb5, 0xf6, 0x0f, 0xef, 0x64, 0x10, 0x60, 0x03, 
    0x02, 0x01, 0x3e, 0xc3, 0x79, 0x9f, 0x78, 0xa1, 0x12, 0x0b, 0xf7, 0x78, 0xa2, 0xf6, 0x08, 0xa6, 
    0x07, 0xc3, 0x79, 0xab, 0x78, 0xad, 0x12, 0x0b, 0xf7, 0x78, 0xae, 0xf6, 0x08, 0xa6, 0x07, 0xc3, 
    0x79, 0xb4, 0xe7, 0x78, 0xb5, 0x96, 0x08, 0xf6, 0xd3, 0x79, 0x9f, 0xe7, 0x78, 0x9d, 0x96, 0x19, 
    0xe7, 0x18, 0x96, 0x40, 0x05, 0x09, 0xe7, 0x08, 0x80, 0x06, 0xc3, 0x79, 0x9d, 0xe7, 0x78, 0x9f, 
    0x12, 0x0b, 0xf8, 0xfe, 0x78, 0xa4, 0xa6, 0x06, 0x08, 0xa6, 0x07, 0xd3, 0x79, 0xab, 0xe7, 0x78, 
    0xa9, 0x96, 0x19, 0xe7, 0x18, 0x96, 0x40, 0x05, 0x09, 0xe7, 0x08, 0x80, 0x06, 0xc3, 0x79, 0xa9, 
    0xe7, 0x78, 0xab, 0x12, 0x0b, 0xf8, 0xfe, 0x78, 0xb0, 0xa6, 0x06, 0x08, 0xa6, 0x07, 0x79, 0xb4, 
    0xe7, 0xd3, 0x78, 0xb3, 0x96, 0x40, 0x05, 0xe7, 0x96, 0xff, 0x80, 0x08, 0xc3, 0x79, 0xb3, 0xe7, 
    0x78, 0xb4, 0x96, 0xff, 0x78, 0xb7, 0xa6, 0x07, 0xe5, 0x1f, 0x64, 0x02, 0x60, 0x03, 0x02, 0x02, 
    0xef, 0x90, 0x30, 0x24, 0x74, 0x0f, 0xf0, 0x90, 0x0e, 0x8a, 0xe4, 0x93, 0xff, 0x18, 0xe6, 0xc3, 
    0x9f, 0x40, 0x03, 0x02, 0x03, 0x09, 0x90, 0x30, 0x24, 0x74, 0x0e, 0xf0, 0x78, 0xa2, 0x12, 0x0b, 
    0xd9, 0x12, 0x0b, 0xa2, 0x90, 0x0e, 0x87, 0x12, 0x0b, 0xb7, 0x78, 0x9e, 0x12, 0x0b, 0xe8, 0x7b, 
    0x04, 0x12, 0x0b, 0x90, 0xc3, 0x12, 0x07, 0x0e, 0x50, 0x6f, 0x90, 0x0e, 0x8b, 0xe4, 0x93, 0xff, 
    0x78, 0xb7, 0xe6, 0x9f, 0x40, 0x07, 0x90, 0x30, 0x24, 0x74, 0x0a, 0x80, 0x16, 0x90, 0x0e, 0x89, 
    0xe4, 0x93, 0xff, 0xd3, 0x78, 0xa7, 0xe6, 0x9f, 0x18, 0xe6, 0x94, 0x00, 0x40, 0x09, 0x90, 0x30, 
    0x24, 0x74, 0x0b, 0xf0, 0x75, 0x1f, 0x05, 0x78, 0xae, 0x12, 0x0b, 0xd9, 0x12, 0x0b, 0xa2, 0x90, 
    0x0e, 0x88, 0x12, 0x0b, 0xb7, 0x78, 0xa8, 0x12, 0x0b, 0xe8, 0x7b, 0x40, 0x12, 0x0b, 0x90, 0xd3, 
    0x12, 0x07, 0x0e, 0x40, 0x24, 0x90, 0x30, 0x24, 0x74, 0x0c, 0xf0, 0x75, 0x1f, 0x05, 0x22, 0x90, 
    0x30, 0x24, 0x74, 0x01, 0xf0, 0xe5, 0x1f, 0xb4, 0x05, 0x0f, 0xd2, 0x01, 0xc2, 0x02, 0xe4, 0xf5, 
    0x1f, 0xf5, 0x1e, 0xd2, 0x34, 0xd2, 0x32, 0xd2, 0x35, 0x22, 0xe5, 0x1f, 0x60, 0x03, 0x02, 0x03, 
    0x93, 0xf5, 0x1e, 0xd2, 0x34, 0x75, 0x34, 0xff, 0x75, 0x35, 0x0e, 0x75, 0x36, 0x55, 0x75, 0x37, 
    0x01, 0x12, 0x0d, 0x85, 0xe4, 0xff, 0xef, 0x25, 0xe0, 0x24, 0x4c, 0xf8, 0xe4, 0xf6, 0x08, 0xf6, 
    0x0f, 0xbf, 0x34, 0xf2, 0x90, 0x0e, 0x8c, 0xe4, 0x93, 0xff, 0xe5, 0x49, 0xc3, 0x9f, 0x50, 0x04, 
    0x7f, 0x05, 0x80, 0x02, 0x7f, 0xfb, 0x78, 0xbb, 0xa6, 0x07, 0x12, 0x0e, 0xbc, 0x40, 0x04, 0x7f, 
    0x03, 0x80, 0x02, 0x7f, 0x30, 0x78, 0xba, 0xa6, 0x07, 0xe6, 0x18, 0xf6, 0x08, 0xe6, 0x78, 0xb7, 
    0xf6, 0x78, 0xba, 0xe6, 0x78, 0xb8, 0xf6, 0x78, 0xbd, 0x76, 0x33, 0xe4, 0x08, 0xf6, 0x78, 0xb6, 
    0x76, 0x01, 0x75, 0x48, 0x02, 0x78, 0xb4, 0xf6, 0x08, 0xf6, 0x74, 0xff, 0x78, 0xbf, 0xf6, 0x08, 
    0xf6, 0x75, 0x1f, 0x01, 0x78, 0xba, 0xe6, 0x75, 0xf0, 0x05, 0xa4, 0xf5, 0x49, 0x12, 0x0a, 0xfd, 
    0xc2, 0x36, 0x22, 0x78, 0xb6, 0xe6, 0xd3, 0x94, 0x00, 0x40, 0x02, 0x16, 0x22, 0xe5, 0x1f, 0xb4, 
    0x05, 0x23, 0xe4, 0xf5, 0x1f, 0xc2, 0x01, 0x78, 0xb4, 0xe6, 0xfe, 0x08, 0xe6, 0xff, 0x78, 0x4c, 
    0xa6, 0x06, 0x08, 0xa6, 0x07, 0xa2, 0x36, 0xe4, 0x33, 0xf5, 0x3c, 0x90, 0x30, 0x28, 0xf0, 0x75, 
    0x1e, 0x10, 0xd2, 0x34, 0x22, 0xe5, 0x49, 0x75, 0xf0, 0x05, 0x84, 0x78, 0xba, 0xf6, 0x90, 0x0e, 
    0x85, 0xe4, 0x93, 0xff, 0x25, 0xe0, 0x24, 0x0a, 0xf8, 0xe6, 0xfc, 0x08, 0xe6, 0xfd, 0x78, 0xba, 
    0xe6, 0x25, 0xe0, 0x24, 0x4c, 0xf8, 0xa6, 0x04, 0x08, 0xa6, 0x05, 0xef, 0x12, 0x0e, 0xc3, 0xd3, 
    0x78, 0xb5, 0x96, 0xee, 0x18, 0x96, 0x40, 0x0d, 0x78, 0xba, 0xe6, 0x78, 0xb7, 0xf6, 0x78, 0xb4, 
    0xa6, 0x06, 0x08, 0xa6, 0x07, 0x90, 0x0e, 0x85, 0xe4, 0x93, 0x12, 0x0e, 0xc3, 0xc3, 0x78, 0xc0, 
    0x96, 0xee, 0x18, 0x96, 0x50, 0x0d, 0x78, 0xba, 0xe6, 0x78, 0xb8, 0xf6, 0x78, 0xbf, 0xa6, 0x06, 
    0x08, 0xa6, 0x07, 0x78, 0xb4, 0xe6, 0xfe, 0x08, 0xe6, 0xc3, 0x78, 0xc0, 0x96, 0xff, 0xee, 0x18, 
    0x96, 0x78, 0xc1, 0xf6, 0x08, 0xa6, 0x07, 0x90, 0x0e, 0x8e, 0xe4, 0x18, 0x12, 0x0e, 0xa1, 0x40, 
    0x02, 0xd2, 0x36, 0x78, 0xba, 0xe6, 0x08, 0x26, 0x08, 0xf6, 0xe5, 0x1f, 0x64, 0x01, 0x70, 0x4a, 
    0xe6, 0xc3, 0x78, 0xbe, 0x12, 0x0e, 0x97, 0x40, 0x05, 0x12, 0x0e, 0x92, 0x40, 0x39, 0x12, 0x0e, 
    0xba, 0x40, 0x04, 0x7f, 0xfe, 0x80, 0x02, 0x7f, 0x02, 0x78, 0xbb, 0xa6, 0x07, 0x78, 0xb7, 0xe6, 
    0x24, 0x03, 0x78, 0xbd, 0xf6, 0x78, 0xb7, 0xe6, 0x24, 0xfd, 0x78, 0xbe, 0xf6, 0x12, 0x0e, 0xba, 
    0x40, 0x06, 0x78, 0xbe, 0xe6, 0xff, 0x80, 0x04, 0x78, 0xbd, 0xe6, 0xff, 0x78, 0xbc, 0xa6, 0x07, 
    0x75, 0x1f, 0x02, 0x78, 0xb6, 0x76, 0x01, 0x02, 0x05, 0x59, 0xe5, 0x1f, 0x64, 0x02, 0x60, 0x03, 
    0x02, 0x05, 0x39, 0x78, 0xbc, 0xe6, 0xff, 0xc3, 0x78, 0xbe, 0x12, 0x0e, 0x98, 0x40, 0x08, 0x12, 
    0x0e, 0x92, 0x50, 0x03, 0x02, 0x05, 0x37, 0x12, 0x0e, 0xba, 0x40, 0x04, 0x7f, 0xff, 0x80, 0x02, 
    0x7f, 0x01, 0x78, 0xbb, 0xa6, 0x07, 0x78, 0xb7, 0xe6, 0x04, 0x78, 0xbd, 0xf6, 0x78, 0xb7, 0xe6, 
    0x14, 0x78, 0xbe, 0xf6, 0x18, 0x12, 0x0e, 0xbc, 0x40, 0x04, 0xe6, 0xff, 0x80, 0x02, 0x7f, 0x00, 
    0x78, 0xbd, 0xa6, 0x07, 0xd3, 0x08, 0xe6, 0x64, 0x80, 0x94, 0x80, 0x40, 0x04, 0xe6, 0xff, 0x80, 
    0x02, 0x7f, 0x00, 0x78, 0xbe, 0xa6, 0x07, 0xc3, 0x18, 0xe6, 0x64, 0x80, 0x94, 0xb3, 0x50, 0x04, 
    0xe6, 0xff, 0x80, 0x02, 0x7f, 0x33, 0x78, 0xbd, 0xa6, 0x07, 0xc3, 0x08, 0xe6, 0x64, 0x80, 0x94, 
    0xb3, 0x50, 0x04, 0xe6, 0xff, 0x80, 0x02, 0x7f, 0x33, 0x78, 0xbe, 0xa6, 0x07, 0x12, 0x0e, 0xba, 
    0x40, 0x06, 0x78, 0xbe, 0xe6, 0xff, 0x80, 0x04, 0x78, 0xbd, 0xe6, 0xff, 0x78, 0xbc, 0xa6, 0x07, 
    0x75, 0x1f, 0x03, 0x78, 0xb6, 0x76, 0x01, 0x80, 0x20, 0xe5, 0x1f, 0x64, 0x03, 0x70, 0x26, 0x78, 
    0xbc, 0xe6, 0xff, 0xc3, 0x78, 0xbe, 0x12, 0x0e, 0x98, 0x40, 0x05, 0x12, 0x0e, 0x92, 0x40, 0x09, 
    0x78, 0xb7, 0xe6, 0x78, 0xbc, 0xf6, 0x75, 0x1f, 0x04, 0x78, 0xbc, 0xe6, 0x75, 0xf0, 0x05, 0xa4, 
    0xf5, 0x49, 0x02, 0x0a, 0xfd, 0xe5, 0x1f, 0xb4, 0x04, 0x1f, 0x90, 0x0e, 0x8d, 0xe4, 0x78, 0xc1, 
    0x12, 0x0e, 0xa1, 0x40, 0x02, 0xd2, 0x36, 0x75, 0x1f, 0x05, 0x75, 0x34, 0xff, 0x75, 0x35, 0x0e, 
    0x75, 0x36, 0x59, 0x75, 0x37, 0x01, 0x12, 0x0d, 0x85, 0x22, 0xef, 0x8d, 0xf0, 0xa4, 0xa8, 0xf0, 
    0xcf, 0x8c, 0xf0, 0xa4, 0x28, 0xce, 0x8d, 0xf0, 0xa4, 0x2e, 0xfe, 0x22, 0xbc, 0x00, 0x0b, 0xbe, 
    0x00, 0x29, 0xef, 0x8d, 0xf0, 0x84, 0xff, 0xad, 0xf0, 0x22, 0xe4, 0xcc, 0xf8, 0x75, 0xf0, 0x08, 
    0xef, 0x2f, 0xff, 0xee, 0x33, 0xfe, 0xec, 0x33, 0xfc, 0xee, 0x9d, 0xec, 0x98, 0x40, 0x05, 0xfc, 
    0xee, 0x9d, 0xfe, 0x0f, 0xd5, 0xf0, 0xe9, 0xe4, 0xce, 0xfd, 0x22, 0xed, 0xf8, 0xf5, 0xf0, 0xee, 
    0x84, 0x20, 0xd2, 0x1c, 0xfe, 0xad, 0xf0, 0x75, 0xf0, 0x08, 0xef, 0x2f, 0xff, 0xed, 0x33, 0xfd, 
    0x40, 0x07, 0x98, 0x50, 0x06, 0xd5, 0xf0, 0xf2, 0x22, 0xc3, 0x98, 0xfd, 0x0f, 0xd5, 0xf0, 0xea, 
    0x22, 0xe8, 0x8f, 0xf0, 0xa4, 0xcc, 0x8b, 0xf0, 0xa4, 0x2c, 0xfc, 0xe9, 0x8e, 0xf0, 0xa4, 0x2c, 
    0xfc, 0x8a, 0xf0, 0xed, 0xa4, 0x2c, 0xfc, 0xea, 0x8e, 0xf0, 0xa4, 0xcd, 0xa8, 0xf0, 0x8b, 0xf0, 
    0xa4, 0x2d, 0xcc, 0x38, 0x25, 0xf0, 0xfd, 0xe9, 0x8f, 0xf0, 0xa4, 0x2c, 0xcd, 0x35, 0xf0, 0xfc, 
    0xeb, 0x8e, 0xf0, 0xa4, 0xfe, 0xa9, 0xf0, 0xeb, 0x8f, 0xf0, 0xa4, 0xcf, 0xc5, 0xf0, 0x2e, 0xcd, 
    0x39, 0xfe, 0xe4, 0x3c, 0xfc, 0xea, 0xa4, 0x2d, 0xce, 0x35, 0xf0, 0xfd, 0xe4, 0x3c, 0xfc, 0x22, 
    0x75, 0xf0, 0x08, 0x75, 0x82, 0x00, 0xef, 0x2f, 0xff, 0xee, 0x33, 0xfe, 0xcd, 0x33, 0xcd, 0xcc, 
    0x33, 0xcc, 0xc5, 0x82, 0x33, 0xc5, 0x82, 0x9b, 0xed, 0x9a, 0xec, 0x99, 0xe5, 0x82, 0x98, 0x40, 
    0x0c, 0xf5, 0x82, 0xee, 0x9b, 0xfe, 0xed, 0x9a, 0xfd, 0xec, 0x99, 0xfc, 0x0f, 0xd5, 0xf0, 0xd6, 
    0xe4, 0xce, 0xfb, 0xe4, 0xcd, 0xfa, 0xe4, 0xcc, 0xf9, 0xa8, 0x82, 0x22, 0xb8, 0x00, 0xc1, 0xb9, 
    0x00, 0x59, 0xba, 0x00, 0x2d, 0xec, 0x8b, 0xf0, 0x84, 0xcf, 0xce, 0xcd, 0xfc, 0xe5, 0xf0, 0xcb, 
    0xf9, 0x78, 0x18, 0xef, 0x2f, 0xff, 0xee, 0x33, 0xfe, 0xed, 0x33, 0xfd, 0xec, 0x33, 0xfc, 0xeb, 
    0x33, 0xfb, 0x10, 0xd7, 0x03, 0x99, 0x40, 0x04, 0xeb, 0x99, 0xfb, 0x0f, 0xd8, 0xe5, 0xe4, 0xf9, 
    0xfa, 0x22, 0x78, 0x18, 0xef, 0x2f, 0xff, 0xee, 0x33, 0xfe, 0xed, 0x33, 0xfd, 0xec, 0x33, 0xfc, 
    0xc9, 0x33, 0xc9, 0x10, 0xd7, 0x05, 0x9b, 0xe9, 0x9a, 0x40, 0x07, 0xec, 0x9b, 0xfc, 0xe9, 0x9a, 
    0xf9, 0x0f, 0xd8, 0xe0, 0xe4, 0xc9, 0xfa, 0xe4, 0xcc, 0xfb, 0x22, 0x75, 0xf0, 0x10, 0xef, 0x2f, 
    0xff, 0xee, 0x33, 0xfe, 0xed, 0x33, 0xfd, 0xcc, 0x33, 0xcc, 0xc8, 0x33, 0xc8, 0x10, 0xd7, 0x07, 
    0x9b, 0xec, 0x9a, 0xe8, 0x99, 0x40, 0x0a, 0xed, 0x9b, 0xfd, 0xec, 0x9a, 0xfc, 0xe8, 0x99, 0xf8, 
    0x0f, 0xd5, 0xf0, 0xda, 0xe4, 0xcd, 0xfb, 0xe4, 0xcc, 0xfa, 0xe4, 0xc8, 0xf9, 0x22, 0xeb, 0x9f, 
    0xf5, 0xf0, 0xea, 0x9e, 0x42, 0xf0, 0xe9, 0x9d, 0x42, 0xf0, 0xe8, 0x9c, 0x45, 0xf0, 0x22, 0xe8, 
    0x60, 0x0f, 0xef, 0xc3, 0x33, 0xff, 0xee, 0x33, 0xfe, 0xed, 0x33, 0xfd, 0xec, 0x33, 0xfc, 0xd8, 
    0xf1, 0x22, 0xe4, 0x93, 0xfc, 0x74, 0x01, 0x93, 0xfd, 0x74, 0x02, 0x93, 0xfe, 0x74, 0x03, 0x93, 
    0xff, 0x22, 0xe6, 0xfb, 0x08, 0xe6, 0xf9, 0x08, 0xe6, 0xfa, 0x08, 0xe6, 0xcb, 0xf8, 0x22, 0xec, 
    0xf6, 0x08, 0xed, 0xf6, 0x08, 0xee, 0xf6, 0x08, 0xef, 0xf6, 0x22, 0xa4, 0x25, 0x82, 0xf5, 0x82, 
    0xe5, 0xf0, 0x35, 0x83, 0xf5, 0x83, 0x22, 0xd0, 0x83, 0xd0, 0x82, 0xf8, 0xe4, 0x93, 0x70, 0x12, 
    0x74, 0x01, 0x93, 0x70, 0x0d, 0xa3, 0xa3, 0x93, 0xf8, 0x74, 0x01, 0x93, 0xf5, 0x82, 0x88, 0x83, 
    0xe4, 0x73, 0x74, 0x02, 0x93, 0x68, 0x60, 0xef, 0xa3, 0xa3, 0xa3, 0x80, 0xdf, 0x90, 0x38, 0x04, 
    0x78, 0x50, 0x12, 0x0c, 0x7f, 0x90, 0x38, 0x00, 0xe0, 0xfe, 0xa3, 0xe0, 0xfd, 0xed, 0xff, 0xc3, 
    0x12, 0x0c, 0x38, 0x90, 0x38, 0x10, 0x12, 0x0c, 0x2c, 0x90, 0x38, 0x06, 0x78, 0x52, 0x12, 0x0c, 
    0x7f, 0x90, 0x38, 0x02, 0xe0, 0xfe, 0xa3, 0xe0, 0xfd, 0xed, 0xff, 0xc3, 0x12, 0x0c, 0x38, 0x90, 
    0x38, 0x12, 0x12, 0x0c, 0x2c, 0xa3, 0xe0, 0xb4, 0x31, 0x07, 0x78, 0x50, 0x79, 0x50, 0x12, 0x0c, 
    0x95, 0x90, 0x38, 0x14, 0xe0, 0xb4, 0x71, 0x15, 0x78, 0x50, 0xe6, 0xfe, 0x08, 0xe6, 0x78, 0x02, 
    0xce, 0xc3, 0x13, 0xce, 0x13, 0xd8, 0xf9, 0x79, 0x51, 0xf7, 0xee, 0x19, 0xf7, 0x90, 0x38, 0x15, 
    0xe0, 0xb4, 0x31, 0x07, 0x78, 0x52, 0x79, 0x52, 0x12, 0x0c, 0x95, 0x90, 0x38, 0x15, 0xe0, 0xb4, 
    0x71, 0x15, 0x78, 0x52, 0xe6, 0xfe, 0x08, 0xe6, 0x78, 0x02, 0xce, 0xc3, 0x13, 0xce, 0x13, 0xd8, 
    0xf9, 0x79, 0x53, 0xf7, 0xee, 0x19, 0xf7, 0x79, 0x50, 0x12, 0x0c, 0x67, 0x09, 0x12, 0x0c, 0x67, 
    0xaf, 0x45, 0x12, 0x0c, 0x1d, 0x7d, 0x50, 0x12, 0x05, 0x9c, 0x78, 0x58, 0xa6, 0x06, 0x08, 0xa6, 
    0x07, 0xaf, 0x43, 0x12, 0x0c, 0x1d, 0x7d, 0x50, 0x12, 0x05, 0x9c, 0x78, 0x54, 0xa6, 0x06, 0x08, 
    0xa6, 0x07, 0xaf, 0x46, 0x78, 0x52, 0x12, 0x0c, 0x1f, 0x7d, 0x3c, 0x12, 0x05, 0x9c, 0x78, 0x5a, 
    0xa6, 0x06, 0x08, 0xa6, 0x07, 0xaf, 0x44, 0x7e, 0x00, 0x78, 0x52, 0x12, 0x0c, 0x21, 0x7d, 0x3c, 
    0x12, 0x05, 0x9c, 0x78, 0x56, 0xa6, 0x06, 0x08, 0xa6, 0x07, 0xc3, 0x78, 0x59, 0xe6, 0x94, 0x08, 
    0x18, 0xe6, 0x94, 0x00, 0x50, 0x05, 0x76, 0x00, 0x08, 0x76, 0x08, 0xc3, 0x78, 0x5b, 0xe6, 0x94, 
    0x08, 0x18, 0xe6, 0x94, 0x00, 0x50, 0x05, 0x76, 0x00, 0x08, 0x76, 0x08, 0x78, 0x58, 0x12, 0x0c, 
    0x54, 0xff, 0xd3, 0x78, 0x55, 0xe6, 0x9f, 0x18, 0xe6, 0x9e, 0x40, 0x0e, 0x78, 0x58, 0xe6, 0x13, 
    0xfe, 0x08, 0xe6, 0x78, 0x55, 0x12, 0x0c, 0x8a, 0x80, 0x04, 0x7e, 0x00, 0x7f, 0x00, 0x78, 0x5c, 
    0x12, 0x0c, 0x4c, 0xff, 0xd3, 0x78, 0x57, 0xe6, 0x9f, 0x18, 0xe6, 0x9e, 0x40, 0x0e, 0x78, 0x5a, 
    0xe6, 0x13, 0xfe, 0x08, 0xe6, 0x78, 0x57, 0x12, 0x0c, 0x8a, 0x80, 0x04, 0x7e, 0x00, 0x7f, 0x00, 
    0xe4, 0xfc, 0xfd, 0x78, 0x60, 0x12, 0x07, 0x4f, 0x78, 0x58, 0x12, 0x0c, 0x54, 0x78, 0x55, 0x26, 
    0xff, 0xee, 0x18, 0x36, 0xfe, 0x78, 0x64, 0x12, 0x0c, 0x4c, 0x78, 0x57, 0x26, 0xff, 0xee, 0x18, 
    0x36, 0xfe, 0xe4, 0xfc, 0xfd, 0x78, 0x68, 0x12, 0x07, 0x4f, 0x12, 0x0c, 0x5c, 0x78, 0x64, 0x12, 
    0x07, 0x42, 0xd3, 0x12, 0x07, 0x0e, 0x40, 0x08, 0x12, 0x0c, 0x5c, 0x78, 0x64, 0x12, 0x07, 0x4f, 
    0x78, 0x52, 0x12, 0x0c, 0x5e, 0x78, 0x68, 0x12, 0x07, 0x42, 0xd3, 0x12, 0x07, 0x0e, 0x40, 0x0a, 
    0x78, 0x52, 0x12, 0x0c, 0x5e, 0x78, 0x68, 0x12, 0x07, 0x4f, 0xe4, 0xfd, 0x78, 0x5f, 0x12, 0x0c, 
    0x77, 0x24, 0x01, 0x12, 0x0c, 0x40, 0x78, 0x63, 0x12, 0x0c, 0x77, 0x24, 0x02, 0x12, 0x0c, 0x40, 
    0x78, 0x67, 0x12, 0x0c, 0x77, 0x24, 0x03, 0x12, 0x0c, 0x40, 0x78, 0x6b, 0x12, 0x0c, 0x77, 0x24, 
    0x04, 0x12, 0x0c, 0x40, 0x0d, 0xbd, 0x05, 0xd4, 0xc2, 0x0e, 0xc2, 0x06, 0x22, 0x85, 0x08, 0x41, 
    0x90, 0x30, 0x24, 0xe0, 0xf5, 0x3d, 0xa3, 0xe0, 0xf5, 0x3e, 0xa3, 0xe0, 0xf5, 0x3f, 0xa3, 0xe0, 
    0xf5, 0x40, 0xa3, 0xe0, 0xf5, 0x3c, 0xd2, 0x33, 0xe5, 0x41, 0x12, 0x07, 0x67, 0x09, 0xb4, 0x03, 
    0x09, 0xb8, 0x04, 0x09, 0xbe, 0x05, 0x09, 0xc1, 0x06, 0x09, 0xc4, 0x07, 0x09, 0xcd, 0x08, 0x09, 
    0xde, 0x12, 0x09, 0xe0, 0x80, 0x09, 0xe5, 0x81, 0x0a, 0x43, 0x8f, 0x0a, 0x32, 0x90, 0x0a, 0x43, 
    0x91, 0x0a, 0x43, 0x92, 0x0a, 0x43, 0x93, 0x0a, 0x43, 0x94, 0x0a, 0x43, 0x98, 0x0a, 0x40, 0x9f, 
    0x00, 0x00, 0x0a, 0x5e, 0x12, 0x0e, 0xce, 0x22, 0x12, 0x0e, 0xce, 0xd2, 0x03, 0x22, 0xd2, 0x03, 
    0x22, 0xc2, 0x03, 0x22, 0xa2, 0x36, 0xe4, 0x33, 0xf5, 0x3c, 0x02, 0x0a, 0x43, 0xc2, 0x01, 0xc2, 
    0x02, 0xc2, 0x03, 0x12, 0x0d, 0x14, 0x75, 0x1e, 0x70, 0xd2, 0x34, 0x02, 0x0a, 0x43, 0x80, 0x4d, 
    0x12, 0x0f, 0x17, 0x80, 0x5e, 0x85, 0x3d, 0x43, 0x85, 0x3e, 0x44, 0xe5, 0x45, 0xc3, 0x13, 0xff, 
    0xe5, 0x43, 0xc3, 0x9f, 0x50, 0x02, 0x8f, 0x43, 0xe5, 0x46, 0xc3, 0x13, 0xff, 0xe5, 0x44, 0xc3, 
    0x9f, 0x50, 0x02, 0x8f, 0x44, 0xe5, 0x45, 0xc3, 0x13, 0xff, 0xfd, 0xe5, 0x43, 0x90, 0x0e, 0x7f, 
    0x12, 0x0e, 0xea, 0x40, 0x04, 0xee, 0x9f, 0xf5, 0x43, 0xe5, 0x46, 0xc3, 0x13, 0xff, 0xfd, 0xe5, 
    0x44, 0x90, 0x0e, 0x80, 0x12, 0x0e, 0xea, 0x40, 0x04, 0xee, 0x9f, 0xf5, 0x44, 0x12, 0x07, 0x8d, 
    0x80, 0x11, 0x85, 0x40, 0x46, 0x85, 0x3f, 0x45, 0x85, 0x3e, 0x44, 0x85, 0x3d, 0x43, 0x80, 0x03, 
    0x02, 0x07, 0x8d, 0x90, 0x30, 0x24, 0xe5, 0x3d, 0xf0, 0xa3, 0xe5, 0x3e, 0xf0, 0xa3, 0xe5, 0x3f, 
    0xf0, 0xa3, 0xe5, 0x40, 0xf0, 0xa3, 0xe5, 0x3c, 0xf0, 0x90, 0x30, 0x23, 0xe4, 0xf0, 0x22, 0xc0, 
    0xe0, 0xc0, 0x83, 0xc0, 0x82, 0xc0, 0xd0, 0x90, 0x3f, 0x0c, 0xe0, 0xf5, 0x32, 0xe5, 0x32, 0x30, 
    0xe3, 0x4c, 0x30, 0x35, 0x3e, 0x90, 0x60, 0x19, 0xe0, 0xf5, 0x0a, 0xa3, 0xe0, 0xf5, 0x0b, 0x90, 
    0x60, 0x1d, 0xe0, 0xf5, 0x14, 0xa3, 0xe0, 0xf5, 0x15, 0x30, 0x01, 0x06, 0x30, 0x32, 0x03, 0xd3, 
    0x80, 0x01, 0xc3, 0x92, 0x09, 0x30, 0x02, 0x06, 0x30, 0x32, 0x03, 0xd3, 0x80, 0x01, 0xc3, 0x92, 
    0x0a, 0x30, 0x32, 0x0c, 0x30, 0x03, 0x09, 0x20, 0x02, 0x06, 0x20, 0x01, 0x03, 0xd3, 0x80, 0x01, 
    0xc3, 0x92, 0x0b, 0x90, 0x30, 0x01, 0xe0, 0x44, 0x40, 0xf0, 0xe0, 0x54, 0xbf, 0xf0, 0xe5, 0x32, 
    0x30, 0xe1, 0x14, 0x30, 0x33, 0x11, 0x90, 0x30, 0x22, 0xe0, 0xf5, 0x08, 0xe4, 0xf0, 0x30, 0x00, 
    0x03, 0xd3, 0x80, 0x01, 0xc3, 0x92, 0x08, 0xe5, 0x32, 0x30, 0xe5, 0x12, 0x90, 0x56, 0xa1, 0xe0, 
    0xf5, 0x09, 0x30, 0x30, 0x09, 0x30, 0x05, 0x03, 0xd3, 0x80, 0x01, 0xc3, 0x92, 0x0d, 0x90, 0x3f, 
    0x0c, 0xe5, 0x32, 0xf0, 0xd0, 0xd0, 0xd0, 0x82, 0xd0, 0x83, 0xd0, 0xe0, 0x32, 0x90, 0x0e, 0x7d, 
    0xe4, 0x93, 0xfe, 0x74, 0x01, 0x93, 0xff, 0xc3, 0x90, 0x0e, 0x7b, 0x74, 0x01, 0x93, 0x9f, 0xff, 
    0xe4, 0x93, 0x9e, 0xfe, 0xe4, 0x8f, 0x3b, 0x8e, 0x3a, 0xf5, 0x39, 0xf5, 0x38, 0xab, 0x3b, 0xaa, 
    0x3a, 0xa9, 0x39, 0xa8, 0x38, 0xaf, 0x49, 0xfc, 0xfd, 0xfe, 0x12, 0x05, 0xf1, 0x12, 0x0e, 0xfc, 
    0xe4, 0x7b, 0xff, 0xfa, 0xf9, 0xf8, 0x12, 0x06, 0x7c, 0x12, 0x0e, 0xfc, 0x90, 0x0e, 0x69, 0xe4, 
    0x12, 0x0f, 0x11, 0x12, 0x0e, 0xfc, 0xe4, 0x85, 0x48, 0x37, 0xf5, 0x36, 0xf5, 0x35, 0xf5, 0x34, 
    0xaf, 0x37, 0xae, 0x36, 0xad, 0x35, 0xac, 0x34, 0xa3, 0x12, 0x0f, 0x11, 0x8f, 0x37, 0x8e, 0x36, 
    0x8d, 0x35, 0x8c, 0x34, 0xe5, 0x3b, 0x45, 0x37, 0xf5, 0x3b, 0xe5, 0x3a, 0x45, 0x36, 0xf5, 0x3a, 
    0xe5, 0x39, 0x45, 0x35, 0xf5, 0x39, 0xe5, 0x38, 0x45, 0x34, 0xf5, 0x38, 0xe4, 0xf5, 0x22, 0xf5, 
    0x23, 0x85, 0x3b, 0x31, 0x85, 0x3a, 0x30, 0x85, 0x39, 0x2f, 0x85, 0x38, 0x2e, 0x02, 0x0d, 0xc5, 
    0xad, 0x39, 0xac, 0x38, 0xfa, 0xf9, 0xf8, 0x12, 0x05, 0xf1, 0x8f, 0x3b, 0x8e, 0x3a, 0x8d, 0x39, 
    0x8c, 0x38, 0xab, 0x37, 0xaa, 0x36, 0xa9, 0x35, 0xa8, 0x34, 0x22, 0xef, 0x25, 0xe0, 0x24, 0x4c, 
    0xf8, 0xe6, 0xfc, 0x08, 0xe6, 0xfd, 0x22, 0x93, 0xff, 0xe4, 0xfc, 0xfd, 0xfe, 0x12, 0x05, 0xf1, 
    0x8f, 0x37, 0x8e, 0x36, 0x8d, 0x35, 0x8c, 0x34, 0x22, 0xf9, 0xc3, 0xe6, 0x97, 0x18, 0xe6, 0x19, 
    0x97, 0x22, 0xff, 0xa6, 0x06, 0x08, 0xa6, 0x07, 0x22, 0xe6, 0xfe, 0x08, 0xe6, 0xff, 0xe4, 0x8f, 
    0x37, 0x8e, 0x36, 0xf5, 0x35, 0xf5, 0x34, 0x22, 0xe6, 0xfe, 0x08, 0xe6, 0xff, 0xe4, 0x8f, 0x3b, 
    0x8e, 0x3a, 0xf5, 0x39, 0xf5, 0x38, 0x22, 0xe7, 0x96, 0xff, 0x19, 0xe7, 0x18, 0x96, 0x22, 0xff, 
    0xa6, 0x06, 0x08, 0xa6, 0x07, 0x78, 0x6c, 0xe6, 0xfe, 0x08, 0xe6, 0x22, 0x78, 0x4c, 0xe6, 0xfe, 
    0x08, 0xe6, 0x22, 0x78, 0xa7, 0xef, 0x26, 0xf6, 0x18, 0xe4, 0x36, 0xf6, 0x22, 0x78, 0x50, 0x7e, 
    0x00, 0xe6, 0xfc, 0x08, 0xe6, 0xfd, 0x12, 0x05, 0x8a, 0x7c, 0x00, 0x22, 0xe0, 0xa3, 0xe0, 0x75, 
    0xf0, 0x02, 0xa4, 0xff, 0xae, 0xf0, 0xc3, 0x08, 0xe6, 0x9f, 0xf6, 0x18, 0xe6, 0x9e, 0xf6, 0x22, 
    0xff, 0xe5, 0xf0, 0x34, 0x60, 0x8f, 0x82, 0xf5, 0x83, 0xec, 0xf0, 0x22, 0xe4, 0xfc, 0xfd, 0x12, 
    0x07, 0x4f, 0x78, 0x5a, 0xe6, 0xc3, 0x13, 0xfe, 0x08, 0xe6, 0x13, 0x22, 0x78, 0x50, 0xe6, 0xfe, 
    0x08, 0xe6, 0xff, 0xe4, 0xfc, 0xfd, 0x22, 0xe7, 0xc4, 0xf8, 0x54, 0xf0, 0xc8, 0x68, 0xf7, 0x09, 
    0xe7, 0xc4, 0x54, 0x0f, 0x48, 0xf7, 0x22, 0xe6, 0xfc, 0xed, 0x75, 0xf0, 0x04, 0xa4, 0x22, 0xe0, 
    0xfe, 0xa3, 0xe0, 0xfd, 0xee, 0xf6, 0xed, 0x08, 0xf6, 0x22, 0x13, 0xff, 0xc3, 0xe6, 0x9f, 0xff, 
    0x18, 0xe6, 0x9e, 0xfe, 0x22, 0xe6, 0xc3, 0x13, 0xf7, 0x08, 0xe6, 0x13, 0x09, 0xf7, 0x22, 0x75, 
    0x89, 0x03, 0x75, 0xa8, 0x01, 0x75, 0xb8, 0x04, 0x75, 0x34, 0xff, 0x75, 0x35, 0x0e, 0x75, 0x36, 
    0x15, 0x75, 0x37, 0x0d, 0x12, 0x0d, 0x85, 0x12, 0x00, 0x09, 0x12, 0x0f, 0x17, 0x12, 0x00, 0x06, 
    0xd2, 0x00, 0xd2, 0x33, 0xd2, 0xaf, 0x75, 0x34, 0xff, 0x75, 0x35, 0x0e, 0x75, 0x36, 0x49, 0x75, 
    0x37, 0x03, 0x12, 0x0d, 0x85, 0x30, 0x08, 0x09, 0xc2, 0x33, 0x12, 0x09, 0x5d, 0xc2, 0x08, 0xd2, 
    0x33, 0x30, 0x0b, 0x09, 0xc2, 0x35, 0x12, 0x00, 0x0e, 0xc2, 0x0b, 0xd2, 0x35, 0x30, 0x09, 0x09, 
    0xc2, 0x35, 0x12, 0x03, 0x0a, 0xc2, 0x09, 0xd2, 0x35, 0x30, 0x0e, 0x03, 0x12, 0x07, 0x8d, 0x30, 
    0x34, 0xd3, 0x90, 0x30, 0x29, 0xe5, 0x1e, 0xf0, 0xb4, 0x10, 0x05, 0x90, 0x30, 0x23, 0xe4, 0xf0, 
    0xc2, 0x34, 0x80, 0xc1, 0xe4, 0xf5, 0x49, 0x90, 0x0e, 0x77, 0x93, 0xff, 0xe4, 0x8f, 0x37, 0xf5, 
    0x36, 0xf5, 0x35, 0xf5, 0x34, 0xaf, 0x37, 0xae, 0x36, 0xad, 0x35, 0xac, 0x34, 0x90, 0x0e, 0x6a, 
    0x12, 0x0f, 0x11, 0x8f, 0x37, 0x8e, 0x36, 0x8d, 0x35, 0x8c, 0x34, 0x90, 0x0e, 0x72, 0x12, 0x07, 
    0x32, 0xef, 0x45, 0x37, 0xf5, 0x37, 0xee, 0x45, 0x36, 0xf5, 0x36, 0xed, 0x45, 0x35, 0xf5, 0x35, 
    0xec, 0x45, 0x34, 0xf5, 0x34, 0xe4, 0xf5, 0x22, 0xf5, 0x23, 0x85, 0x37, 0x31, 0x85, 0x36, 0x30, 
    0x85, 0x35, 0x2f, 0x85, 0x34, 0x2e, 0x12, 0x0d, 0xc5, 0xe4, 0xf5, 0x22, 0xf5, 0x23, 0x90, 0x0e, 
    0x72, 0x12, 0x0f, 0x05, 0x12, 0x0d, 0xc5, 0xe4, 0xf5, 0x22, 0xf5, 0x23, 0x90, 0x0e, 0x6e, 0x12, 
    0x0f, 0x05, 0x02, 0x0d, 0xc5, 0xae, 0x35, 0xaf, 0x36, 0xe4, 0xfd, 0xed, 0xc3, 0x95, 0x37, 0x50, 
    0x33, 0x12, 0x0f, 0x52, 0xe4, 0x93, 0xf5, 0x38, 0x74, 0x01, 0x93, 0xf5, 0x39, 0x45, 0x38, 0x60, 
    0x23, 0x85, 0x39, 0x82, 0x85, 0x38, 0x83, 0xe0, 0xfc, 0x12, 0x0f, 0x52, 0x74, 0x03, 0x93, 0x52, 
    0x04, 0x12, 0x0f, 0x52, 0x74, 0x02, 0x93, 0x42, 0x04, 0x85, 0x39, 0x82, 0x85, 0x38, 0x83, 0xec, 
    0xf0, 0x0d, 0x80, 0xc7, 0x22, 0xa2, 0xaf, 0x92, 0x31, 0xc2, 0xaf, 0xe5, 0x23, 0x45, 0x22, 0x90, 
    0x0e, 0x5d, 0x60, 0x0b, 0x12, 0x0f, 0x47, 0xe0, 0xf5, 0x2c, 0xe0, 0xf5, 0x2d, 0x80, 0x0f, 0x12, 
    0x0f, 0x47, 0xe5, 0x30, 0xf0, 0x90, 0x0e, 0x5f, 0x12, 0x0f, 0x47, 0xe5, 0x31, 0xf0, 0xa2, 0x31, 
    0x92, 0xaf, 0x22, 0x78, 0x7f, 0xe4, 0xf6, 0xd8, 0xfd, 0x75, 0x81, 0xcb, 0x02, 0x0c, 0x9f, 0x00, 
    0x11, 0x05, 0x25, 0x16, 0x33, 0x02, 0x50, 0x72, 0x6f, 0x66, 0x69, 0x74, 0x20, 0x20, 0x14, 0x00, 
    0x10, 0x00, 0x56, 0x40, 0x1a, 0x30, 0x29, 0x7e, 0x00, 0x30, 0x04, 0x20, 0xdf, 0x30, 0x05, 0x40, 
    0xbf, 0x50, 0x03, 0x00, 0xfd, 0x50, 0x27, 0x01, 0xfe, 0x60, 0x00, 0x11, 0x00, 0x3f, 0x05, 0x30, 
    0x00, 0x3f, 0x06, 0x22, 0x00, 0x3f, 0x01, 0x2a, 0x00, 0x3f, 0x02, 0x00, 0x00, 0x36, 0x06, 0x07, 
    0x00, 0x3f, 0x0b, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x40, 0xbf, 0x30, 0x01, 0x00, 
    0xbf, 0x30, 0x29, 0x70, 0x00, 0x3a, 0x00, 0x00, 0xff, 0x3a, 0x00, 0x00, 0xff, 0x36, 0x03, 0x36, 
    0x02, 0x41, 0x44, 0x58, 0x20, 0x18, 0x10, 0x0a, 0x04, 0x04, 0x00, 0x03, 0xff, 0x64, 0x00, 0x00, 
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x06, 0x00, 0x03, 0x98, 0x00, 0xcc, 0x50, 
    0x3c, 0x28, 0x1e, 0x0c, 0x0c, 0x00, 0x00, 0x10, 0x0c, 0x10, 0x04, 0x0c, 0x6e, 0x06, 0x05, 0x00, 
    0xa5, 0x5a, 0x78, 0xbc, 0xe6, 0xd3, 0x08, 0xff, 0xe6, 0x64, 0x80, 0xf8, 0xef, 0x64, 0x80, 0x98, 
    0x22, 0x93, 0xff, 0x7e, 0x00, 0xe6, 0xfc, 0x08, 0xe6, 0xfd, 0x12, 0x05, 0x8a, 0x78, 0xbf, 0xe6, 
    0xfc, 0x08, 0xe6, 0xfd, 0xd3, 0xef, 0x9d, 0xee, 0x9c, 0x22, 0x78, 0xbb, 0xd3, 0xe6, 0x64, 0x80, 
    0x94, 0x80, 0x22, 0x25, 0xe0, 0x24, 0x0a, 0xf8, 0xe6, 0xfe, 0x08, 0xe6, 0xff, 0x22, 0xd2, 0x01, 
    0xc2, 0x02, 0xe4, 0xf5, 0x1f, 0xf5, 0x1e, 0xd2, 0x34, 0xd2, 0x32, 0xd2, 0x35, 0xd2, 0x01, 0xc2, 
    0x02, 0xf5, 0x1f, 0xf5, 0x1e, 0xd2, 0x34, 0xd2, 0x32, 0x22, 0x2d, 0xfd, 0xe4, 0x33, 0xfc, 0xe4, 
    0x93, 0xfe, 0xfb, 0xd3, 0xed, 0x9b, 0x74, 0x80, 0xf8, 0x6c, 0x98, 0x22, 0x8f, 0x3b, 0x8e, 0x3a, 
    0x8d, 0x39, 0x8c, 0x38, 0x22, 0x12, 0x07, 0x32, 0x8f, 0x31, 0x8e, 0x30, 0x8d, 0x2f, 0x8c, 0x2e, 
    0x22, 0x93, 0xf9, 0xf8, 0x02, 0x07, 0x1f, 0x90, 0x0e, 0x81, 0x12, 0x07, 0x32, 0x8f, 0x46, 0x8e, 
    0x45, 0x8d, 0x44, 0x8c, 0x43, 0xd2, 0x06, 0x30, 0x06, 0x03, 0xd3, 0x80, 0x01, 0xc3, 0x92, 0x0e, 
    0x22, 0xc0, 0xe0, 0xc0, 0x83, 0xc0, 0x82, 0x90, 0x3f, 0x0d, 0xe0, 0xf5, 0x33, 0xe5, 0x33, 0xf0, 
    0xd0, 0x82, 0xd0, 0x83, 0xd0, 0xe0, 0x32, 0xe4, 0x93, 0xfe, 0x74, 0x01, 0x93, 0xf5, 0x82, 0x8e, 
    0x83, 0x22, 0x8f, 0x82, 0x8e, 0x83, 0x75, 0xf0, 0x04, 0xed, 0x02, 0x07, 0x5b,  
};

const size_t OV5640_NumOfAF_FW = (size_t)(sizeof(OV5640_AF_FW)/sizeof(OV5640_AF_FW[0]));
