//
// Created by TangM on 2023/11/5.
//

#ifndef _WS2812led_H
#define _WS2812led_H
#include "main.h"
typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_Color_TypeDef;

extern const RGB_Color_TypeDef SKY;
extern const RGB_Color_TypeDef RED;
extern const RGB_Color_TypeDef GREEN;
extern const RGB_Color_TypeDef BLUE;
extern const RGB_Color_TypeDef MAGENTA;
extern const RGB_Color_TypeDef YELLOW ;
extern const RGB_Color_TypeDef OEANGE ;
extern const RGB_Color_TypeDef BLACK  ;
extern const RGB_Color_TypeDef WHITE  ;





void RGB_RED(uint16_t Pixel_Len);
void RGB_GREEN(uint16_t Pixel_Len);
void RGB_BLACK(uint16_t Pixel_Len);
void RGB_color_local(uint16_t Pixel_local,RGB_Color_TypeDef color);
void set_black_local(uint16_t Pixel_site);
void set_black_len(uint16_t Pixel_Len);
#endif
