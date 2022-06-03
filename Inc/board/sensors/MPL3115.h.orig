/*
** file:	mpl3115.h
** data:	2016-8-31 19:08:27
** author:	eric.xu @ lierda
*/
#ifndef __MPL3115_H_
#define __MPL3115_H_

#include "stm32l4xx_hal.h"

typedef enum {
	MODE_ALTIMETER,
	MODE_BAROMETER,
} MPL3115_MODE_T;

#define MPL3115_DEVICE_ADDRESS		0xC0
#define MPL3115_READ_MODE			0xC1
#define MPL3115_WRITE_MODE			MPL3115_DEVICE_ADDRESS

#define STATUS		0x00

/* altitude & temperature */
#define OUT_P_MSB	0x01
#define OUT_P_CSB	0x02
#define OUT_P_LSB	0x03
#define OUT_T_MSB	0x04
#define OUT_T_LSB	0x05

#define WHO_AM_I	0x0C
#define PT_DATA_CFG	0x13
#define CTRL_REG1	0x26
#define CTRL_REG2	0x27
#define CTRL_REG3	0x28

/* funtion */
void MPL3115_Init(MPL3115_MODE_T mode);
float MPL3115_ReadAltitude(void);
float MPL3115_ReadPressure(void);
float MPL3115_ReadTemp(void);
uint8_t MPL3115_ReadID(void);
void MPL3115_IO_Init(uint32_t address);
I2C_HandleTypeDef *Get_MPL3115_Handler(void);
void MPL3115IntoSleep(void);
#endif

