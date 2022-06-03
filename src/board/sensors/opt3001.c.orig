#include "opt3001.h"
#include "hdc1000.h"
#include "common.h"

extern int8_t Error_num;



I2C_HandleTypeDef opt3001;


/* opt3001 initial code */
// void LightSensor_Init()
static uint16_t OPT3001_Configure(void);

void OPT3001_Init(void)
{
	OPT3001_IO_Init(OPT3001_ADDR);	
	
	OPT3001_Configure();

	HAL_Delay(10);
}

static void OPT3001_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
	I2C_HandleTypeDef *OPT3001_Handle = Get_OPT3001_Handler();
	uint8_t tBuf[3] = {0};
	
	tBuf[0] = addr;
	tBuf[1] = *buffer++;
	tBuf[2] = *buffer;
	
	while(HAL_I2C_Master_Transmit(OPT3001_Handle, OPT3001_WRITE_MODE, tBuf, 3, 1000) != HAL_OK);
	
}


static void OPT3001_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
	I2C_HandleTypeDef *OPT3001_Handle = Get_OPT3001_Handler();
		
	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Transmit(OPT3001_Handle, OPT3001_WRITE_MODE, &addr, 1, 1000) != HAL_OK)
	{
		if(time_out_break_ms(500) == 1)
		{
			Error_num = -14;
			break;
		}
	}
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Receive(OPT3001_Handle, OPT3001_READ_MODE, buffer, len, 1000) != HAL_OK)
	{
		if(time_out_break_ms(500) == 1)
		{
			Error_num = -15;
			break;
		}
	}
}

static uint16_t OPT3001_Configure(void)
{
	uint16_t value;
	uint8_t buffer[2];
	
	/* 0xc4 0x10 */
	OPT3001_Read_Buffer(OPT3001_CONFIGURATION, buffer, 2);
	value = (buffer[0] << 8) | buffer[1]; 
	
	/* enable automatic full-scale setting mode */
	value &=  ~OPT3001_CONFIGURATION_RN_MASK;
	value |= OPT3001_CONFIGURATION_RN_AUTO;		
	
	if ((value & OPT3001_CONFIGURATION_M_MASK) == OPT3001_CONFIGURATION_M_SHUTDOWN)
	{
		value &= ~OPT3001_CONFIGURATION_M_MASK;
		value |= OPT3001_CONFIGURATION_M_CONTINUOUS;
	}
	
	buffer[0] = (value & 0xFF00) >> 8;
	buffer[1] = (value & 0x00FF);
	OPT3001_Write_Buffer(OPT3001_CONFIGURATION, buffer, 2);	
	
	return 0;
}

uint16_t OPT3001_Result(void)
{
	uint8_t buffer[2];
	
	OPT3001_Read_Buffer(OPT3001_RESULT, buffer, 2);	
	
	return ((buffer[0] << 8) | buffer[1]);
}


float OPT3001_Get_Lux(void)
{
	float lux;
	uint16_t result;
	 	
	result = OPT3001_Result();
	
	lux = 0.01*(1 << ((result & 0xF000) >> 12))*(result & 0xFFF);

	return lux;
}

uint16_t OPT3001_Read_ManufacturerID(void)
{
	uint8_t buffer[2];
	
	OPT3001_Read_Buffer(OPT3001_MANUFACTURER_ID, buffer, 2);		
	
	return ((buffer[0] << 8) | buffer[1]);
}

uint16_t OPT3001_Read_DeviceID(void)
{
	uint8_t buffer[2];
	
	OPT3001_Read_Buffer(OPT3001_DEVICE_ID, buffer, 2);		
	
	return ((buffer[0] << 8) | buffer[1]);
}

void OPT3001_Delay(uint32_t ms)
{
	HAL_Delay(ms);
}

void OPT3001_IO_Init(uint32_t address)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	/* enable gpio clock */
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//  
//	/* Configure data ready gpio */
//	GPIO_InitStruct.Pin = GPIO_PIN_13;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	I2Cx_Init(&opt3001, address);
}

I2C_HandleTypeDef *Get_OPT3001_Handler(void)
{
	return &opt3001;
}

void OPT3001IntoSleep(void)
{
    uint8_t buffer[2];
    buffer[0]=0;
    buffer[1]=0;
    OPT3001_Write_Buffer(OPT3001_CONFIGURATION, buffer, 2);	
}

