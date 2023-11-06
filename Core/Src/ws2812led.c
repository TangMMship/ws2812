//
// Created by TangM on 2023/11/5.
//
#include "main.h"
#include "ws2812led.h"
#include "tim.h"
//每帧1.25ns就是800khz  72000000/800000=90   pwm计数90次  （0码高电平时间）/（周期）--->  0.4 / 1.25 = 0.32 90*0.32=28 这里取25 1码同理
#define CODE_1       (58)       //1码定时器计数次数
#define CODE_0       (26)       //0码定时器计数次数



const RGB_Color_TypeDef RED      = {255,0,0};   //显示红色RGB数据
const RGB_Color_TypeDef GREEN    = {0,255,0};
const RGB_Color_TypeDef BLUE     = {0,0,255};
const RGB_Color_TypeDef MAGENTA  = {255,0,220};
const RGB_Color_TypeDef YELLOW   = {127,216,0};
const RGB_Color_TypeDef OEANGE   = {127,106,0};
const RGB_Color_TypeDef BLACK    = {0,0,0};
const RGB_Color_TypeDef WHITE    = {255,255,255};
const RGB_Color_TypeDef SKY      = {0,255,255};


#define led_nums  4
/*
数据代表一个LED，最后一行24个0代表RESET码  所以数量要比灯数量多1
 */
uint32_t pixel_led[led_nums+1][24];


/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：LedId为LED序号，Color：定义的颜色结构体
*/
void RGB_SetColor(uint8_t LedId,RGB_Color_TypeDef Color)
{
    uint8_t i;
    if (LedId > led_nums)return; //avoid overflow 防止写入ID大于LED总数

    for (i = 0;  i < 8 ; i++) pixel_led[LedId][i] = ((Color.G & (1 << (7 - i))) ? (CODE_1) : CODE_0);//数组某一行0~7转化存放G
    for (i = 8;  i < 16; i++) pixel_led[LedId][i] = ((Color.R & (1 << (15 - i))) ? (CODE_1) : CODE_0);//数组某一行8~15转化存放R
    for (i = 16; i < 24; i++) pixel_led[LedId][i] = ((Color.B & (1 << (23 - i))) ? (CODE_1) : CODE_0);//数组某一行16~23转化存放B

}

/*
功能：最后一行装在24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*1.2=30us > 24us(要求大于24us)
*/
void Reset_Load()
{
    uint8_t i;
    for(i=0;i<24;i++)
    {
        pixel_led[led_nums][i] = 0;
    }
}


//pwm为向上计数  1码和0码计数值放在pixel_led中   每个led用24个码确定
void send_1f()
{
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pixel_led,(led_nums+1)*24);
}


void RGB_RED(uint16_t Pixel_Len)
{
    uint16_t i;
    for(i=0;i<Pixel_Len;i++)//给对应个数LED写入红色
    {
        RGB_SetColor(i,RED);
    }
    for(i=Pixel_Len;i<led_nums;i++)
    {
        RGB_SetColor(i,BLACK);
    }
    Reset_Load();
    send_1f();
}


void RGB_GREEN(uint16_t Pixel_Len)
{
    uint16_t i;
    for(i=0;i<Pixel_Len;i++)//给对应个数LED写入绿色
    {
        RGB_SetColor(i,GREEN);
    }
    for(i=Pixel_Len;i<led_nums;i++)
    {
        RGB_SetColor(i,BLACK);
    }
    Reset_Load();
    send_1f();
}

/*描述:给相应长度的灯带全变黑
 * Pixel_Len变黑的灯个数
 * */
void RGB_BLACK(uint16_t Pixel_Len)
{
    uint16_t i;
    for(i=0;i<Pixel_Len;i++)//给对应个数LED写入红色
    {
        RGB_SetColor(i,BLACK);
    }
    Reset_Load();
    send_1f();
}

/*描述:给相应长度的灯带全变黑
 * Pixel_Len变黑的灯个数  但此函数只针对数组  不直接作用显示
 * */
void set_black_len(uint16_t Pixel_Len)
{
    uint16_t i;
    for(i=0;i<Pixel_Len;i++)//给对应个数LED写入红色
    {
        RGB_SetColor(i,BLACK);
    }

}

//把数组相应位置变黑  对应第n个灯
void set_black_local(uint16_t Pixel_site)
{
    RGB_SetColor(Pixel_site,BLACK);
}

//给第Pixle_local位置的灯亮相应颜色 其他灯不管
void RGB_color_local(uint16_t Pixel_local,RGB_Color_TypeDef color)
{
    RGB_SetColor(Pixel_local,color);
    Reset_Load();
    send_1f();
}