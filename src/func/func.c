#include "func.h"
#include "usart.h"
#include "ST7789v.h"
#include "lorawan_node_driver.h"

void Device_Config(void)
{
    // ���
    HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_SET);

    // �����Ļ
    LCD_Clear(WHITE);

    // ����ΪClassA
    nodeCmdConfig("AT+CLASS=0");
    debug_printf("AT+CLASS=0\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // �ն���������Ϊȷ��֡
    nodeCmdConfig("AT+CONFIRM=1");
    debug_printf("AT+CONFIRM=1\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // �ر��Զ��ش�����
    nodeCmdConfig("AT+NBTRIALS=1");
    debug_printf("AT+NBTRIALS=1\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // �����ź��������ģʽ
    nodeCmdConfig("AT+STATUS=2,2");
    debug_printf("AT+STATUS=2,2\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // ���ù̶�ͨ������
    nodeCmdConfig("AT+DATARATE=3");
    debug_printf("AT+DATARATE=3\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // �ر��Զ����ʵ���
    nodeCmdConfig("AT+ADR=0");
    debug_printf("AT+ADR=0\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);
}

int my_atoi(uint8_t *str)
{
    char flag = 0;
    int sum = 0;

    while (*str == ' ')
    {
        str++;
    }

    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
        {
            flag = 1;
        }
        str++;
    }

    while (*str >= '0' && *str <= '9')
    {
        sum = sum * 10 + *str - '0';
        str++;
    }

    return sum * (flag ? -1 : 1);
}
