#ifndef __FUNC_H__
#define __FUNC_H__
#include "stm32l4xx_hal.h"

void Device_Config(void);
int my_atoi(uint8_t *);
double Cal_success_rate(uint8_t data[], uint8_t cnt, uint8_t flag);
double Cal_average_RSSI(int data[], uint8_t cnt, uint8_t flag);

#endif
