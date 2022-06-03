#include "max7q.h"					   
#include "stdio.h"	 
#include "stdarg.h"	 
#include "string.h"	 
#include "math.h"  
#include "stm32l4xx_hal.h"
#include "usart.h"



uint8_t Gps_DISGLL[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A};
uint8_t Gps_DISGSA[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31};
uint8_t Gps_DISGSV[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38};
uint8_t Gps_DISVTG[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46};
//uint8_t Gps_STOPPED[16]={0xB5,0x62,0x06,0x57,0x08,0x00,0x01,0x00,0x00,0x00,0x50,0x4F,0x54,0x53,0xAC,0x85};
uint8_t Gps_POWSAVE[10]={0xB5,0x62,0x06,0x11,0x02,0x00,0x08,0x01,0x22,0x92};
uint8_t Gps_STOP[12]={0xB5,0x62,0x06,0x04,0x04,0x00,0x00,0x00,0x08,0x00,0x16,0x74};
uint8_t Gps_START[12]={0xB5,0x62,0x06,0x04,0x04,0x00,0x00,0x00,0x09,0x00,0x17,0x76};


u8 gps_gvs_comp=0;

//int local_new_flag = 0;

//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号							  
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}
//m^n函数
//返回值:m^n次方.
u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
u32 NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 

//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGGA_Analysis(GPS_DATA *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GPGGA");
	posx=NMEA_Comma_Pos(p1,6);								//得到GPS状态
	if(posx!=0XFF)gpsx->Gps_Sta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//得到用于定位的卫星数
	if(posx!=0XFF)gpsx->Posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//得到海拔高度
	if(posx!=0XFF)gpsx->Altitude=NMEA_Str2num(p1+posx,&dx);  
}

//分析GPRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPRMC_Analysis(GPS_DATA *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	  
        u8 BTT;
	p1=(u8*)strstr((const char *)buf,"$GPRMC");
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	                        //得到UTC时间,去掉ms
                BTT =temp/10000 + 8;
                if(BTT>24)
                  gpsx->UTC.hour = BTT - 24;
                else 
		  gpsx->UTC.hour = BTT;
                
		gpsx->UTC.min=(temp/100)%100;
		gpsx->UTC.sec=temp%100;	 
                
//                if(gpsx->UTC.sec % 2 == 0)
//                  local_new_flag = 1;
	}			  			  
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	
		gpsx->Latitude=temp/NMEA_Pow(10,dx-2);
	}
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)gpsx->NS=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->Longitude=temp/NMEA_Pow(10,dx-2);	
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->EW=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->UTC.date=temp/10000;
		gpsx->UTC.month=(temp/100)%100;
		gpsx->UTC.year=2000+temp%100;	 	 
	} 
        
        
        
        
}



