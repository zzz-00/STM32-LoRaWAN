/*
** file:	mma8451.h
** data:	2016-8-31 23:01:07
** author:	eric.xu @ lierda
*/
#ifndef __MMA8451_H_
#define __MMA8451_H_

#include "stm32l4xx_hal.h"

#define MMA8451_DEVICE_ADDRESS		0x38
#define MMA8451_READ_MODE			(MMA8451_DEVICE_ADDRESS | 1)
#define MMA8451_WRITE_MODE			MMA8451_DEVICE_ADDRESS

#define STATUS				0x00

#define OUT_X_MSB			0x01
#define OUT_X_LSB			0x02
#define	OUT_Y_MSB			0x03
#define OUT_Y_LSB			0x04
#define OUT_Z_MSB			0x05
#define OUT_Z_LSB			0x06

#define	XYZ_DATA_CFG		0x0E
#define M_CTRL_REG1			0x2A

#define ACTIVE_MASK			0x1

/* var */
typedef struct {
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
} ACCELER_T;

/* function */
void MMA8451_Init(void);
ACCELER_T MMA8451_ReadAcceleration(void);
void MMA8451_IO_Init(uint32_t address);
I2C_HandleTypeDef *Get_MMA8451_Handler(void);
void MMA8451IntoSleep(void);
#endif

