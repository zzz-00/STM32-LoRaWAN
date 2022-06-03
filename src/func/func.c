#include "func.h"
#include "usart.h"
#include "lorawan_node_driver.h"

void Device_Config(void)
{
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
