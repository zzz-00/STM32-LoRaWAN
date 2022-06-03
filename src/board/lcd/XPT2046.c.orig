#include "XPT2046.h"
#include "ST7789v.h"
#include "math.h"
#include "app.h"

//DEVICE_SATE Device_Sate;
uint8_t LCD_EN = 1;

Pen_Holder Pen_Point;//定义笔实体


void XPT2046_init(void)
{
	XPT2046_CS_HIGH();
	XPT2046_CLK_HIGH();
  	XPT2046_D_OUT_HIGH();
}

/* 函数:SPI方式写一个字节
 * 参数:ch:uchar:字节值
 * 返回:(无)
 */
void XPT2046_write(uint8_t num)
{	
    uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)
			XPT2046_D_OUT_HIGH();
		else 
			XPT2046_D_OUT_LOW();   
		num<<=1;   
		
		XPT2046_CLK_LOW();   	 
		XPT2046_CLK_HIGH();   
	} 			
}

/* 函数:SPI方式读函数
 * 参数:(无)
 * 返回值:12位有效数据
 */
uint16_t XPT2046_read(uint8_t CMD)
{
    uint8_t count=0; 	  
	uint16_t Num=0; 
	
	XPT2046_CLK_LOW();			//先拉低时钟 	 
	XPT2046_CS_LOW(); 			//选中ADS7843	 
	XPT2046_write(CMD);	//发送命令字
	Delay_us(6);			//ADS7846的转换时间最长为6us
	XPT2046_CLK_HIGH();			//给1个时钟，清除BUSY   	    
	XPT2046_CLK_LOW(); 	 
	
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		XPT2046_CLK_LOW();//下降沿有效  	    	   
		XPT2046_CLK_HIGH();
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == 1)
		Num++; 		 
	}  	
	
	Num>>=4;   				//只有高12位有效.
	//LCD_ShowNum(50,200,Num,10,16);
	XPT2046_CS_HIGH(); 				//释放ADS7843	
	//while(1); 
	return(Num);   
}

//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
#define READ_TIMES 15 //读取次数
#define LOST_VAL 5	  //丢弃值
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=XPT2046_read(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 


//带滤波的坐标读取
//最小值不能少于100.
uint8_t Read_ADS(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	
	//LCD_ShowNum(60,110,xtemp,12,16); 
	//LCD_ShowNum(60,130,ytemp,12,16); 
	//while(1);												   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}


//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 50 //误差范围 
uint8_t Read_ADS2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
		//LCD_ShowNum(50,200,*x,5,16);
        *y=(y1+y2)/2;
		//LCD_ShowNum(50,220,*y,5,16);
        return 1;
    }else{
		return 0;	  
	}	
} 



//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	 HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);

//	 Read_ADS2(&Pen_Point.X,&Pen_Point.Y);
//	 Pen_Point.Key_Sta = 1;

//	 Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
//	 Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff; 

//	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
//	delay_10ms(50);
//	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
//}


void Delay_us(uint16_t i)
{
	uint8_t u=6;
	
	for(uint8_t j=0;j<i;j++)
	{
		while(u--);
	}
}

//画一个触摸点
//用来校准用的
void Drow_Touch_Point(u8 x,u16 y,u16 color)
{
	LCD_DrawLine(x-12,y,x+13,y,color);//横线
	LCD_DrawLine(x,y-12,x,y+13,color);//竖线
	LCD_DrawPoint(x+1,y+1,1,color);
	LCD_DrawPoint(x-1,y+1,1,color);
	LCD_DrawPoint(x+1,y-1,1,color);
	LCD_DrawPoint(x-1,y-1,1,color);
	Draw_Circle(x,y,6,color);//画中心圈
}



//触摸屏校准代码
//得到四个校准参数
int Touch_Adjust(void)
{								 
	u16 pos_temp[4][2];//坐标缓存值
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	float num=1.2;
	float fac; 	   
	cnt=0;			
	
	LCD_Clear(WHITE);//清屏   

	LCD_ShowString(35,110,"Please touch the dots,",BLUE);//校正完成
	LCD_ShowString(20,140,"carries on the correction!",BLUE);//校正完成

	Drow_Touch_Point(20,20,RED);//画点1 
	
	Pen_Point.Key_Sta = 0;// 消除触发信号
	Pen_Point.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	 
	
	while(1)
	{
		//if(Device_Sate !=START_MODE)
		//{
		//	return 0;
		//}
		if(Pen_Point.Key_Sta == 1)//按键按下了
		{		 	
			Pen_Point.Key_Sta = 0;
		 	pos_temp[cnt][0]=Pen_Point.X;
			pos_temp[cnt][1]=Pen_Point.Y;
			cnt++;
			
			switch(cnt)
			{			   
				case 1:
					LCD_Fill(0,0,40,40,WHITE);
					Drow_Touch_Point(220,20,RED);//画点2
					break;
				case 2:
					LCD_Fill(200,0,240,40,WHITE);
					Drow_Touch_Point(20,300,RED);//画点3
					break;
				case 3: 
					LCD_Fill(0,280,40,320,WHITE);
					Drow_Touch_Point(220,300,RED);//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		    //对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					//LCD_ShowNum(50,70,d1,3,16);
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20,RED);
						LCD_ShowString(20,110,"Touch Screen Adjust Again",BLUE);//校正完成
						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20,RED);
						LCD_ShowString(20,110,"Touch Screen Adjust Again",BLUE);//校正完成
						continue;
					}//正确了							   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_ShowString(20,110,"Touch Screen Adjust Again",BLUE);//校正完成
						continue;
					}//正确了
					//计算结果
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac	0.128							 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff	
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac	0.180		   
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff 349
				//	LCD_ShowNum(50,40,Pen_Point.xfac*1000,10,16)	;
				//  LCD_ShowNum(50,60,Pen_Point.xoff*1000000,20,16)	;
				//	LCD_ShowNum(50,80,Pen_Point.yfac*1000,10,16)	;
				//	LCD_ShowNum(50,100,Pen_Point.yoff,20,16);		 
				//	POINT_COLOR=BLUE;
					LCD_Fill(0,110,240,160,WHITE);
					LCD_ShowString(35,110,"Touch Screen Adjust OK!",BLUE);//校正完成
					delay_10ms(150);
				//	LCD_Clear(WHITE);
				//	while(1);  
					return 1;//校正完成				 
			}
		}
	} 
}	

