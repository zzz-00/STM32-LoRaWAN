/**
******************************************************************************
* @file    stm32l4xx_it.c
* @brief   Interrupt Service Routines.
******************************************************************************
*
* COPYRIGHT(c) 2017 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal_rtc.h"

/* USER CODE BEGIN 0 */
#include "usart.h"


extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef TIM6_InitStruct;
//extern RTC_HandleTypeDef hrtc;
//extern TIM_HandleTypeDef  TimHandle;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_lpuart_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern RTC_HandleTypeDef hrtc;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
    
    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */
    
    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
    }
    /* USER CODE BEGIN HardFault_IRQn 1 */
    
    /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */
    
    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
    }
    /* USER CODE BEGIN MemoryManagement_IRQn 1 */
    
    /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */
    
    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
    }
    /* USER CODE BEGIN BusFault_IRQn 1 */
    
    /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */
    
    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
    }
    /* USER CODE BEGIN UsageFault_IRQn 1 */
    
    /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn 0 */
    
    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */
    
    /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */
    
    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */
    
    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */
    
    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */
    
    /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */
    
    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
    /* USER CODE BEGIN SysTick_IRQn 1 */
    
    /* USER CODE END SysTick_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_InitStruct);
}
/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/
/**
* @brief This function handles EXTI line 4 to 15 interrupts.
*/

void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */

  /* USER CODE END EXTI4_15_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

void EXTI4_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_15_IRQn 0 */
    
    /* USER CODE END EXTI4_15_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    /* USER CODE BEGIN EXTI4_15_IRQn 1 */
    
    /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
* @brief This function handles EXTI line 5 to 9 interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_15_IRQn 0 */
    
    /* USER CODE END EXTI4_15_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6); //KEY2
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5); //BUSY
    /* USER CODE BEGIN EXTI4_15_IRQn 1 */
    
    /* USER CODE END EXTI4_15_IRQn 1 */
}


/**
* @brief This function handles EXTI line 4 to 15 interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_15_IRQn 0 */
    
    /* USER CODE END EXTI4_15_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);  //KEY1
    /* USER CODE BEGIN EXTI4_15_IRQn 1 */
    
    /* USER CODE END EXTI4_15_IRQn 1 */
}


/**
* @brief This function handles DMA1 channel5 global interrupt.
*/
void DMA1_Channel5_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */
    
    /* USER CODE END DMA1_Channel5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
    /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */
    
    /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel6 global interrupt.
*/
void DMA1_Channel6_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel6_IRQn 0 */
    
    /* USER CODE END DMA1_Channel6_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart2_rx);
    /* USER CODE BEGIN DMA1_Channel6_IRQn 1 */
    
    /* USER CODE END DMA1_Channel6_IRQn 1 */
}

/**
* @brief This function handles DMA2 channel7 global interrupt.
*/
void DMA2_Channel7_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Channel7_IRQn 0 */
    
    /* USER CODE END DMA2_Channel7_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_lpuart_rx);
    /* USER CODE BEGIN DMA2_Channel7_IRQn 1 */
    
    /* USER CODE END DMA2_Channel7_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    Usart1Receive_IDLE();
    
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    
    /* USER CODE END USART1_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
    /* USER CODE BEGIN USART2_IRQn 0 */
    usart2_receive_idle(); 
    
    /* USER CODE END USART2_IRQn 0 */
    HAL_UART_IRQHandler(&huart2);
    /* USER CODE BEGIN USART2_IRQn 1 */
    
    /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles LPUART1 global interrupt.
*/
void LPUART1_IRQHandler(void)
{
    /* USER CODE BEGIN LPUART1_IRQn 0 */
    lpusart1_receive_idle();
    /* USER CODE END LPUART1_IRQn 0 */
    HAL_UART_IRQHandler(&hlpuart1);
    /* USER CODE BEGIN LPUART1_IRQn 1 */
    
    /* USER CODE END LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/**
* @brief  This function handles Flash interrupt request.                 //modify
* @param  None
* @retval None
*/
void FLASH_IRQHandler(void)
{
    HAL_FLASH_IRQHandler();
}


/**
* @brief  This function handles RTC Alarm interrupt request.
* @param  None
* @retval None
*/
void RTC_WKUP_IRQHandler(void)
{
  	HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
    //     HAL_RTCEx_WakeUpTimerEventCallback(&hrtc);
}



/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
