#ifndef __ST7789V_H
#define __ST7789V_H

#include "stm32l4xx_hal.h"

#define Height 320
#define Width  240
#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0
#define WHITE  0xffff
#define BLACK  0x0000
#define PURPLE 0xf81f

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t



#define IM0_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define IM0_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)

#define IM1_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define IM1_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)

#define IM2_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define IM2_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)

#define IC_OFF()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define IC_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)

#define REST_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET)
#define REST_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET)

#define CS_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)
#define CS_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)

#define DC_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define DC_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)

#define WR_HIGH()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define WR_LOW()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)

#define RD_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET)
#define RD_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET)

#define DATA0_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET)
#define DATA0_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET)

#define DATA1_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET)
#define DATA1_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET)

#define DATA2_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET)
#define DATA2_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET)

#define DATA3_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET)
#define DATA3_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET)

#define DATA4_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET)
#define DATA4_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET)

#define DATA5_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET)
#define DATA5_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET)

#define DATA6_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET)
#define DATA6_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET)

#define DATA7_HIGH()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET)
#define DATA7_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET)

#define DATA8_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)
#define DATA8_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)

#define DATA9_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
#define DATA9_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)

#define DATA10_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET)
#define DATA10_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET)

#define DATA11_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET)
#define DATA11_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET)

#define DATA12_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET)
#define DATA12_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET)

#define DATA13_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET)
#define DATA13_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET)

#define DATA14_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define DATA14_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)

#define DATA15_HIGH()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define DATA15_LOW()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)

void LCD_WriteCommand(uint8_t comd);
void LCD_WriteData_8bit(uint8_t data);
void ST7789V_INIT(void);
void LCDOpenWindows(uint16_t x, uint16_t y, uint16_t len, uint16_t wid);
void LCD_WriteData_16bit(uint16_t data);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint8_t size,uint16_t color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void Draw_Circle(u16 x0,u16 y0,u8 r, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2 ,u16 color);
u32 mypow(u8 m,u8 n);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t color);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u16 color);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_Clear(uint16_t color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t color);
void delay_10ms(int count);
void LCD_ST7789_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t Xend,uint16_t Yend,const unsigned char *pic,uint8_t mode);
void LCD_Test(void);
void LCD_Init(void);


#endif
