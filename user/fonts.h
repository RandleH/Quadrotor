#ifndef __FONT_H
#define __FONT_H       

#include "fonts.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>



#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table; 
  uint16_t Width;   
  uint16_t Height;      
} sFONT;
  

extern sFONT Font24x48;
extern sFONT Font16x32;
extern sFONT Font8x16;

#define      GetGBKCode( ucBuffer, usChar ) 

#ifdef __cplusplus
}
#endif

#endif /*end of __FONT_H    */


