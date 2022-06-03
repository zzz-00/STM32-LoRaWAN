/**
 * @mainpage		����
 * @author     	Lierda-WSN-LoRaWAN-TEAM
 * @version 		V1.0.3
 * @date 		2020-08-30
 *
 * LoRaWAN_TRAIN �ǻ���LoRaWANЭ��Ĵ��������ݴ���ʵ����룬��ʵ���ն�ʹ��������LoRaWAN��������ɣ��������ݰ���������ʹ�á���Ҫ�ļ�˵������Ҫ����˵��
 * - 1. ICAģ�鹤��ģʽ�빤��״̬���ƽӿ�
 * 	- ����״̬������״̬�л�
 * 	- ָ����͸��ģʽ���л�
 * - 2. ATָ�����ģ��������ýӿ�
 * - 3. �����ӿ�
 * - 4. ���ݷ��ͽӿ�
 * - 5. �����ݰ����ͽӿڣ���������ʵ���ն˵Ĳ�ְ�Э�飩
 *
 * @section application_arch 01 ģ��������Ӧ�ÿ��
 *
 * LoRaWAN ICA Node Driver����ʹ�÷�ʽ��
 * @image html LoRaWAN_ICA-Module-driver.png "Figure 1: Uart driver of ICA Node
 *
 * @section  LoRaWAN_ICA_Node_Driver API
 * ������ϸ��Ϣ����ο� @ref ICA-Driver
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#include "lorawan_node_driver.h"
#include "usart.h"
#include "dma.h"
#include "rtc.h"
#include "app.h"
#include "gpio.h"
#include "tim.h"
#include "i2c.h"
#include "hdc1000.h"
#include "opt3001.h"
#include "MPL3115.h"
#include "mma8451.h"
#include "ST7789v.h"
#include "XPT2046.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 *
 * @retval None
 */
int main(void)
{
    HAL_Init();

    /** ϵͳʱ������ */
    SystemClock_Config();

    /** �����ʼ�� */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_RTC_Init();

    /** ���ڳ�ʼ�� */
    MX_LPUART1_Init(9600);  // MCU��ģ����������
    MX_USART2_Init(115200); // MCU��PC��������
    MX_I2C1_Init();

    HAL_Delay(20);
    LPUART1_Clear_IT(); //����жϲ����������ж�
    USART2_Clear_IT();  //����жϲ����������ж�

    /** ��ʪ�ȴ����� */
    HDC1000_Init();

    /** ��ǿ������ */
    OPT3001_Init();

    /** ��ѹ������ */
    MPL3115_Init(MODE_BAROMETER);

    /** ���ٶȴ����� */
    MMA8451_Init();

    /** Һ�� */
    LCD_Init();

    /** ��λģ�� */
    HAL_Delay(500); //ģ���ϵ��ʼ��ʱ��
    Node_Hard_Reset();

    /** ��������Ϣ��ӡ */
    LoRaWAN_Borad_Info_Print();

    /* Infinite loop */
    /* USER CODE BEGIN WHILE  */
    while (1)
    {
        LoRaWAN_Func_Process();
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /* PWR CLOCK ENABLE*/
    __PWR_CLK_ENABLE();

    /**Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
    //	RCC_OscInitStruct.LSEState = RCC_LSE_ON ;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2; // PLLCLK = HSI*N/M/R = 80MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // select the PLL as the system clock source
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // AHB prescaler
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         // APB1 prescaler
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // APB2 prescaler

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_RTC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_HSI;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /**Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /**Configure the Systick interrupt time
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000); // 1ms �ж�һ��

    /**Configure the Systick
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  file: The file name as string.
 * @param  line: The line in file as a number.
 * @retval None
 */
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/