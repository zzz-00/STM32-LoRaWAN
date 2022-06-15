#include "func.h"
#include "usart.h"
#include "ST7789v.h"
#include "lorawan_node_driver.h"
#include <stdio.h>

void Device_Config(void)
{
    // 灭灯
    HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_SET);

    // 清除屏幕
    LCD_Clear(WHITE);
    LCD_ShowString(10, 10, "DevEui:009569000000F586", BLUE);

    // 配置为ClassA
    nodeCmdConfig("AT+CLASS=0");
    debug_printf("AT+CLASS=0\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // 终端上行数据为确认帧
    nodeCmdConfig("AT+CONFIRM=1");
    debug_printf("AT+CONFIRM=1\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // 关闭自动重传功能
    nodeCmdConfig("AT+NBTRIALS=1");
    debug_printf("AT+NBTRIALS=1\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // 开启信号质量输出模式
    nodeCmdConfig("AT+STATUS=2,2");
    debug_printf("AT+STATUS=2,2\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // 配置固定通信速率
    nodeCmdConfig("AT+DATARATE=3");
    debug_printf("AT+DATARATE=3\r\n");
    HAL_Delay(100);
    debug_printf("%s\r\n", UART_TO_LRM_RECEIVE_BUFFER);

    // 关闭自动速率调整
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

double Cal_success_rate(uint8_t data[], uint8_t cnt, uint8_t flag)
{
    double sum = 0;
    switch (flag)
    {
    case 0:
    {
        for (int i = 0; i < cnt; i++)
        {
            sum += data[i];
        }
        sum = sum / cnt * 1.0;
    }
    break;

    case 1:
    {
        for (int i = 0; i < 20; i++)
        {
            sum += data[i];
        }
        sum = sum / 20 * 1.0;
    }
    break;

    default:
        break;
    }
    return sum;
}

double Cal_average_RSSI(int data[], uint8_t cnt, uint8_t flag)
{
    double sum = 0;
    switch (flag)
    {
    case 0:
    {
        for (int i = 0; i < cnt; i++)
        {
            sum += data[i];
        }
        sum = sum / cnt * 1.0;
    }

    break;
    case 1:
    {
        for (int i = 0; i < 10; i++)
        {
            sum += data[i];
        }
        sum = sum / 10 * 1.0;
    }

    break;

    default:
        break;
    }
    return sum;
}

int LCD_Set_Scroll_Area(uint16_t tfa, uint16_t vsa, uint16_t bta)
{
    uint8_t data;

    if (tfa + vsa + bta != 320)
    {
        return -1;
    }

    LCD_WriteCommand(0x33);

    data = tfa >> 8;
    LCD_WriteData_8bit(data);
    data = tfa;
    LCD_WriteData_8bit(data);
    data = vsa >> 8;
    LCD_WriteData_8bit(data);
    data = vsa;
    LCD_WriteData_8bit(data);
    data = bta >> 8;
    LCD_WriteData_8bit(data);
    data = bta;
    LCD_WriteData_8bit(data);

    return 0;
}

void LCD_Set_Scroll_Start_Address(uint16_t vsp)
{

    LCD_WriteCommand(0x37);

    LCD_WriteData_16bit(vsp / 256);
    LCD_WriteData_16bit(vsp % 256);
}

