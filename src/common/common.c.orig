#include "stm32l4xx.h"
#include "common.h"



/** 设置超时开始标致 */
bool timeout_start_flag = true;

/** 定义设备模式参数 */
DEVICE_MODE_T device_mode = NO_MODE;
DEVICE_MODE_T *Device_Mode_str = &device_mode; 


/**
*  延时函数
*@param 	delay: 延时的时间，单位毫秒
*/
void system_delay_ms(uint32_t delay) 
{
	uint32_t tickstart = 0U;
	tickstart = GET_SYSTEM_TIME;
	while((GET_SYSTEM_TIME - tickstart) < delay);
}


/**
*  超时函数
*@param 	time: 设置的具体超时时间值，单位毫秒
*@return	是否到设定的超时时间
*/
bool time_out_break_ms( uint32_t time )
{
	static uint32_t start_time;
	
	if(true == timeout_start_flag)
	{
		start_time = GET_SYSTEM_TIME;
		timeout_start_flag = false;
	}
	if(SUBTRANCTION_CORSS_ZERO(GET_SYSTEM_TIME, start_time) >= time)
	{
		timeout_start_flag = true;
		return true;
	}
	return false;
}


/**
*  将小写字母转为大写并删除空格
*@param 	[IN]src: 原始字符串
*@param 	[OUT]des: 目标地址
*@return	无
*/
void lower2upper_and_remove_spaces(uint8_t *src,  uint8_t *des)
{
    do
    {
        if(*src >= 'a' && *src <= 'z')
        {
            *des++ = *src - 'a' + 'A';
        }
        else if(' ' != *src)
        {
            *des++ = *src;
        }
        
    }while(*src++);	
}


/**
*  查找一个字符串是否在另一个字符串中出现
*@param 	s: 原始字符串
*@param 	d: 要查找的字符串
*@return	返回查找的结果
*@retval		NULL: d字符串未出现在s字符串
*@retval		s:    d字符串出现在s字符串,且返回当前指针s所指向的地址
*/
uint8_t* find_string(uint8_t *s, uint8_t *d)
{
    uint8_t *tmp;
    
    while(0 != *s && 0 != *d)
    {
        tmp = d;
        while(*s == *tmp && *s && *tmp)
        {
            s++;
            tmp++;
        }
        
        if(0 == *tmp)
        {
            return s;
        }
        s++;
    }
    return NULL;
}


/**
*  截取指定开头和结尾的中间字符
*@param		str: 原始字符串
*@param 	s: 开始字符串
*@param		e: 结尾字符串
*@param		res: 截取到的字符串
*/
void match_string(uint8_t *str, uint8_t *s, uint8_t *e, uint8_t *res)
{
    uint8_t *first_result = NULL;
    uint8_t *f_t = NULL, *tmp = NULL;
    uint8_t i = 0, t_i = 0, result_flag = 0;

    first_result = find_string(str, s);

    if(NULL != first_result)
    {
        f_t = first_result;
        while(0 != *f_t && 0 != *e)
        {
            tmp = e;
            t_i = 0;
            while(*f_t == *tmp && *f_t && *tmp)
            {
                f_t++;
                tmp++;
                t_i++;
            }

            if(0 == *tmp)
            {
				result_flag = 1;
                break;
            }
            else if(t_i > 0)
            {
                f_t--;
                t_i--;
            }

            f_t++;
            i += t_i + 1;
        }

        while(i > 0 && result_flag)
        {
            *res = *first_result;
            res ++;
            first_result ++;
            i--;
        }
    }
}


/**
*  十六进制字符串转整数
*@param		s: 十六进制字符串
*@param 	s: 转换字符的升序
*@return	整数结果
*/
uint32_t htoi(uint8_t s[], uint8_t size)  
{  
    uint8_t i = 0;  
    uint32_t  n = 0;  
	
	
    for (i = 0; i < size; i++)  
    {  
		s[i] = s[i] >= 'A' && s[i] <= 'Z' ? s[i] + 'a' - 'A' : s[i];

		if((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z'))
		{
			if (s[i] > '9')  
			{
				n = (n << 4) + (10 + s[i] - 'a');  
			}
			else  
			{  
				n = (n << 4) + (s[i] - '0');  
			} 			
		}
		else
		{
			break;
		}
    }

    return n;  
}  



