#ifndef _KEY_H
#define _KEY_H

#include "stm32l4xx_hal.h"
#include "app.h"
#include "common.h"

#define A_K1 GPIO_PIN_13
#define A_K2 GPIO_PIN_6

typedef struct
{
    uint8_t A_KEY1;
    uint8_t A_KEY2;
} KEY;

void KEY_DO(KEY *key_temp,DEVICE_MODE_T *Device_Mode_temp);

#endif
