#ifndef __XPT2046_H__
#define __XPT2046_H__

#include "stm32l4xx_hal.h"
#include "gpio.h"

//#define OLD_LCD

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

//笔杆结构体
typedef struct 
{
	u16 X0;//原始坐标
	u16 Y0;
	u16 X; //最终/暂存坐标
	u16 Y;						   	    
	u8  Key_Sta;//笔的状态			  
//触摸屏校准参数
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;


//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 指令集
#define CMD_RDY 0X90  //0B10010000即用差分方式读X坐标
#define CMD_RDX	0XD0  //0B11010000即用差分方式读Y坐标   											 
#define TEMP_RD	0XF0  //0B11110000即用差分方式读Y坐标   



#define XPT2046_CLK_HIGH()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define XPT2046_CLK_LOW()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)

#define XPT2046_CS_HIGH()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define XPT2046_CS_LOW()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)

#define XPT2046_D_OUT_HIGH()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define XPT2046_D_OUT_LOW()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)

void XPT2046_init(void);
void XPT2046_write(uint8_t ch);
uint16_t XPT2046_read(uint8_t CMD);
uint16_t ADS_Read_XY(uint8_t xy);
uint8_t Read_ADS(uint16_t *x,uint16_t *y);
uint8_t Read_ADS2(uint16_t *x,uint16_t *y);

void Delay_us(uint16_t i);

void Drow_Touch_Point(u8 x,u16 y,u16 color);
int Touch_Adjust(void);

extern uint8_t LCD_EN;
#endif

