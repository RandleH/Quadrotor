#include "stdarg.h"
#include "stdio.h"
#include "fsl_gpio.h"

#include "lcd.h"

#define LCD_BL_GPIO 				      GPIO1
#define LCD_BL_GPIO_PIN 		          (15U)

#define RGB(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3)) 
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)


AT_NONCACHEABLE_SECTION_ALIGN(pixel_t gt911_buffer[1][GT911_HEIGHT][GT911_WIDTH],64);
uint32_t GT911::FrameBuffer = (uint32_t)gt911_buffer[0];

volatile uint32_t LCD::s_frame_count = 0;
volatile bool     LCD::s_frame_done  = false;


GT911::GT911(bool enableIRQ):LCD(GT911_WIDTH,GT911_HEIGHT,GT911_HSW,GT911_HFP,GT911_HBP,GT911_VSW,GT911_VFP,GT911_VBP,\
	                   GT911_POLARITY_FLAGS,\
	                   (uint32_t)gt911_buffer[0],\
	                   kELCDIF_PixelFormatRGB565,\
	                   kELCDIF_DataBus16Bit,\
	                   enableIRQ),font(Font8x16){
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 
        1,
        kGPIO_NoIntmode
    };
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
    this->backLight(ON);
    this->bitPerPixel = sizeof(uint16_t);//RGB565
    this->setBackColor(CL_BLACK);
    this->setTextColor(CL_WHITE);
    this->setFont(Font8x16);
    this->cursorPos[0] = 0;
    this->cursorPos[1] = 0;
}

void GT911::init(LCDIF_Type* LCDIFx,bool enIRQ){
    GT911(true);
}


void GT911::backLight(_switch_status status){
    GPIO_PinWrite(LCD_BL_GPIO, LCD_BL_GPIO_PIN,(uint8_t)status);
}

void GT911::clear(void){
    memset((volatile pixel_t*)GT911::FrameBuffer,0,sizeof(pixel_t)*this->panelWidth*this->panelHeight);
    this->cursorPos[0] = 0;
    this->cursorPos[1] = 0;
}

void GT911::clear(uint32_t XPos_s,uint32_t YPos_s,uint32_t XPos_e,uint32_t YPos_e){
    uint32_t yBufferPos=0,xPixelPos=0;
    uint32_t y_range = YPos_e-YPos_s;
    uint32_t x_range = XPos_e-XPos_s;
    register size_t bpp = this->bitPerPixel;
    
    yBufferPos = YPos_s*(this->panelWidth)*bpp;
    xPixelPos  = XPos_s;

    while(y_range--){
        memset((volatile pixel_t*)(GT911::FrameBuffer + (bpp*xPixelPos) + yBufferPos),0,bpp*x_range);
        xPixelPos += this->panelWidth; 
    }
}

void GT911::fill(pixel_t color){
    pixel_t *buffer = (pixel_t*)(GT911::FrameBuffer);
    for (uint16_t page = 0; page < this->panelHeight; page++ ){    
        for (uint16_t column = 0; column < this->panelWidth; column++){   
          *buffer = color;
          buffer++;
        }      
    }
}

void GT911::fill(uint32_t XPos_s,uint32_t YPos_s,uint32_t XPos_e,uint32_t YPos_e,pixel_t color){
    uint32_t yBufferPos=0,xPixelPos=0;
    uint32_t y_range = YPos_e-YPos_s;
    uint32_t x_range = XPos_e-XPos_s;
    register size_t bpp = this->bitPerPixel;
    
    yBufferPos = YPos_s*(this->panelWidth)*bpp;
    xPixelPos  = XPos_s;

    while(y_range--){
        memset((volatile pixel_t*)(GT911::FrameBuffer + (bpp*xPixelPos) + yBufferPos),color,bpp*x_range);
        xPixelPos += this->panelWidth; 
    }
}

void GT911::setBackColor(pixel_t color){
    this->BackColor = color;
}

