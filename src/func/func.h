#ifndef __FUNC_H__
#define __FUNC_H__
#include "stm32l4xx_hal.h"

static uint32_t prngState = 0;
extern uint8_t position[45][33];

void Device_Config(void);
void Read_Hex(uint8_t buffer[], uint16_t len, uint8_t *data);
double Cal_success_rate(uint8_t data[], uint8_t cnt, uint8_t flag);
double Cal_average_RSSI(int data[], uint8_t cnt, uint8_t flag);
int LCD_Set_Scroll_Area(uint16_t tfa, uint16_t vsa, uint16_t bta);
void LCD_Set_Scroll_Start_Address(uint16_t vsp);
void Draw_background(void);
void Clear_Point(int num);
void Draw_LineChart(int data[]);
uint32_t init_rand(uint32_t seed);
uint32_t _rand(void);
uint32_t rand_range(uint32_t min, uint32_t max);
void Map(int RSSI);
void Stiimulate_Location(int RSSI);
void Move_Judge_1st(uint8_t mode);

#endif
