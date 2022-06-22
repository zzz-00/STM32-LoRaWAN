#include "func.h"
#include "usart.h"
#include "ST7789v.h"
#include "lorawan_node_driver.h"
#include <stdio.h>
#include <stdlib.h>

void Device_Config(void)
{
    // 灭灯
    HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_SET);

    // 清除屏幕
    LCD_Clear(WHITE);

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

    // 获取设备DevEui
    uint8_t str[30] = {0};
    uint8_t dsp[30] = "DevEui:";
    int j = 7;
    nodeCmdConfig("AT+DEVEUI?");
    debug_printf("AT+DEVEUI?\r\n");
    HAL_Delay(100);
    match_string(UART_TO_LRM_RECEIVE_BUFFER, "DEVEUI:", "OK", str);
    if (str[0] != '\0')
    {
        for (int i = 0; i < sizeof(str); i++)
        {
            if (str[i] != 0x20 && str[i] != 0x0D && str[i] != 0x0A)
            {
                dsp[j] = str[i];
                j++;
            }
        }
    }
    dsp[j] = '\0';
    debug_printf("%s\r\n", dsp);
    LCD_ShowString(10, 10, dsp, BLUE);
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

void Draw_background(void)
{
    LCD_Fill(0, 150, 240, 320, WHITE);
    LCD_DrawRectangle(35, 150, 235, 300, BLUE);
    // 画列点
    for (uint16_t x = 85; x < 235; x += 50)
    {
        for (uint16_t y = 150; y < 300; y += 10)
        {
            LCD_DrawPoint(x, y, 1, BLUE);
        }
    }
    // 画行点
    for (uint16_t y = 200; y < 300; y += 50)
    {
        for (uint16_t x = 35; x < 235; x += 10)
        {
            LCD_DrawPoint(x, y, 1, BLUE);
        }
    }
    LCD_ShowString(2, 150, "---", BLUE);
    LCD_ShowString(2, 196, "---", BLUE);
    LCD_ShowString(2, 246, "---", BLUE);
    LCD_ShowString(2, 296, "---", BLUE);
    LCD_ShowString(35, 301, "0", BLUE);
    LCD_ShowString(131, 301, "5", BLUE);
    LCD_ShowString(223, 301, "10", BLUE);
}

void Clear_Point(int num)
{
    uint16_t lie = 0;
    for (lie = 151; lie < 300; lie++)
    {
        LCD_DrawPoint(num, lie, 1, WHITE);
    }
    if (!((num - 35) % 50))
    {
        for (lie = 150; lie < 300; lie += 10)
        {
            LCD_DrawPoint(num, lie, 1, BLUE);
        }
    }
    if (!((num - 35) % 10))
    {
        for (lie = 200; lie < 300; lie += 50)
        {
            LCD_DrawPoint(num, lie, 1, BLUE);
        }
    }
}

void Draw_LineChart(int data[])
{
    uint8_t str[10];
    int max = data[0];
    int min = data[0];
    // float height = 0;
    float height = 0;
    int top = 0;
    int bottom = 0;
    int position = 150;
    int x = 0, y = 0, x0 = 55, y0 = 0;
    for (int i = 0; i < 10; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
        if (data[i] < min)
        {
            min = data[i];
        }
    }

    debug_printf("max:%d min:%d \r\n", max, min);
    top = max;
    bottom = min;
    height = -(150 / (top - bottom));
    LCD_Fill(0, 150, 34, 320, WHITE);
    sprintf((char *)str, "%d", top);
    LCD_ShowString(2, 150, str, BLUE);
    sprintf((char *)str, "%d", (int)(top + 50.0 / height * 1 - 0.5));
    LCD_ShowString(2, 196, str, BLUE);
    sprintf((char *)str, "%d", (int)(top + 50.0 / height * 2 - 0.5));
    LCD_ShowString(2, 246, str, BLUE);
    sprintf((char *)str, "%d", (int)(top + 50.0 / height * 3 - 0.5));
    LCD_ShowString(2, 296, str, BLUE);

    y0 = (int)(position + (data[0] - top) * height);
    debug_printf("%d %d %.2f\r\n", data[0], y0, height);
    for (int i = 36; i < 235; i++)
    {
        Clear_Point(i);
    }
    for (int i = 1; i < 10; i++)
    {
        x = 55 + i * 20;
        y = (int)(position + (data[i] - top) * height);
        LCD_DrawLine(x0, y0, x, y, BLUE);
        x0 = x;
        y0 = y;
    }
}

/*********************************************************************
 * @fn      init_rand
 *
 * @brief   Seed pseudo-random number generator
 *
 * @param   seed An integer value to be used as seed by the pseudo-random number generator
 *
 * @return  Error code
 */
uint32_t init_rand(uint32_t seed)
{
    // Seed the pseudo-random number generator
    prngState += seed;

    // Successful processing
    return SUCCESS;
}

/*********************************************************************
 * @fn      _rand
 *
 * @brief   Get a random value
 *
 * @param   void
 *
 * @return  Random value
 */
uint32_t _rand(void)
{
    uint32_t value;

    // Use a linear congruential generator (LCG) to update the state of the PRNG
    prngState *= 1103515245;
    prngState += 12345;
    value = (prngState >> 16) & 0x07FF;

    prngState *= 1103515245;
    prngState += 12345;
    value <<= 10;
    value |= (prngState >> 16) & 0x03FF;

    prngState *= 1103515245;
    prngState += 12345;
    value <<= 10;
    value |= (prngState >> 16) & 0x03FF;

    // Return the random value
    return value;
}

/*********************************************************************
 * @fn      rand_range
 *
 * @brief   Get a random value in the specified range
 *
 *
 * @param   min Lower bound
 *
 * @param   max Upper bound
 *
 *
 * @return  Random value in the specified range
 */
uint32_t rand_range(uint32_t min, uint32_t max)
{
    uint32_t value;

    // Valid parameters?
    if (max > min)
    {
        // Pick up a random value in the given range
        value = min + (_rand() % (max - min + 1));
    }
    else
    {
        // Use default value
        value = min;
    }

    // Return the random value
    return value;
}

void Map(int RSSI)
{
    int random;
    uint16_t color;
    static uint16_t x = 120;
    static uint16_t y = 230;

    random = rand_range(0, 3); // 线性取余法
    debug_printf("%d\r\n", random);

    switch (random)
    {
    case 0:
    {
        if (x < 225)
        {
            x += 5;
        }
        else
        {
            x = x;
        }
    }
    break;

    case 1:
    {
        if (x > 15)
        {
            x -= 5;
        }
        else
        {
            x = x;
        }
    }
    break;

    case 2:
    {
        if (y < 305)
        {
            y += 5;
        }
        else
        {
            y = y;
        }
    }
    break;

    case 3:
    {
        if (y > 155)
        {
            y -= 5;
        }
        else
        {
            y = y;
        }
    }
    break;

    default:
        break;
    }

    if (RSSI < -19 && RSSI > -85)
    {
        color = GREEN;
    }
    else if (RSSI < -84 && RSSI > -100)
    {
        color = YELLOW;
    }
    else
    {
        color = RED;
    }

    debug_printf("x:%d y:%d", x, y);
    LCD_DrawPoint(x, y, 3, color);
}
