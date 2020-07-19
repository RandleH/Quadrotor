#include "MIMXRT1052.h"
#include "fsl_elcdif.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "fonts.h"
#include "common.h"

#ifndef _LCD_H
#define _LCD_H

#define __ENABLE_LCD_IRQ__ true
#define GT911_HEIGHT 480
#define GT911_WIDTH  800
#define GT911_HSW 	 1
#define GT911_HFP 	 22
#define GT911_HBP 	 46
#define GT911_VSW 	 1
#define GT911_VFP 	 22
#define GT911_VBP 	 23
#define GT911_POLARITY_FLAGS	(kELCDIF_DataEnableActiveHigh |	\
							     kELCDIF_VsyncActiveLow       | \
							     kELCDIF_HsyncActiveLow       | \
							     kELCDIF_DriveDataOnRisingClkEdge)

typedef uint16_t pixel_t;

#define LCD_BL_IOMUXC			        IOMUXC_GPIO_AD_B0_15_GPIO1_IO15
#define LCD_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_3_MAX_200MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)  
#define LCD_BUS_24_BIT 0 

#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

enum{
	CL_WHITE        = RGB(255,255,255),	
	CL_BLACK        = RGB(  0,  0,  0),	
	CL_RED          = RGB(255,	0,  0),	
	CL_GREEN        = RGB(  0,255,  0),	
	CL_BLUE         = RGB(  0,	0,255),	
	CL_YELLOW       = RGB(255,255,  0),	

	CL_GREY         = RGB( 98, 98, 98), 
	CL_GREY1		= RGB( 150, 150, 150), 
	CL_GREY2		= RGB( 180, 180, 180), 
	CL_GREY3		= RGB( 200, 200, 200), 
	CL_GREY4		= RGB( 230, 230, 230), 	

	CL_BUTTON_GREY	= RGB( 220, 220, 220), 

	CL_MAGENTA      = RGB(255, 0, 255),	
	CL_CYAN         = RGB( 0, 255, 255),	

	CL_BLUE1        = RGB(  0,  0, 240),		
	CL_BLUE2        = RGB(  0,  0, 128),	
	CL_BLUE3        = RGB( 68, 68, 255),	
	CL_BLUE4        = RGB(  0, 64, 128),	


	CL_BTN_FACE	    = RGB(236, 233, 216),	
	CL_BOX_BORDER1	= RGB(172, 168,153),
	CL_BOX_BORDER2	= RGB(255, 255,255),

	CL_MASK			= 0x7FFF	
};



class LCD{
public:
	LCD(void){}
	LCD(uint16_t panelWidth,uint16_t panelHeight,\
		uint8_t  hsw,uint8_t hfp,uint8_t hbp,uint8_t vsw,uint8_t vfp,uint8_t vbp,\
		uint32_t polarityFlags,uint32_t bufferAddr,\
		elcdif_pixel_format_t pixelFormat,elcdif_lcd_data_bus_t dataBus,
		bool enableIRQ){
        
		this->lcd_cfg.panelWidth    = this->panelWidth  = panelWidth;
		this->lcd_cfg.panelHeight   = this->panelHeight = panelHeight;
		this->lcd_cfg.hsw           = hsw;
		this->lcd_cfg.hfp           = hbp;
		this->lcd_cfg.vsw           = vsw;
		this->lcd_cfg.vfp           = vfp;
		this->lcd_cfg.vbp           = vbp;
		this->lcd_cfg.polarityFlags = polarityFlags;
		this->lcd_cfg.bufferAddr    = bufferAddr;
		this->lcd_cfg.pixelFormat   = pixelFormat;
		this->lcd_cfg.dataBus       = dataBus;

		LCD::setClock();
		LCD::IOMUXC_PAD_Config();
        LCD::IOMUXC_MUX_Config();
		ELCDIF_RgbModeInit(LCDIF, &this->lcd_cfg);
		ELCDIF_RgbModeStart(LCDIF);
        if(enableIRQ){
			EnableIRQ(LCDIF_IRQn);
			ELCDIF_EnableInterrupts(LCDIF, kELCDIF_CurFrameDoneInterruptEnable);
		}
	}

	static volatile uint32_t s_frame_count;
	static volatile bool     s_frame_done;

