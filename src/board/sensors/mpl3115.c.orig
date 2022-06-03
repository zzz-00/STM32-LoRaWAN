/*
** file:	mpl3115.c
** data:	2016-8-31 19:09:49
** author:	eric.xu @ lierda
*/

#include "MPL3115.h"
#include "hdc1000.h"
#include "common.h"

extern int8_t Error_num;

I2C_HandleTypeDef mpl3115;

#define MPL3115_Delay(x)	HAL_Delay(x)

static void MPL3115_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t buf_len)
{
	I2C_HandleTypeDef *MPL3115_Handle = Get_MPL3115_Handler();
	/* device address | register address | register value */
	uint8_t tBuf[2] = {0};
	
	tBuf[0] = addr;	
	tBuf[1] = *buffer;
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Transmit(MPL3115_Handle, MPL3115_WRITE_MODE, tBuf, 2, 1000) != HAL_OK)
	{
		if(time_out_break_ms(500) == 1)
		{
			Error_num = -17;
			break;
		}
	}
}

static void MPL3115_Read_Buffer(uint8_t addr, uint8_t addr_len, uint8_t *buffer, uint8_t buf_len)
{
	I2C_HandleTypeDef *MPL3115_Handle = Get_MPL3115_Handler();
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Mem_Read(MPL3115_Handle, MPL3115_DEVICE_ADDRESS, addr, addr_len, buffer, buf_len, 1000) != HAL_OK)
	{
		if(time_out_break_ms(500) == 1)
		{
			Error_num = -17;
			break;
		}
	}
	
}

void MPL3115_SetMode(MPL3115_MODE_T mode)
{
	uint8_t value;
	
	if (mode == MODE_ALTIMETER)
	{
		/* set altimeter mode with OSR = 128 */
		value = 0xB8;
		MPL3115_Write_Buffer(CTRL_REG1, &value, 1);

		/* enable data flag */
		value = 0x07;
		MPL3115_Write_Buffer(PT_DATA_CFG, &value, 1);

		/* polling mode, set active */
		value = 0xB9;
		MPL3115_Write_Buffer(CTRL_REG1, &value, 1);	
	} else {
        //value = 0x00;
		//MPL3115_Write_Buffer(CTRL_REG1, &value, 1);                
		value = 0x21;
		MPL3115_Write_Buffer(CTRL_REG1, &value, 1);	
        value = 0x00;
        MPL3115_Write_Buffer(CTRL_REG2, &value, 1);
        value = 0x11;
        MPL3115_Write_Buffer(CTRL_REG3, &value, 1);
	}	

}

/*
** CTRL_REG1 0x26
** ALT Altimeter-Barometer mode, if 1 in Altimeter mode, 0 in Barometer mode
*/
void MPL3115_Init(MPL3115_MODE_T mode)
{ 	
	MPL3115_IO_Init(MPL3115_DEVICE_ADDRESS);
	
	MPL3115_SetMode(mode);

	HAL_Delay(10);
}

//static uint8_t status;
uint8_t MPL3115_ReadStatus()
{
	uint8_t status;
	
	MPL3115_Read_Buffer(STATUS, 1, &status, 1);
	
	return status;
}


float MPL3115_ReadPressure(void)
{
  	uint8_t msb = 0, csb = 0, lsb = 0;
	float pressure;
  
	MPL3115_ReadStatus();
    MPL3115_Read_Buffer(OUT_P_MSB, 1, &msb, 1);
    MPL3115_Read_Buffer(OUT_P_CSB, 1, &csb, 1);
    MPL3115_Read_Buffer(OUT_P_LSB, 1, &lsb, 1);

    pressure = (float)((msb << 16 | csb << 8 | lsb)/64.00); 
  	
	return pressure;
}

float MPL3115_ReadAltitude(void)
{
  uint8_t statu;
  uint8_t msb = 0, csb = 0, lsb = 0;
	float altitude;
  
  statu = MPL3115_ReadStatus();

  if(statu & 0x08)	//02
  {
		MPL3115_Read_Buffer(OUT_P_MSB, 1, &msb, 1);
		MPL3115_Read_Buffer(OUT_P_CSB, 1, &csb, 1);
		MPL3115_Read_Buffer(OUT_P_LSB, 1, &lsb, 1);

		//(msb << 24 | csb << 16 | lsb << 8)/ 65536	
		altitude = (float)(( msb << 24 | csb << 16 | lsb << 8) / 65536.00); 
		
		return altitude;
  }
	return altitude;
}


float MPL3115_ReadTemp(void)
{
  uint8_t statu;
  uint8_t t_msb = 0, t_lsb = 0;
  float temper;
  
	statu = MPL3115_ReadStatus();
  if(statu & 0x08)
  {		
		MPL3115_Read_Buffer(OUT_T_MSB, 1, &t_msb, 1);
		MPL3115_Read_Buffer(OUT_T_LSB, 1, &t_lsb, 1);

		temper = (float)(t_msb + (float)(t_lsb>>4)/16);
			
    return(temper);
  }
  return 0;
}


uint8_t MPL3115_ReadID(void)
{
	uint8_t device_id = 1;
	
	MPL3115_Read_Buffer(WHO_AM_I, 1, &device_id, 1);
	
	return device_id;
}

void MPL3115_IO_Init(uint32_t address)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	
//		/* Configure data ready gpio */
//	GPIO_InitStruct.Pin = GPIO_PIN_6;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	I2Cx_Init(&mpl3115, address);
}

I2C_HandleTypeDef *Get_MPL3115_Handler(void)
{
	return &mpl3115;
}

void MPL3115IntoSleep(void)
{
  uint8_t   value;
  value = 0;
  MPL3115_Write_Buffer(CTRL_REG1, &value, 1);
  value = 0x0F;
  MPL3115_Write_Buffer(CTRL_REG2, &value, 1);
  value = 0x55;
  MPL3115_Read_Buffer(CTRL_REG1, 1, &value, 1);
  value = 0x56;
  MPL3115_Read_Buffer(CTRL_REG2, 1, &value, 1);
  value = 0x57;
  MPL3115_Read_Buffer(CTRL_REG3, 1, &value, 1);
}

