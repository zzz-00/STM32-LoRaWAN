#ifndef __OPT3001_H_
#define __OPT3001_H_

#include "stm32l4xx_hal.h"

#define OPT3001_ADDR		0x88		//addr pin connect gnd
#define OPT3001_READ_MODE	(OPT3001_ADDR | 1)
#define OPT3001_WRITE_MODE	(OPT3001_ADDR | 0)

//#define OPT3001_MANUFACTURER_ID 0x5449
//#define OPT3001_DEVICE_ID		0x3001

/* register address */
#define OPT3001_RESULT			0x00
#define OPT3001_CONFIGURATION	0x01
#define OPT3001_LOW_LIMIT		0x02
#define OPT3001_HIGH_LIMIT		0x03
#define OPT3001_MANUFACTURER_ID	0x7e	//0x5449
#define OPT3001_DEVICE_ID		0x7f	//0x3001

#define OPT3001_CONFIGURATION_RN_MASK	(0xf << 12)
#define OPT3001_CONFIGURATION_RN_AUTO	(0xc << 12)

#define OPT3001_CONFIGURATION_CT	(0x1 << 11)		//conversion time field

#define OPT3001_CONFIGURATION_M_MASK	(3 << 9)
#define OPT3001_CONFIGURATION_M_SHUTDOWN (0 << 9)
#define OPT3001_CONFIGURATION_M_SINGLE	(1 << 9)
#define OPT3001_CONFIGURATION_M_CONTINUOUS (2 << 9) /* also 3 << 9 */

#define OPT3001_CONFIGURATION_OVF	(0x1 << 8)
#define OPT3001_CONFIGURATION_CRF	(0x1 << 7)
#define OPT3001_CONFIGURATION_FH	(0x1 << 6)
#define OPT3001_CONFIGURATION_FL	(0x1 << 5)
#define OPT3001_CONFIGURATION_L		(0x1 << 4)
#define OPT3001_CONFIGURATION_POL	(0x1 << 3)
#define OPT3001_CONFIGURATION_ME	(0x1 << 2)

#define OPT3001_CONFIGURATION_FC_MASK	(3 << 0

/* The end-of-conversion enable is located in the low-limit register */
#define OPT3001_LOW_LIMIT_EOC_ENABLE	0xc000

#define OPT3001_REG_EXPONENT(n)		((n) >> 12)
#define OPT3001_REG_MANTISSA(n)		((n) & 0xfff)

#define OPT3001_INT_TIME_LONG		800
#define OPT3001_INT_TIME_SHORT		100

/*
 * Time to wait for conversion result to be ready. The device datasheet
 * sect. 6.5 states results are ready after total integration time plus 3ms.
 * This results in worst-case max values of 113ms or 883ms, respectively.
 * Add some slack to be on the safe side.
 */
#define OPT3001_RESULT_READY_SHORT	150
#define OPT3001_RESULT_READY_LONG	1000

typedef struct {
	uint32_t mode;
	uint32_t int_time;
	uint16_t high_thresh_mantissa;
	uint16_t low_thresh_mantissa;
	uint8_t high_thresh_exp;
	uint8_t low_htresh_exp;
} OPT3001_Def;


void OPT3001_Init(void);

uint16_t OPT3001_Result(void);
float OPT3001_Get_Lux(void);
uint16_t OPT3001_Read_ManufacturerID(void);
uint16_t OPT3001_Read_DeviceID(void);
void OPT3001_Delay(uint32_t ms);
void OPT3001_IO_Init(uint32_t address);
I2C_HandleTypeDef *Get_OPT3001_Handler(void);
void OPT3001IntoSleep(void);

#endif