//分析GPGSV信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSV_Analysis(GPS_DATA *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx; 
        static u8 i=0,j=0,p=0;
        static u8 gsv_table[50][2]={0};
	p1=(u8*)strstr((const char *)buf,"$GPGSV");
	posx=NMEA_Comma_Pos(p1,1);								//句柄总数
	if(posx!=0XFF)gpsx->Total_Number=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,2);								//当前句柄编号
	if(posx!=0XFF)gpsx->Current_Number=NMEA_Str2num(p1+posx,&dx); 
        
        posx=NMEA_Comma_Pos(p1,4);								//卫星编号
	if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	       
        posx=NMEA_Comma_Pos(p1,7);								//得到该编号卫星信噪比
	if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx);

        posx=NMEA_Comma_Pos(p1,8);								//卫星编号
	if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	       	
	posx=NMEA_Comma_Pos(p1,11);								//得到该编号卫星信噪比
	if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx); 

        posx=NMEA_Comma_Pos(p1,12);								//卫星编号
	if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	       	
	posx=NMEA_Comma_Pos(p1,15);								//得到该编号卫星信噪比
	if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx); 
        
        posx=NMEA_Comma_Pos(p1,16);								//卫星编号
	if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	               
	posx=NMEA_Comma_Pos(p1,19);								//得到该编号卫星信噪比
	if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx); 
        
        
        for(p=1;p<gpsx->Total_Number;p++)
        {
              p1=(u8*)strstr((const char *)p1+6,"$GPGSV");
      //	posx=NMEA_Comma_Pos(p1,1);								//句柄总数
      //	if(posx!=0XFF)gpsx->Total_Number=NMEA_Str2num(p1+posx,&dx);	
              posx=NMEA_Comma_Pos(p1,2);								//当前句柄编号
              if(posx!=0XFF)gpsx->Current_Number=NMEA_Str2num(p1+posx,&dx); 
              
              posx=NMEA_Comma_Pos(p1,4);								//卫星编号
              if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	       
              posx=NMEA_Comma_Pos(p1,7);								//得到该编号卫星信噪比
              if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx);

              posx=NMEA_Comma_Pos(p1,8);								//卫星编号
              if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	       	
              posx=NMEA_Comma_Pos(p1,11);								//得到该编号卫星信噪比
              if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx); 

              posx=NMEA_Comma_Pos(p1,12);								//卫星编号
              if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	       	
              posx=NMEA_Comma_Pos(p1,15);								//得到该编号卫星信噪比
              if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx); 
              
              posx=NMEA_Comma_Pos(p1,16);								//卫星编号
              if(posx!=0XFF)gsv_table[i][0]=NMEA_Str2num(p1+posx,&dx);	               
              posx=NMEA_Comma_Pos(p1,19);								//得到该编号卫星信噪比
              if(posx!=0XFF)gsv_table[i++][1]=NMEA_Str2num(p1+posx,&dx); 
        }
        
        

        
        if(i >= 12)
        {
            bubble_sort(&gsv_table[0][0], i) ;
           for(j=0;j<i;j++)
           {
             if(gsv_table[j][1] > 60) gsv_table[j][1] = 60;
             gpsx->SNR[j] = gsv_table[j][1];
           }
             
           i=0;    
           gps_gvs_comp = 1;
        }

       
 
}


/*--------------               冒泡排序               --------*/

void  bubble_sort(u8 *exmp, u8 len)
{
  u8 i ,j,temp,temp1;
  
  u8 buf[50][2]={0};
  exmp = &buf[0][0];

  for(i=0;i<len-1;i++)
    for(j=0;j<len-1-i;j++)
    {
      if(buf[j][1]<buf[j+1][1])
      {
        temp = buf[j][1];
        buf[j][1]= buf[j+1][1];
        buf[j+1][1] = temp;
        
        temp = buf[j][0];
        buf[j][0]= buf[j+1][0];
        buf[j+1][0] = temp;
      }
    }
  
  if(len>12) len = 12;
  
  for(i=0;i<len-1;i++)
    for(j=0;j<len-1-i;j++)
    {
      if(buf[j][0]>buf[j+1][0])
      {
        temp = buf[j][0];
        buf[j][0]= buf[j+1][0];
        buf[j+1][0] = temp;
        
        temp = buf[j][1];
        buf[j][1]= buf[j+1][1];
        buf[j+1][1] = temp;
      }
    }  
  
  
  
  
}




void GPS_Analysis(GPS_DATA *gpsx,u8 *buf)
{
	NMEA_GPGGA_Analysis(gpsx,buf);	//GPGGA解析 	
	NMEA_GPRMC_Analysis(gpsx,buf);	//GPRMC解析
        NMEA_GPGSV_Analysis(gpsx,buf);  //GPGSV解析
}

//GPS校验和计算
//buf:数据缓存区首地址
//len:数据长度
//cka,ckb:两个校验结果.
void Ublox_CheckSum(u8 *buf,u16 len,u8* cka,u8*ckb)
{
	u16 i;
	*cka=0;*ckb=0;
	for(i=0;i<len;i++)
	{
		*cka=*cka+buf[i];
		*ckb=*ckb+*cka;
	}
}

void GPS_Init(void)
{
	GPS_ON;
	HAL_Delay(100);
	Usart1SendData(Gps_DISGLL,16);
	HAL_Delay(10);
	Usart1SendData(Gps_DISGSA,16);
	HAL_Delay(10);
	Usart1SendData(Gps_DISGSV,16);
	HAL_Delay(10);
	Usart1SendData(Gps_DISVTG,16);
	HAL_Delay(50);
	Usart1SendData(Gps_STOP,12);
}


/* config gps modue output, only gprmc*/
