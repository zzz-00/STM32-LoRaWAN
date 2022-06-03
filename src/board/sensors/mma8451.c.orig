/*
** file:	mma8451.c
** data:	2016-8-31 23:13:16
** author:	eric.xu @ lierda
*/

#include "mma8451.h"
#include "hdc1000.h"
#include "common.h"

extern int8_t Error_num;

I2C_HandleTypeDef mma8451;


#define MMA8451_Delay(x)	HAL_Delay(x)


static void MMA8451_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
	I2C_HandleTypeDef *MMA8451_Handle = Get_MMA8451_Handler();
	/* device address | register address | register value */
	uint8_t tBuf[2] = {0};
	
	tBuf[0] = addr;
	tBuf[1] = *buffer;
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Master_Transmit(MMA8451_Handle, MMA8451_WRITE_MODE, tBuf, 2, 1000) != HAL_OK)
	{
		if(time_out_break_ms(500) == 1)
		{
			Error_num = -19;
			break;
		}
	}
	
}

static void MMA8451_Read_Buffer(uint8_t addr, uint8_t addr_len, uint8_t *buffer, uint8_t buff_len)
{
	I2C_HandleTypeDef *MMA8451_Handle = Get_MMA8451_Handler();
	
	timeout_start_flag = true;
	
	while(HAL_I2C_Mem_Read(MMA8451_Handle, MMA8451_DEVICE_ADDRESS, addr, addr_len, buffer, buff_len, 1000) != HAL_OK)
	{
		if(time_out_break_ms(500) == 1)
		{
			Error_num = -19;
			break;
		}
	}
}

void MMA8451_Init(void)
{
	uint8_t cfg, data_cfg;
	
	MMA8451_IO_Init(MMA8451_DEVICE_ADDRESS);
	
	MMA8451_Read_Buffer(M_CTRL_REG1, 1, &cfg, 1);
	
	cfg &= ~ACTIVE_MASK;	//standby
	MMA8451_Write_Buffer(M_CTRL_REG1, &cfg, 1);

	MMA8451_Read_Buffer(XYZ_DATA_CFG, 1, &data_cfg, 1);
	data_cfg |= 0x1;	//set 4g mode
	
	MMA8451_Write_Buffer(XYZ_DATA_CFG, &data_cfg, 1);
	
	cfg |= ACTIVE_MASK;		//active 
	MMA8451_Write_Buffer(M_CTRL_REG1, &cfg, 1);	

	HAL_Delay(10);
	
}

int16_t MMA8451_CalcAcceleration(int16_t data)
{
	int16_t temp;
	
	if(data < 0x200)
	{
		temp = (int16_t)(data*0.0078*100);	
	} else {
		data = data - 0x200;
		temp = (int16_t)((-4 + data*0.0078)*100);
	}
	
	return temp;
}

ACCELER_T MMA8451_ReadAcceleration(void)
{
	uint8_t statu;
	ACCELER_T accel = {0};
	uint8_t msb = 0, lsb = 0;
	int16_t accel_x, accel_y, accel_z;
	
	MMA8451_Read_Buffer(STATUS, 1, &statu, 1);
	
	if (statu & 0xf)
	{		
		MMA8451_Read_Buffer(OUT_X_MSB, 1, &msb, 1);
		MMA8451_Read_Buffer(OUT_X_LSB, 1, &lsb, 1);
		accel_x = ((msb << 8) | lsb ) >> 6; 
		
//		Uart_Printf("MMA8451 Acceleration OUT_X_MSB: 0x%x\r\n", msb);
//		Uart_Printf("MMA8451 Acceleration OUT_X_LSB: 0x%x\r\n", lsb);
		
		MMA8451_Read_Buffer(OUT_Y_MSB, 1, &msb, 1);
		MMA8451_Read_Buffer(OUT_Y_LSB, 1, &lsb, 1);
		accel_y = ((msb << 8) | lsb ) >> 6; 
//		Uart_Printf("MMA8451 Acceleration OUT_Y_MSB: 0x%x\r\n", msb);
//		Uart_Printf("MMA8451 Acceleration OUT_Y_LSB: 0x%x\r\n", lsb);	
		
		
		MMA8451_Read_Buffer(OUT_Z_MSB, 1, &msb, 1);
		MMA8451_Read_Buffer(OUT_Z_LSB, 1, &lsb, 1);
		accel_z = ((msb << 8) | lsb ) >> 6; 
//		Uart_Printf("MMA8451 Acceleration OUT_Z_MSB: 0x%x\r\n", msb);
//		Uart_Printf("MMA8451 Acceleration OUT_Z_LSB: 0x%x\r\n", lsb);
		
		/* uint 7.8mg, equals to 0.0078g */
//		accel.accel_x = (int16_t)(accel_x*0.78);
//		accel.accel_y = (int16_t)(accel_y*0.78);
//		accel.accel_z = (int16_t)(accel_z*0.78);
		accel.accel_x = MMA8451_CalcAcceleration(accel_x);
		accel.accel_y =	MMA8451_CalcAcceleration(accel_y);
		accel.accel_z =	MMA8451_CalcAcceleration(accel_z);
	}
	
	return accel;
}

void MMA8451_IO_Init(uint32_t address)
{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	I2Cx_Init(&mma8451, address);
}

I2C_HandleTypeDef *Get_MMA8451_Handler(void)
{
	return &mma8451;
}

void MMA8451IntoSleep(void)
{
    uint8_t cfg;
    cfg = 0;
    MMA8451_Write_Buffer(M_CTRL_REG1, &cfg, 1);
}

