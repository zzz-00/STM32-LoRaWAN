#ifndef __LCD_APP_H
#define __LCD_APP_H
#include "stm32l4xx_hal.h"

void LCD_Adjust(void);
void Touch_Key(uint16_t x,uint16_t y);
void LCD_APP_INIT(void);
void LCD_APP_INIT_one(void);
void LCD_MODE1_do(void);

void LCD_APP_INIT_two(void);
void LCD_MODE2_do(void);
void LCD_APP_INIT_tre(void);
void LCD_MODE3_do(void);
void LCD_APP_INIT_P2P(void);
void LCD_MODE4_do(void);

void LCD_APP_Mode(void);



#endif
