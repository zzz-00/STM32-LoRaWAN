#ifndef _LORAWAN_NODE_DRIVER_H_
#define _LORAWAN_NODE_DRIVER_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*-------------------------根据实际情况添加必要的头文件-------------------------*/
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "common.h"
/*=====================================END======================================*/


/** 用户根据产品需要，在以下宏定义中自行补充AT指令（默认为NULL），此宏定义中所加的指令均会被保存到模块中。
 * @par 示例:
 * 如需设置入网扫描频方式为：单模、异频、1A2，可如下改写宏定义：
 * @code
	#define AT_COMMAND_AND_SAVE		"AT+BAND=47,0",  \
									"AT+CHMASK=00FF" \
 * @endcode
*/


#ifndef DEBUG_LOG_LEVEL_1
#define DEBUG_LOG_LEVEL_1
#endif


/** 模块连续异常计数最大值，超过该值时驱动将复位模块 */
#define ABNORMAL_CONTINUOUS_COUNT_MAX			6

/* -------------------------------------------------------------------------------
 * 					!!!!此线以上用户根据平台及需要自行修改!!!!
 *********************************************************************************
 *						!!!!此线以下，请勿做修改!!!!
 --------------------------------------------------------------------------------*/


/*--------------------日志输出等级，未尾数字越大，等级越高----------------------*/
#ifdef DEBUG_LOG_LEVEL_1
#define DEBUG_LOG_LEVEL_0
#endif
/*=====================================END======================================*/

/*-----------------------------模块进阶应用-------------------------------------*/
#ifndef M_ADVANCED_APP
#define M_ADVANCED_APP  0
#endif
/*=====================================END======================================*/

/*--------------------------发送数据的帧类型------------------------------------*/
/** 高四位为1代表确认帧 */
#define CONFIRM_TYPE		0x10
/** 高四位为0代表非确认帧 */
#define UNCONFIRM_TYPE		0x00
/*=====================================END======================================*/


/*-------------------------各种状态、引脚、返回值的枚举-------------------------*/
/**
 * 模块的各种工作状态
 */
typedef enum {
    command, 	///< 命令模式
    transparent,///< 透传模式
    wakeup,		///< 唤醒模块
    sleep,		///< 休眠模块
} node_status_t;

/**
 * 模块的各种功能引脚
 */
typedef enum {
    mode,	///< 模式引脚(切换命令和透传)
    wake,	///< 唤醒引脚(切换休眠和唤醒)
    stat,	///< 模块通信状态引脚
    busy,	///< 模块是否为忙引脚
} node_gpio_t;

/**
 * GPIO电平状态
 */
typedef enum {
    low,	///< 低电平
    high,	///< 高电平
    unknow,	///< 未知电平
} gpio_level_t;

/**
 * 释放链表资源的等级
 */
typedef enum {
    all_list,	///< 释放所有链表中所有资源
    save_data,	///< 保留链表中data不为空的资源
    save_data_and_last,  ///< 保留链表中data不为空的资源和最后一条资源
} free_level_t;

/**
 * 发送数据的返回状态
 */
typedef enum {
    NODE_COMM_SUCC	       		= 0x01,		///< 通信成功
    NODE_NOT_JOINED				= 0x02,		///< 模块未入网
    NODE_COMM_NO_ACK			= 0x04,		///< 确认帧未收到ACK
    NODE_BUSY_BFE_RECV_UDATA	= 0x08,		///< 模块当前处于忙状态
    NODE_BUSY_ATR_COMM			= 0x10,		///< 模块处于异常状态
    NODE_IDLE_ATR_RECV_UDATA	= 0x20,		///< 模块串口无响应
    USER_DATA_SIZE_WRONG		= 0x40		///< 数据包长度错误
} execution_status_t;


/**
 * 终端主动复位模块的信号标致
 */
typedef union node_reset_single
{
    uint8_t value;

    struct
    {
        uint8_t frame_type_modify      	 : 1;	///< 该位用于设置帧类型
        uint8_t RFU          			 : 7;	///< 其它位暂时保留，供以后使用
    } Bits;
} node_reset_single_t;


/**
 * 本次通信息的下行信息
 */
typedef struct down_info {
    uint16_t size;			///< 业务数据长度
    uint8_t *business_data;	///< 业务数据
#ifdef USE_NODE_STATUS
    uint8_t result_ind;	///< 该字段详见模块使用说明书
    int8_t snr;			///< 下行SNR，若无下行，则该值为0
    int8_t rssi;		///< 下行RSSI，若无下行，则该值为0
    uint8_t datarate;	///< 本次下行对应的上行速率
#endif
} down_info_t;

/**
 * 下行信息的链表结构
 */
typedef struct list {
    down_info_t down_info;   ///< 下行信息结构
    struct list *next;
} down_list_t;

/*=====================================END======================================*/

/*------------------------------提供给用户的接口函数----------------------------*/
void Node_Hard_Reset(void);
void nodeGpioConfig(node_gpio_t type, node_status_t value);
bool nodeCmdConfig(char *str);
bool nodeCmdInqiure(char *str,uint8_t *content);
bool nodeJoinNet(uint16_t time_second);
void nodeResetJoin(uint16_t time_second);
execution_status_t nodeDataCommunicate(uint8_t *buffer, uint8_t size, down_list_t **list_head);

#if M_ADVANCED_APP
execution_status_t node_block_send_lowpower(uint8_t frame_tpye, uint8_t *buffer, uint8_t size, down_list_t **list_head);
bool node_block_send_big_packet(uint8_t *buffer, uint16_t size, uint8_t resend_num, down_list_t **list_head);
#endif
/*=====================================END======================================*/

extern uint8_t confirm_continue_failure_count;

/*--------------------------------提供给用户的变量------------------------------*/
extern bool node_join_successfully;
/*=====================================END======================================*/

#endif // _LORAWAN_MODULE_DRIVER_H_
