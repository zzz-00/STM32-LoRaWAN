#include <string.h>
#include <stdio.h>
#include "app.h"
#include "usart.h"
#include "gpio.h"
#include "lorawan_node_driver.h"
#include "hdc1000.h"
#include "sensors_test.h"
#include "func.h"
#include "tim.h"
#include "common.h"
#include "ST7789v.h"

extern DEVICE_MODE_T device_mode;
extern DEVICE_MODE_T *Device_Mode_str;
down_list_t *pphead = NULL;

uint8_t empty_data[9] = {0xAA, 0, 0, 0, 0, 0, 0, 0, 0x0F};
uint8_t correspond_flag = 0;
uint32_t tim6_cnt = 0;
uint32_t total_cnt = 0;
uint8_t send_cnt = 0;
uint8_t full_flag = 0;
uint8_t success_flag = 2;

uint8_t str[20] = {0};
int UpCnt = 0;
int DnCnt = 0;
double loss_tolerance = 0;
double correspond_success_rate = 0;
double average_RSSI = 0;
uint8_t loss_tolerance_s[30];
uint8_t correspond_success_rate_s[30];
uint8_t average_RSSI_s[30];
uint8_t RSSI[30];
uint8_t SNR[30];

recv_data signal_quality_monitor[20] = {0};

//-----------------Users application--------------------------
void LoRaWAN_Func_Process(void)
{
    static DEVICE_MODE_T dev_stat = NO_MODE;
    static uint8_t config_flag = 0;

    uint16_t temper = 0;

    switch ((uint8_t)device_mode)
    {
    /* 指令模式 */
    case CMD_CONFIG_MODE:
    {
        /* 如果不是command Configuration function, 则进入if语句,只执行一次 */
        if (dev_stat != CMD_CONFIG_MODE)
        {
            dev_stat = CMD_CONFIG_MODE;
            debug_printf("\r\n[Command Mode]\r\n");

            nodeGpioConfig(wake, wakeup);
            nodeGpioConfig(mode, command);
        }
        /* 等待usart2产生中断 */
        if (UART_TO_PC_RECEIVE_FLAG)
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            lpusart1_send_data(UART_TO_PC_RECEIVE_BUFFER, UART_TO_PC_RECEIVE_LENGTH);
        }
        /* 等待lpuart1产生中断 */
        if (UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);
        }
    }
    break;

    /* 透传模式 */
    case DATA_TRANSPORT_MODE:
    {
        /* 如果不是data transport function,则进入if语句,只执行一次 */
        if (dev_stat != DATA_TRANSPORT_MODE)
        {
            dev_stat = DATA_TRANSPORT_MODE;
            debug_printf("\r\n[Transperant Mode]\r\n");

            /* 模块入网判断 */
            if (nodeJoinNet(JOIN_TIME_120_SEC) == false)
            {
                return;
            }

            temper = HDC1000_Read_Temper() / 1000;

            nodeDataCommunicate((uint8_t *)&temper, sizeof(temper), &pphead);
        }

        /* 等待usart2产生中断 */
        if (UART_TO_PC_RECEIVE_FLAG && GET_BUSY_LEVEL) // Ensure BUSY is high before sending data
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            nodeDataCommunicate((uint8_t *)UART_TO_PC_RECEIVE_BUFFER, UART_TO_PC_RECEIVE_LENGTH, &pphead);
        }

        /* 如果模块正忙, 则发送数据无效，并给出警告信息 */
        else if (UART_TO_PC_RECEIVE_FLAG && (GET_BUSY_LEVEL == 0))
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            debug_printf("--> Warning: Don't send data now! Module is busy!\r\n");
        }

        /* 等待lpuart1产生中断 */
        if (UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);
        }
    }
    break;

    /*工程模式*/
    case PRO_TRAINING_MODE:
    {
        /* 如果不是Class C云平台数据采集模式, 则进入if语句,只执行一次 */
        if (dev_stat != PRO_TRAINING_MODE)
        {
            dev_stat = PRO_TRAINING_MODE;
            debug_printf("\r\n[Project Mode]\r\n");
        }

        /* 你的实验代码位置 */
        if (config_flag == 0)
        {
            Device_Config();
            /* 模块入网判断 */
            if (nodeJoinNet(JOIN_TIME_120_SEC) == false)
            {
                return;
            }
            Tim6_Conf(100);
            HAL_TIM_Base_Start_IT(&htim6);
            config_flag = 1;
        }

        /* 等待lpuart1产生中断 */
        if (UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            LCD_Fill(10, 50, 240, 149, WHITE);
            correspond_flag = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);
            HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);

            /* 下行数据处理 */
            match_string(UART_TO_LRM_RECEIVE_BUFFER, "UpCnt:", ",", str); // 获取UpCnt
            if (str != NULL)
            {
                UpCnt = my_atoi(str);
                debug_printf("UpCnt:%d\r\n", UpCnt);
            }
            memset(str, 0, sizeof(str));

            match_string(UART_TO_LRM_RECEIVE_BUFFER, "DnCnt:", ",", str); // 获取DnCnt
            if (str != NULL)
            {
                DnCnt = my_atoi(str);
                debug_printf("DnCnt:%d\r\n", DnCnt);
            }
            memset(str, 0, sizeof(str));

            match_string(UART_TO_LRM_RECEIVE_BUFFER, "pRSSI:", ",", str); // 获取RSSI
            if (str != NULL)
            {
                signal_quality_monitor[send_cnt - 1].rssi = my_atoi(str);
                debug_printf("RSSI:%d\r\n", signal_quality_monitor[send_cnt - 1].rssi);
            }
            sprintf((char *)RSSI, "RSSI:%d", signal_quality_monitor[send_cnt - 1].rssi);
            LCD_ShowString(10, 50, RSSI, BLUE);
            memset(str, 0, sizeof(str));

            match_string(UART_TO_LRM_RECEIVE_BUFFER, "SNR:", ",", str); // 获取SNR
            if (str != NULL)
            {
                signal_quality_monitor[send_cnt - 1].snr = my_atoi(str);
                debug_printf("SNR:%d\r\n", signal_quality_monitor[send_cnt - 1].snr);
            }
            sprintf((char *)SNR, "SNR:%d", signal_quality_monitor[send_cnt - 1].snr);
            LCD_ShowString(10, 70, SNR, BLUE);
            memset(str, 0, sizeof(str));

            success_flag = 1;
        }

        /* 丢包率计算与显示 */
        loss_tolerance = (UpCnt - DnCnt) / UpCnt * 1.00;
        if (loss_tolerance > 0.1)
        {
            HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_RESET);
        }
        sprintf((char *)loss_tolerance_s, "Loss_tolerance:%.2f%%", loss_tolerance * 100.0);
        LCD_ShowString(10, 90, loss_tolerance_s, BLUE);

        if (send_cnt > 0)
        {
            /* 通信成功率计算与显示 */
            signal_quality_monitor[send_cnt - 1].state = 1;
            correspond_success_rate = Cal_success_rate(signal_quality_monitor, send_cnt, full_flag);
            sprintf((char *)correspond_success_rate_s, "Success rate:%.2f%%", correspond_success_rate * 100.0);
            LCD_ShowString(10, 110, correspond_success_rate_s, BLUE);

            /* 平均RSSI计算与显示 */
            average_RSSI = Cal_average_RSSI(signal_quality_monitor, send_cnt, full_flag);
            sprintf((char *)average_RSSI_s, "Average RSSI:%.2f", average_RSSI);
            LCD_ShowString(10, 130, average_RSSI_s, BLUE);
        }

        if (success_flag == 1)
        {
            LCD_ShowString(10, 30, "Communication succeeded", BLUE);
        }
        else if (success_flag == 0)
        {
            LCD_ShowString(10, 30, "Communication failed", BLUE);
        }
        else
        {
            LCD_Fill(10, 30, 240, 49, WHITE);
        }
    }
    break;

    default:
        break;
    }
}

