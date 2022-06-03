/*
** file:	hdc1000.h
** data:	2016-8-22 16:49:07
** author:	eric.xu @ lierda
*/
#include "hdc1000.h"
#include "usart.h"
#include "common.h"

extern int8_t Error_num;


//#define I2C_INT		/* i2c interrupt mode */

I2C_HandleTypeDef hdc1000;


#define HDC1000_Delay(x) HAL_Delay(x)

/* configuration register */
static void HDC1000_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
	I2C_HandleTypeDef *HDC1000_Handle = Get_HDC1000_Handler();
	/* device address | register address | register value */
	uint8_t tBuf[3] = {0};
	
	tBuf[0] = addr;
	
	if (len != 2)
	{
		return;	
	}
	
	tBuf[1] = *buffer++;
	tBuf[2] = *buffer;
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Transmit(HDC1000_Handle, HDC1000_Device_Adderss, tBuf, 3, 1000) != HAL_OK)			
	{
		if (HAL_I2C_GetError(HDC1000_Handle) != HAL_I2C_ERROR_AF)
		{
		  Error_num = -13;
		}	
		if(time_out_break_ms(500) == 1)  //超时跳出
		{
			break;
		}
	}
}

static void HDC1000_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
	I2C_HandleTypeDef *HDC1000_Handle = Get_HDC1000_Handler();
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Transmit(HDC1000_Handle, HDC1000_Device_Adderss, &addr, 1, 1000) != HAL_OK)				
	{
		if (HAL_I2C_GetError(HDC1000_Handle) != HAL_I2C_ERROR_AF)
		{
		  Error_num = -11;
		}
		if(time_out_break_ms(500) == 1)  //超时跳出
		{
			break;
		}
	}

	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Receive(HDC1000_Handle, HDC1000_Device_Adderss | 0x1, buffer, len, 1000) != HAL_OK)	
	{
		if (HAL_I2C_GetError(HDC1000_Handle) != HAL_I2C_ERROR_AF)
		{
		  Error_num = -12;
		}
		if(time_out_break_ms(500) == 1)  //超时跳出
		{
			break;
		}
	}
}

void HDC1000_IO_Init(uint32_t address)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	/* enable gpio clock */
//	HDC1000_DYDRn_GPIO_CLK_ENABLE();
//  
//	/* Configure data ready gpio */
//	GPIO_InitStruct.Pin = HDC1000_DYDRn_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(HDC1000_DYDRn_GPIO_PORT, &GPIO_InitStruct);
	
	I2Cx_Init(&hdc1000, address);
}

void I2Cx_Init(I2C_HandleTypeDef *i2c_handler, uint32_t address)
{
	if(HAL_I2C_GetState(i2c_handler) == HAL_I2C_STATE_RESET)
	{
		/* I2C Config */
		i2c_handler->Instance              = I2C1;
		i2c_handler->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
		i2c_handler->Init.Timing           = 0x10707DBC;
		i2c_handler->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
		i2c_handler->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		i2c_handler->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
		i2c_handler->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
		i2c_handler->Init.OwnAddress1      = address;
		i2c_handler->Init.OwnAddress2      = 0;
	}
	HAL_I2C_Init(i2c_handler);
	
	HAL_I2CEx_ConfigAnalogFilter(i2c_handler, I2C_ANALOGFILTER_ENABLE);
}


static void HDC1000_Soft_Reset(void)
{
	I2C_HandleTypeDef *HDC1000_Handle = Get_HDC1000_Handler();
	
	uint8_t temp[2] = {0x80, 0x00};
	
	HDC1000_Write_Buffer(HDC1000_Read_Conf, temp, 2);
	
	HDC1000_Delay(15);	//15ms
}

static void HDC1000_Set_Mode(uint16_t mode)
{
	I2C_HandleTypeDef *HDC1000_Handle = Get_HDC1000_Handler();
	uint8_t temp[2];
	
	temp[0] = (uint8_t)(mode >> 8);
	temp[1] = (uint8_t)mode;
	
	HDC1000_Write_Buffer(HDC1000_Read_Conf, temp, 2);
}

void HDC1000_Init(void)
{	
	HDC1000_IO_Init(HDC1000_Device_Adderss);
	HDC1000_Soft_Reset();
	HDC1000_Set_Mode(HDC1000_MODE);	
	
	HDC1000_Delay(10);
	/**/
}
uint16_t HDC1000_Read_Temper(void)
{
	uint8_t buffer[2];
	
	HDC1000_Read_Buffer(HDC1000_Read_Temperature, buffer, 2);

	return ((buffer[0] << 8) | buffer[1]);
}

uint16_t HDC1000_Read_Humidi(void)
{
	uint8_t buffer[2];
	
	HDC1000_Read_Buffer(HDC1000_Read_Humidity, buffer, 2);

	return ((buffer[0] << 8) | buffer[1]);
	
}

uint16_t HDC1000_Read_Config(void)
{
	uint8_t buffer[2];
	
    HDC1000_Read_Buffer(HDC1000_Read_Conf, buffer, 2);

    return ((buffer[0] << 8) | buffer[1]);
}

uint16_t HDC1000_Read_ManufacturerID(void)
{
	uint8_t buffer[2];	
	
	HDC1000_Read_Buffer(HDC1000_MANUFACTURER_ID, buffer, 2);
	
	return ((buffer[0] << 8) | buffer[1]);
}


uint16_t HDC1000_Read_DeviceID(void)
{
	uint8_t buffer[2];
	
	HDC1000_Read_Buffer(HDC1000_DEVICE_ID, buffer, 2);
	
	return ((buffer[0] << 8) | buffer[1]);
}

I2C_HandleTypeDef *Get_HDC1000_Handler(void)
{
	return &hdc1000;
}


