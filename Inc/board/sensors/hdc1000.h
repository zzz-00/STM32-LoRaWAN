/*
** file:	hdc1000.h
** data:	2016-8-22 16:36:02
** author£º	eric.xu @ lierda
*/
#ifndef __HDC1000_H_
#define __HDC1000_H_

#include "stm32l4xx_hal.h"

#define BASE_NUM	(0x10000)

#define I2C_TIMING_100KHZ       0x10A13E56
#define I2C_TIMING_400KHZ       0x00B1112E

/*  temperature and humidity, HDC1000 gpio */
#define HDC1000_DYDRn_PIN								GPIO_PIN_9
#define HDC1000_DYDRn_GPIO_PORT							GPIOC
#define HDC1000_DYDRn_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOC_CLK_ENABLE()
#define HDC1000_DYDRn_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOC_CLK_DISABLE()

/* according to hdc1000 spec */
#define HDC1000_Device_Adderss      0x80

/* register address */
#define HDC1000_Read_Temperature    0x00
#define HDC1000_Read_Humidity       0x01
#define HDC1000_Read_Conf           0x02

/* 0xFE */
#define HDC1000_MANUFACTURER_ID     0xFE	//0x5449	//TI 
/* 0xFF */
#define HDC1000_DEVICE_ID           0xFF	//0x1000

/*
** 0x02 configuration
** BIT[15]:		RST
** BIT[13]:		Enable Heater
** BIT[12]:		Select acquisition mode
** BIT[11]:		Battery status(read only)
** BIT[10]:		Temperature resolution
** BIT[9:8]:	Humidity resolution
*/
#define HDC1000_RST		(0x1 << 15)	// soft reset
#define HDC1000_ENHT	(0x1 << 13)	// enable heater
#define HDC1000_MODE	(0x1 << 12)	// 1 = Temperature and Humidity are acquired in sequence
// 0 = Temperature and Humidity are acquired separately
#define HDC1000_TRES	(0x0 << 10)	// 0 - 14 bit resolution, 1 - 11 bit resolution
#define HDC1000_RHRES	(0x0 << 8)	// 0 - 14 bit resolution, 1 - 11 bit resolution, 2 - 7 bit resolution

/* function */
void HDC1000_IO_Init(uint32_t address);
void HDC1000_Init(void);
void I2Cx_Init(I2C_HandleTypeDef *i2c_handler, uint32_t address);
uint16_t HDC1000_Read_Temper(void);
uint16_t HDC1000_Read_Humidi(void);
uint16_t HDC1000_Read_ManufacturerID(void);
uint16_t HDC1000_Read_DeviceID(void);
I2C_HandleTypeDef *Get_HDC1000_Handler(void);
#endif