/**
 * @brief   开发板版本信息和按键使用说明信息打印
 * @details 上电所有灯会短暂亮100ms
 * @param   无
 * @return  无
 */
void LoRaWAN_Borad_Info_Print(void)
{
    debug_printf("\r\n\r\n");
    PRINT_CODE_VERSION_INFO("%s", CODE_VERSION);
    debug_printf("\r\n");
    debug_printf("--> Press Key1 to: \r\n");
    debug_printf("-->  - Enter command Mode\r\n");
    debug_printf("-->  - Enter Transparent Mode\r\n");
    debug_printf("--> Press Key2 to: \r\n");
    debug_printf("-->  - Enter Project Trainning Mode\r\n");
    LEDALL_ON;
    HAL_Delay(100);
    LEDALL_OFF;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *tim_BaseHandle)
{
    if (tim_BaseHandle->Instance == TIM6)
    {
        tim6_cnt++;
        if (tim6_cnt == 60)
        {
            if (correspond_flag == 0)
            {
                debug_printf("send data\r\n");
                lpusart1_send_data(empty_data, sizeof(empty_data));
                HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_RESET);
                correspond_flag = 1;
                success_flag = 2;
                if (send_cnt < 20)
                {
                    send_cnt++;
                }
                else
                {
                    send_cnt = 1;
                    full_flag = 1; // 计满20次标志位
                }
                debug_printf("%d\r\n", send_cnt);
                total_cnt++;
                tim6_cnt = 0;
            }
        }
    }
}
