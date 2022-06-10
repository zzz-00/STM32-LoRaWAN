#include <string.h>
#include "app.h"
#include "usart.h"
#include "gpio.h"
#include "lorawan_node_driver.h"
#include "hdc1000.h"
#include "sensors_test.h"
#include "func.h"
#include "tim.h"
#include "common.h"

extern DEVICE_MODE_T device_mode;
extern DEVICE_MODE_T *Device_Mode_str;
down_list_t *pphead = NULL;

uint8_t empty_data[9] = {0xAA, 0, 0, 0, 0, 0, 0, 0, 0x0F};
uint8_t correspond_flag = 0;
uint32_t tim6_cnt = 0;
uint8_t send_cnt = 0;
uint8_t full_flag = 0;

uint8_t str[20] = {0};
double loss_tolerance = 0;

recv_data signal_quality_monitor[20] = {0};

//-----------------Users application--------------------------
void LoRaWAN_Func_Process(void)
{
    static DEVICE_MODE_T dev_stat = NO_MODE;
    static uint8_t config_flag = 0;

    uint16_t temper = 0;

    switch ((uint8_t)device_mode)
    {
    /* ָ��ģʽ */
    case CMD_CONFIG_MODE:
    {
        /* �������command Configuration function, �����if���,ִֻ��һ�� */
        if (dev_stat != CMD_CONFIG_MODE)
        {
            dev_stat = CMD_CONFIG_MODE;
            debug_printf("\r\n[Command Mode]\r\n");

            nodeGpioConfig(wake, wakeup);
            nodeGpioConfig(mode, command);
        }
        /* �ȴ�usart2�����ж� */
        if (UART_TO_PC_RECEIVE_FLAG)
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            lpusart1_send_data(UART_TO_PC_RECEIVE_BUFFER, UART_TO_PC_RECEIVE_LENGTH);
        }
        /* �ȴ�lpuart1�����ж� */
        if (UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);
        }
    }
    break;

    /* ͸��ģʽ */
    case DATA_TRANSPORT_MODE:
    {
        /* �������data transport function,�����if���,ִֻ��һ�� */
        if (dev_stat != DATA_TRANSPORT_MODE)
        {
            dev_stat = DATA_TRANSPORT_MODE;
            debug_printf("\r\n[Transperant Mode]\r\n");

            /* ģ�������ж� */
            if (nodeJoinNet(JOIN_TIME_120_SEC) == false)
            {
                return;
            }

            temper = HDC1000_Read_Temper() / 1000;

            nodeDataCommunicate((uint8_t *)&temper, sizeof(temper), &pphead);
        }

        /* �ȴ�usart2�����ж� */
        if (UART_TO_PC_RECEIVE_FLAG && GET_BUSY_LEVEL) // Ensure BUSY is high before sending data
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            nodeDataCommunicate((uint8_t *)UART_TO_PC_RECEIVE_BUFFER, UART_TO_PC_RECEIVE_LENGTH, &pphead);
        }

        /* ���ģ����æ, ����������Ч��������������Ϣ */
        else if (UART_TO_PC_RECEIVE_FLAG && (GET_BUSY_LEVEL == 0))
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            debug_printf("--> Warning: Don't send data now! Module is busy!\r\n");
        }

        /* �ȴ�lpuart1�����ж� */
        if (UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);
        }
    }
    break;

    /*����ģʽ*/
    case PRO_TRAINING_MODE:
    {
        /* �������Class C��ƽ̨���ݲɼ�ģʽ, �����if���,ִֻ��һ�� */
        if (dev_stat != PRO_TRAINING_MODE)
        {
            dev_stat = PRO_TRAINING_MODE;
            debug_printf("\r\n[Project Mode]\r\n");
        }

        /* ���ʵ�����λ�� */
        if (config_flag == 0)
        {
            Device_Config();
            /* ģ�������ж� */
            if (nodeJoinNet(JOIN_TIME_120_SEC) == false)
            {
                return;
            }
            Tim6_Conf(100);
            HAL_TIM_Base_Start_IT(&htim6);
            config_flag = 1;
        }

        /* �ȴ�lpuart1�����ж� */
        if (UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            correspond_flag = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER, UART_TO_LRM_RECEIVE_LENGTH);
            HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);

            /* �������ݴ��� */
            match_string(UART_TO_LRM_RECEIVE_BUFFER, "UpCnt:", ",", str); // ��ȡUpCnt
            if (str != NULL)
            {
                signal_quality_monitor[send_cnt - 1].UpCnt = my_atoi(str);
                debug_printf("UpCnt:%d\r\n", signal_quality_monitor[send_cnt - 1].UpCnt);
            }
            memset(str, 0, sizeof(str));

            match_string(UART_TO_LRM_RECEIVE_BUFFER, "DnCnt:", ",", str); // ��ȡDnCnt
            if (str != NULL)
            {
                signal_quality_monitor[send_cnt - 1].DnCnt = my_atoi(str);
                debug_printf("DnCnt:%d\r\n", signal_quality_monitor[send_cnt - 1].DnCnt);
            }
            memset(str, 0, sizeof(str));

            match_string(UART_TO_LRM_RECEIVE_BUFFER, "pRSSI:", ",", str); // ��ȡRSSI
            if (str != NULL)
            {
                signal_quality_monitor[send_cnt - 1].rssi = my_atoi(str);
                debug_printf("RSSI:%d\r\n", signal_quality_monitor[send_cnt - 1].rssi);
            }
            memset(str, 0, sizeof(str));

            match_string(UART_TO_LRM_RECEIVE_BUFFER, "SNR:", ",", str); // ��ȡSNR
            if (str != NULL)
            {
                signal_quality_monitor[send_cnt - 1].snr = my_atoi(str);
                debug_printf("SNR:%d\r\n", signal_quality_monitor[send_cnt - 1].snr);
            }
            memset(str, 0, sizeof(str));

            loss_tolerance = (signal_quality_monitor[send_cnt - 1].UpCnt - signal_quality_monitor[send_cnt - 1].DnCnt) / signal_quality_monitor[send_cnt - 1].UpCnt * 1.00; // �����ʼ���
            if (loss_tolerance > 0.1)
            {
                HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_RESET);
            }
            debug_printf("loss tolerance:%.2f\r\n", loss_tolerance);
            signal_quality_monitor[send_cnt - 1].success_flag = 1;
        }
    }
    break;

    default:
        break;
    }
}

/**
 * @brief   ������汾��Ϣ�Ͱ���ʹ��˵����Ϣ��ӡ
 * @details �ϵ����еƻ������100ms
 * @param   ��
 * @return  ��
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
                if (send_cnt < 20)
                {
                    send_cnt++;
                }
                else
                {
                    send_cnt = 1;
                    full_flag = 1; // ����20�α�־λ
                }
                debug_printf("%d\r\n", send_cnt);
                tim6_cnt = 0;
            }
        }
    }
}
