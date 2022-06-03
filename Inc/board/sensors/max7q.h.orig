#ifndef __MAX7Q_H_
#define __MAX7Q_H_

#include "stdint.h"
/**/
typedef uint8_t	u8 ;
typedef uint16_t u16;
typedef uint32_t u32;

#define GPS_ON  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)
#define GPS_OFF  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)


//GPS NMEA-0183协议重要参数结构体定义 
 
//UTC时间信息
typedef struct  
{										    
 	u16 year;	//年份
	u8 month;	//月份
	u8 date;	//日期
	u8 hour; 	//小时
	u8 min; 	//分钟
	u8 sec; 	//秒钟
}nmea_utc_time;   	   
//NMEA 0183 协议解析后数据存放结构体

typedef struct{		
	u8  Gps_Sta;
	nmea_utc_time UTC;			//UTC时间
	u32 Latitude;				//纬度 分扩大100000倍,实际要除以100000
	u8 	NS;					//北纬/南纬,N:北纬;S:南纬				  
	u32 Longitude;			    //经度 分扩大100000倍,实际要除以100000
	u8 	EW;					//东经/西经,E:东经;W:西经				  
 	u8 	Posslnum;				//用于定位的卫星数,0~12.
	int Altitude;			 	//海拔高度,放大了10倍,实际除以10.单位:0.1m
        u8  Total_Number;                             //可见卫星GSV句柄总数
        u8  Current_Number;                           //可见卫星GSV句柄总数
        u8 SNR[12];                                 // 可见卫星信噪比 （0-99）dB
}GPS_DATA; 
//////////////////////////////////////////////////////////////////////////////////////////////////// 	

 
				 
u32 NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(GPS_DATA *gpsx,u8 *buf);
void NMEA_GPGGA_Analysis(GPS_DATA *gpsx,u8 *buf);
void NMEA_GPRMC_Analysis(GPS_DATA *gpsx,u8 *buf);
void GPS_Init(void);
void  bubble_sort(u8 *buf, u8 len);
void NMEA_GPGSV_Analysis(GPS_DATA *gpsx,u8 *buf);
#endif	