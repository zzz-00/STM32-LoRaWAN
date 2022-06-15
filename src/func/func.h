#ifndef __FUNC_H__
#define __FUNC_H__
#include "stm32l4xx_hal.h"

void Device_Config(void);
int my_atoi(uint8_t *);
double Cal_success_rate(uint8_t data[], uint8_t cnt, uint8_t flag);
double Cal_average_RSSI(int data[], uint8_t cnt, uint8_t flag);
int LCD_Set_Scroll_Area(uint16_t tfa, uint16_t vsa, uint16_t bta);
void LCD_Set_Scroll_Start_Address(uint16_t vsp);

#endif
