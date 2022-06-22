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
uint64_t UpCnt = 0;
uint64_t DnCnt = 0;
uint32_t tim6_cnt = 0;
uint8_t RSSI_cnt = 0;
uint8_t correspond_cnt = 0;
uint8_t cycle_cnt = 0;
uint8_t SNR_cnt = 0;

uint8_t RSSI_full_flag = 0;
uint8_t correspond_full_flag = 0; // 通信20次标志位
uint8_t success_flag = 3;         // 通信标志位
uint8_t SNR_flag = 0;
uint8_t LCD_mode_change_flag = 0;
uint8_t LineChart_Update_flag = 0;
uint8_t Map_Update_flag = 0;

uint8_t str_select = 0;
uint8_t str_select_display = 1;

uint8_t str[20] = {0};
int RSSI[10] = {0};
int SNR = 0;
uint8_t correspond_state[20] = {0};

double loss_tolerance = 0;
double correspond_success_rate = 0;
double average_RSSI = 0;

uint8_t LCD_mode = SCROLL_MODE;
uint8_t s_loss_tolerance[30];
uint8_t s_correspond_success_rate[30];
uint8_t s_average_RSSI[30];
uint8_t s_RSSI[30];
uint8_t s_SNR[30];
uint8_t s_scroll_correspond_0[10][30] = {0}; // 滚动显示字符串数组
uint8_t s_scroll_correspond_1[10][30] = {0};

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

            /* 通信计数 */
            if (correspond_cnt < 20)
            {
                correspond_cnt++;
            }
            else
            {
                correspond_cnt = 1;
                cycle_cnt++;
                correspond_full_flag = 1; // 计满20次标志位
            }
            if (correspond_cnt < 11)
            {
                str_select = 0;
            }
            else
            {
                str_select = 1;
            }
            UpCnt++;

            // usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);

            if (find_string(UART_TO_LRM_RECEIVE_BUFFER, "DN") != NULL) // 检测到下行数据
            {
                /* RSSI计数 */
                if (RSSI_cnt < 10)
                {
                    RSSI_cnt++;
                }
                else
                {
                    RSSI_cnt = 1;
                    // LineChart_Update_flag = 1;
                    RSSI_full_flag = 1; // 计满10次标志位
                }

                /* 下行数据处理 */
                match_string(UART_TO_LRM_RECEIVE_BUFFER, "pRSSI:", ",", str); // 获取RSSI
                if (str[0] != '\0')
                {
                    RSSI[RSSI_cnt - 1] = my_atoi(str);
                    if (LCD_mode == MAP)
                    {
                        Map_Update_flag = 1;
                    }
                    // debug_printf("RSSI:%d\r\n", RSSI[RSSI_cnt - 1]);
                }
                sprintf((char *)s_RSSI, "RSSI:%d", RSSI[RSSI_cnt - 1]);
                memset(str, 0, sizeof(str));

                match_string(UART_TO_LRM_RECEIVE_BUFFER, "SNR:", ",", str); // 获取SNR
                if (str[0] != '\0')
                {
                    SNR = my_atoi(str);
                    if (SNR < 3)
                    {
                        SNR_flag = 1;
                    }
                    // debug_printf("SNR:%d\r\n", SNR);
                }
                sprintf((char *)s_SNR, "SNR:%d", SNR);
                memset(str, 0, sizeof(str));

                DnCnt++;

                correspond_state[correspond_cnt - 1] = 1;
                switch (str_select)
                {
                case 0:
                    sprintf((char *)s_scroll_correspond_0[(correspond_cnt - 1) % 10], "%d Communication succeeded", correspond_cnt + 20 * cycle_cnt);
                    break;
                case 1:
                    sprintf((char *)s_scroll_correspond_1[(correspond_cnt - 1) % 10], "%d Communication succeeded", correspond_cnt + 20 * cycle_cnt);
                    break;

                default:
                    break;
                }
                LineChart_Update_flag = 1;
                success_flag = 1; // 本次通信成功
            }
            else // 无下行数据
            {
                correspond_state[correspond_cnt - 1] = 0;
                switch (str_select)
                {
                case 0:
                    sprintf((char *)s_scroll_correspond_0[(correspond_cnt - 1) % 10], "%d Communication failed", correspond_cnt + 20 * cycle_cnt);
                    break;
                case 1:
                    sprintf((char *)s_scroll_correspond_1[(correspond_cnt - 1) % 10], "%d Communication failed", correspond_cnt + 20 * cycle_cnt);
                    break;

                default:
                    break;
                }
                sprintf((char *)s_RSSI, "RSSI:---");
                sprintf((char *)s_SNR, "SNR:---");
                success_flag = 0;
            }
            HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);
        }

        /* 通信成功率计算 */
        if (correspond_cnt > 0)
        {
            correspond_success_rate = Cal_success_rate(correspond_state, correspond_cnt, correspond_full_flag);
            sprintf((char *)s_correspond_success_rate, "Success rate:%.2f%%", correspond_success_rate * 100.0);
        }

        /* 丢包率计算 */
        if (UpCnt > 0)
        {
            loss_tolerance = (UpCnt - DnCnt) * 1.0 / UpCnt;
            sprintf((char *)s_loss_tolerance, "Loss tolerance:%.2f%%", loss_tolerance * 100.0);
            if (loss_tolerance > 0.1)
            {
                HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET);
            }
        }

        /* 平均RSSI计算 */
        if (RSSI_cnt > 0)
        {
            average_RSSI = Cal_average_RSSI(RSSI, RSSI_cnt, RSSI_full_flag);
            sprintf((char *)s_average_RSSI, "Average RSSI:%.2f", average_RSSI);
        }

        /* LCD显示 */
        static int i = 149;
        static int display_cnt = 0;
        switch (LCD_mode)
        {
        case SCROLL_MODE:
        {
            if (LCD_mode_change_flag == 1)
            {
                LCD_Fill(0, 150, 240, 320, WHITE);
                if (s_scroll_correspond_1[0][0] == '\0')
                {
                    for (int i = 0; i < correspond_cnt; i++)
                    {
                        LCD_ShowString(10, 150 + 16 * i, s_scroll_correspond_0[i], BLUE);
                    }
                }
                LCD_mode_change_flag = 0;
            }

            if (s_scroll_correspond_1[0][0] != '\0')
            {
                LCD_Set_Scroll_Area(150, 160, 10);
            }

            LCD_Information_display(success_flag);

            if (correspond_cnt > 0 && correspond_cnt < 11 && correspond_full_flag == 0)
            {
                LCD_ShowString(10, 150 + 16 * (correspond_cnt - 1), s_scroll_correspond_0[(correspond_cnt - 1) % 10], BLUE);
            }
            else
            {
                if ((i - 150) % 16 == 8)
                {
                    LCD_Fill(10, 150 + 16 * (display_cnt % 10), 240, 150 + 16 * (display_cnt % 10 + 1) - 1, WHITE);
                    switch (str_select_display)
                    {
                    case 0:
                        LCD_ShowString(10, 150 + 16 * (display_cnt % 10), s_scroll_correspond_0[display_cnt % 10], BLUE);
                        break;
                    case 1:
                        LCD_ShowString(10, 150 + 16 * (display_cnt % 10), s_scroll_correspond_1[display_cnt % 10], BLUE);
                        break;

                    default:
                        break;
                    }
                    display_cnt++;

                    if (display_cnt == 10)
                    {
                        str_select_display = !str_select_display;
                        display_cnt = 0;
                    }
                }
            }
            if (s_scroll_correspond_1[0][0] != '\0' || correspond_full_flag == 1)
            {
                LCD_Set_Scroll_Start_Address(i);
                if (++i > 309)
                {
                    i = 149;
                }
            }
        }
        break;

        case DYNAMIC_CURVE:
        {
            if (LCD_mode_change_flag == 1)
            {
                LCD_Set_Scroll_Area(320, 0, 0);
                Draw_background();
                LCD_mode_change_flag = 0;
            }

            LCD_Information_display(success_flag);

            if (LineChart_Update_flag)
            {
                Draw_LineChart(RSSI);
                LineChart_Update_flag = 0;
            }
        }
        break;

        case MAP:
        {
            if (LCD_mode_change_flag == 1)
            {
                LCD_Fill(0, 150, 240, 320, WHITE);
                init_rand(HAL_GetTick());
                LCD_mode_change_flag = 0;
            }

            LCD_Information_display(success_flag);

            if (Map_Update_flag)
            {
                Map(RSSI[RSSI_cnt - 1]);
                Map_Update_flag = 0;
            }
        }
        break;

        default:
            break;
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
        if (SNR_flag)
        {
            SNR_cnt++;
            HAL_GPIO_TogglePin(LedGpio_D11, LedPin_D11);
            if (SNR_cnt > 4)
            {
                SNR_cnt = 0;
                SNR_flag = 0;
                HAL_GPIO_TogglePin(LedGpio_D11, LedPin_D11);
                HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_SET);
            }
        }
        if (tim6_cnt == 60)
        {
            if (success_flag != 2)
            {
                debug_printf("send data\r\n");
                lpusart1_send_data(empty_data, sizeof(empty_data));
                HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_RESET);
                success_flag = 2;
            }
            tim6_cnt = 0;
        }
    }
}

void LCD_Information_display(uint8_t flag)
{
    if (flag == 1)
    {
        LCD_ShowString(10, 30, "Communication succeeded", BLUE);
    }
    else if (flag == 0)
    {
        LCD_ShowString(10, 30, "Communication failed", BLUE);
    }
    else
    {
        LCD_Fill(10, 30, 240, 49, WHITE);
    }
    LCD_ShowString(10, 50, s_RSSI, BLUE);
    LCD_ShowString(10, 70, s_SNR, BLUE);
    LCD_ShowString(10, 90, s_loss_tolerance, BLUE);
    LCD_ShowString(10, 110, s_correspond_success_rate, BLUE);
    LCD_ShowString(10, 130, s_average_RSSI, BLUE);
}
