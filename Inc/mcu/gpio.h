/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l476xx.h"
#include "main.h"
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

//-------------Key1\Key2 Macro definition-----------------------------------------
#define Gpio_KEY1   GPIOC
#define Gpio_KEY2   GPIOE
#define Pin_KEY1    GPIO_PIN_13
#define Pin_KEY2    GPIO_PIN_6
#define GET_KEY1_LEVEL		    HAL_GPIO_ReadPin(Gpio_KEY1,Pin_KEY1)
#define GET_KEY2_LEVEL		    HAL_GPIO_ReadPin(Gpio_KEY2,Pin_KEY2)

//----------LED OF WAKE\MODE\BUSY\STAT\RST Macro definition-------------------------
#define LedGpio_D6		GPIOD
#define LedGpio_D7    	GPIOA
#define LedGpio_D8		GPIOB
#define LedGpio_D11		GPIOE
#define LedPin_D6  		GPIO_PIN_7	 
#define LedPin_D7  		GPIO_PIN_10	 
#define LedPin_D8 		GPIO_PIN_1
#define LedPin_D11 		GPIO_PIN_0

#define GET_LED_WAKE_LEVEL		HAL_GPIO_ReadPin(LedGpio_D6, LedPin_D6)
#define GET_LED_MODE_LEVEL		HAL_GPIO_ReadPin(LedGpio_D7,LedPin_D7)
#define GET_LED_BUSY_LEVEL		HAL_GPIO_ReadPin(LedGpio_D8,LedPin_D8)
#define GET_LED_STAT_LEVEL		HAL_GPIO_ReadPin(LedGpio_D11,LedPin_D11)

#define LEDWAKE_ON   HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_RESET)
#define LEDWAKE_OFF  HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET)

#define LEDMODE_ON   HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_RESET)
#define LEDMODE_OFF  HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET)

#define LEDBUSY_ON   HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_RESET)
#define LEDBUSY_OFF  HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET)

#define LEDSTAT_ON   HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_RESET)
#define LEDSTAT_OFF  HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_SET)

#define LEDALL_ON    

#define LEDALL_OFF   

//----------WAKE\MODE\BUSY\STAT\RST Macro definition-------------------------
#define Gpio_WAKE    GPIOC
#define Gpio_MODE    GPIOA
#define Gpio_BUSY    GPIOC
#define Gpio_STAT    GPIOC
#define Gpio_RST     GPIOA
#define Pin_WAKE     GPIO_PIN_3
#define Pin_MODE     GPIO_PIN_0
#define Pin_BUSY     GPIO_PIN_5
#define Pin_STAT     GPIO_PIN_4
#define Pin_RST      GPIO_PIN_1
		
/** 读取连接模块WAKE引脚的电平 */
#define GET_WAKE_LEVEL		HAL_GPIO_ReadPin(Gpio_WAKE, Pin_WAKE)

/** 读取连接模块MODE引脚的电平 */
#define GET_MODE_LEVEL		HAL_GPIO_ReadPin(Gpio_MODE, Pin_MODE)

/** 读取连接模块BUSY引脚的电平 */
#define GET_BUSY_LEVEL		HAL_GPIO_ReadPin(Gpio_BUSY, Pin_BUSY)

/** 读取连接模块STAT引脚的电平 */
#define GET_STAT_LEVEL		HAL_GPIO_ReadPin(Gpio_STAT, Pin_STAT)

/** 设置连接模块WAKE引脚的为高电平 */
#define SET_WAKE_HIGH		HAL_GPIO_WritePin(Gpio_WAKE, Pin_WAKE, GPIO_PIN_SET)

/** 设置连接模块WAKE引脚的为低电平 */
#define SET_WAKE_LOW		HAL_GPIO_WritePin(Gpio_WAKE, Pin_WAKE, GPIO_PIN_RESET)

/** 设置连接模块MODE引脚的为高电平 */
#define SET_MODE_HIGH		HAL_GPIO_WritePin(Gpio_MODE, Pin_MODE, GPIO_PIN_SET)

/** 设置连接模块MODE引脚的为低电平 */
#define SET_MODE_LOW		HAL_GPIO_WritePin(Gpio_MODE, Pin_MODE, GPIO_PIN_RESET)

/** 设置连接模块RESET引脚的为高电平 */
#define SET_RESET_HIGH		HAL_GPIO_WritePin(Gpio_RST, Pin_RST, GPIO_PIN_SET)

/** 设置连接模块RESET引脚的为低电平 */
#define SET_RESET_LOW		HAL_GPIO_WritePin(Gpio_RST, Pin_RST, GPIO_PIN_RESET)


void MX_GPIO_Init(void);
void gpio_into_sleep(void);
void gpio_into_wakeup(void);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
