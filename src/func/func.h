#ifndef __FUNC_H__
#define __FUNC_H__
#include "stm32l4xx_hal.h"

typedef struct
{
    int rssi;
    int snr;
    uint8_t state;
} recv_data;

void Device_Config(void);
int my_atoi(uint8_t *);
double Cal_success_rate(recv_data data[], uint8_t cnt, uint8_t flag);
double Cal_average_RSSI(recv_data data[], uint8_t cnt, uint8_t flag);

#endif