void GT911::setTextColor(pixel_t color){
    this->TextColor = color;
}

void GT911::setFrameBuffer(uint8_t index){
    index &= 0x01;
    GT911::FrameBuffer = (uint32_t)gt911_buffer[index];
}

void GT911::setFont(sFONT& font){
    this->font = font;
}

void GT911::displayChar(uint32_t XPos,uint32_t YPos,char ascii){
    register size_t bpp = this->bitPerPixel;
    uint16_t fontLength;  

    uint16_t relativePositon;
    uint8_t *pfont;
  
    uint32_t yBufferPos = 0;
    uint32_t xPixelPos = 0;
    yBufferPos = YPos*(this->panelWidth)*bpp;
    xPixelPos += XPos;
    
    relativePositon = ascii - ' ';
    fontLength = (this->font.Width*this->font.Height)>>3;
        
    pfont = (uint8_t *)&this->font.table[relativePositon * fontLength];
    
    for(uint16_t page = 0; page<this->font.Height;page++){    
        for(uint16_t col = 0;col<(this->font.Width>>3);col++){
            for(uint8_t bit=0; bit<8; bit++){
                if(*pfont & (0x80>>bit))
                    *(volatile pixel_t*)(GT911::FrameBuffer + (bpp*xPixelPos) + yBufferPos) = this->TextColor;
                else
                    *(volatile pixel_t*)(GT911::FrameBuffer + (bpp*xPixelPos) + yBufferPos) = this->BackColor; 
                xPixelPos++;        
            }
            pfont++;
        }      
        xPixelPos += (this->panelWidth - this->font.Width);       
    }
}

void GT911::displayStr(uint32_t XPos,uint32_t YPos,const char* pStr){
    while( *pStr != '\0' ){ 
        if(*pStr == '\n'){
            XPos = 0;
            YPos += this->font.Height;
            pStr++;
            continue;
        } 

        if(( XPos + this->font.Width) > this->panelWidth){
            XPos = 0;
            YPos += this->font.Height;
        }
        
        if ((YPos + this->font.Height) > this->panelHeight){
            XPos = 0;
            YPos = 0;
        }           
        this->displayChar(XPos,YPos,*pStr);
        XPos += this->font.Width;
        pStr++;

    }
}

void GT911::displayStr(uint32_t XPos,uint32_t YPos,const char* pStr,sFONT& font){
    this->setFont(font);
    this->displayStr(XPos,YPos,pStr);
}

void GT911::printf(const char* format,...){
    va_list args;
    size_t length;
    char* str_temp = new char[100];
    va_start(args, format);
    length = vsprintf(str_temp,format,args);
    for(size_t cnt=0;cnt<length;cnt++){
        if(str_temp[cnt]=='\n'){
            this->cursorPos[0] = 0;
            this->cursorPos[1] += this->font.Height;
            continue;
        }
        if((this->cursorPos[0] + this->font.Width) > this->panelWidth){
            this->cursorPos[0] = 0;
            this->cursorPos[1] += this->font.Height;
        }
        if ((this->cursorPos[1] + this->font.Height) > this->panelHeight){
            this->clear();
            this->cursorPos[0] = 0;
            this->cursorPos[1] = 0;
        }           
        this->displayChar(this->cursorPos[0],this->cursorPos[1],str_temp[cnt]);
        this->cursorPos[0] += this->font.Width;
    }
    va_end(args);
    delete str_temp;
}

#ifdef __cplusplus
extern "C"{
#endif

void LCDIF_IRQHandler(void){
    uint32_t intStatus;
    intStatus = ELCDIF_GetInterruptStatus(LCDIF);
    ELCDIF_ClearInterruptStatus(LCDIF, intStatus);

   if(intStatus & kELCDIF_CurFrameDone){
        LCD::s_frame_done = true;
        LCD::s_frame_count++;
    }
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

#ifdef __cplusplus
}
#endif





