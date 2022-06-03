#ifndef _COMMON_H_
#define _COMMON_H_
#include "stdint.h"
#include "stdbool.h"


/** 读取当前系统的时间，单位为ms */
#define GET_SYSTEM_TIME		HAL_GetTick()

/** 根据实际MCU的打印函数替换宏定义 */
#define DEBUG_PRINTF		debug_printf

/** 全局变量 */
extern bool timeout_start_flag;  //Timeout sign


/** 设备模式功能定义 */
typedef enum {
	CMD_CONFIG_MODE = 0,
	DATA_TRANSPORT_MODE,
	PRO_TRAINING_MODE,
    NO_MODE
} DEVICE_MODE_T;


/** 系统时间差值计算 */
#define SUBTRANCTION_CORSS_ZERO(e, s)   e < s ? e + 4294967296 - s : e - s
/*=====================================END======================================*/

/*-----------------------------字符串和延时函数---------------------------------*/
void system_delay_ms(uint32_t delay);
bool time_out_break_ms(uint32_t time);
void lower2upper_and_remove_spaces(uint8_t *src,  uint8_t *des);
uint8_t* find_string(uint8_t *s, uint8_t *d);
void match_string(uint8_t *str, uint8_t *s, uint8_t *e, uint8_t *res);
uint32_t htoi(uint8_t s[], uint8_t size);
/*=====================================END======================================*/

#endif //common.h