	static void IOMUXC_PAD_Config(void){

	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_LCD_CLK,LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_LCD_ENABLE, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_LCD_HSYNC, LCD_PAD_CONFIG_DATA);  
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_LCD_VSYNC, LCD_PAD_CONFIG_DATA); 

	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LCD_DATA00, LCD_PAD_CONFIG_DATA); 
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LCD_DATA01, LCD_PAD_CONFIG_DATA); 
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_LCD_DATA02, LCD_PAD_CONFIG_DATA);  
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_LCD_DATA03, LCD_PAD_CONFIG_DATA); 
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_LCD_DATA04, LCD_PAD_CONFIG_DATA); 
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_LCD_DATA05, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_LCD_DATA06, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_LCD_DATA07, LCD_PAD_CONFIG_DATA);  
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_LCD_DATA08, LCD_PAD_CONFIG_DATA);  
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_LCD_DATA09, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_LCD_DATA10, LCD_PAD_CONFIG_DATA); 
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_15_LCD_DATA11, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_LCD_DATA12, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_LCD_DATA13, LCD_PAD_CONFIG_DATA);
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_LCD_DATA14, LCD_PAD_CONFIG_DATA); 
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_LCD_DATA15, LCD_PAD_CONFIG_DATA); 
			
#if LCD_BUS_24_BIT
		IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_LCD_DATA16, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_LCD_DATA17, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_LCD_DATA18, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_LCD_DATA19, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_LCD_DATA20, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_LCD_DATA21, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_LCD_DATA22, LCD_PAD_CONFIG_DATA);                                    
	    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_LCD_DATA23, LCD_PAD_CONFIG_DATA);                                    
#endif     
	    IOMUXC_SetPinConfig(LCD_BL_IOMUXC, LCD_PAD_CONFIG_DATA);

	}

	static void IOMUXC_MUX_Config(void){
		IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_LCD_CLK, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_LCD_ENABLE, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_LCD_HSYNC, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_LCD_VSYNC, 0U);
	//B[7:3]  
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_LCD_DATA00, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_LCD_DATA01, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_LCD_DATA02, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_LCD_DATA03, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LCD_DATA04, 0U); 
	//G[7:2]
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LCD_DATA05, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_LCD_DATA06, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_LCD_DATA07, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_LCD_DATA08, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_LCD_DATA09, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_LCD_DATA10, 0U); 
	//R[7:3]                                   
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_LCD_DATA11, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LCD_DATA12, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LCD_DATA13, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_LCD_DATA14, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_LCD_DATA15, 0U); 

#if LCD_BUS_24_BIT
		IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_LCD_DATA16, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_LCD_DATA17, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_LCD_DATA18, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_LCD_DATA19, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_LCD_DATA20, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_LCD_DATA21, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_LCD_DATA22, 0U);                                    
	    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_LCD_DATA23, 0U);                                    
#endif 
			
	    IOMUXC_SetPinMux(LCD_BL_IOMUXC, 0U);
	}
	


private:
	elcdif_rgb_mode_config_t lcd_cfg;
	static void setClock(void){
		clock_video_pll_config_t config = {
            .loopDivider = 36, 
            .postDivider = 8 , 
            .numerator   = 0 , 
            .denominator = 0 ,};
        CLOCK_InitVideoPll(&config);
        CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);
        CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 1);
        CLOCK_SetDiv(kCLOCK_LcdifDiv   , 1);
	}
protected:
	uint16_t panelWidth;
	uint16_t panelHeight;
	size_t   bitPerPixel;
	uint16_t cursorPos[2];
};


class GT911:public LCD{
public:
	GT911(void){}
	GT911(bool enableIRQ);
	void init(LCDIF_Type * LCDIFx,bool enableIRQ);
	static uint32_t FrameBuffer;
	void backLight(_switch_status status);
	void getFrameRate(void);
	void fill(pixel_t color);
	void fill(uint32_t XPos_s,uint32_t YPos_s,uint32_t XPos_e,uint32_t YPos_e,pixel_t color);
	void clear(uint32_t XPos_s,uint32_t YPos_s,uint32_t XPos_e,uint32_t YPos_e);
	void clear(void);
	void setBackColor(pixel_t color);
	void setTextColor(pixel_t color);
	void setFrameBuffer(uint8_t index);
	void setFont(sFONT& font);
	void displayChar(uint32_t XPos,uint32_t YPos,char ascii);
	void displayStr(uint32_t XPos,uint32_t YPos,const char* pStr);
	void displayStr(uint32_t XPos,uint32_t YPos,const char* pStr,sFONT& font);
	void printf(const char* format,...);
	~GT911(){}
private:
    pixel_t    TextColor;
    pixel_t    BackColor;
    sFONT      font;
};

extern pixel_t gt911_buffer[1][GT911_HEIGHT][GT911_WIDTH];

#endif

