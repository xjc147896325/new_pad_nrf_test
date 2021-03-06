/**
  ******************************************************************************
  * @file    bsp_oled.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-09-17
  * @brief   header of oled
  ******************************************************************************
  */
	
#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f4xx.h"
#include "oled_font.h"
#include "drv_systick.h"
#include "drv_i2c.h"
#include "stdio.h"
#include "main.h"

#define OLED_WIDTH  128
#define OLED_HEIGHT 64

#define WHITE 1 
#define BLACK 0 

#define min(a,b) ((a)<=(b)?(a):(b))
#define max(a,b) ((a)>=(b)?(a):(b))
#define ABS(x) (((x) > 0) ? (x) : -(x))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))	

void OLED_Init(void);

void OLED_Refresh_Gram(void);//????OLED?Դ?
void OLED_FillScreen(u8 color);//OLED????????
void OLED_DrawPixel(u8 x, u8 y, u8 color);//????
u8 OLED_GetPixel(u8 x, u8 y);//????
void OLED_DrawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);//??Բ
void OLED_ClearDisplay(void);//OLED????
void OLED_DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);//OLED??ͼ
void DrawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);//???ַ?
void FillRect2(int16_t x, int16_t y, uint8_t w, int8_t h, uint8_t color);////??ʵ?ľ??? ??????????????
void OLED_ShowString(u8 x,u8 y,char *chr);//OLED??ʾ?ַ???
void DrawRect(int16_t x, int16_t y, uint8_t w, int8_t h, uint8_t color);//?????ľ???
void FillPatternedRect(int16_t x, int16_t y, uint8_t w, int8_t h, const uint8_t *ptn);
void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);//????
void OLED_DrawFastVLine(int16_t x, int16_t y, int8_t h, uint8_t color);//????ֱ??
void OLED_DrawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color);//??ˮƽ??
void SetCursor(int16_t x, int16_t y);//????????
void OLED_PrintChar( char *chr);//??ӡ?ַ???
void OLED_PrintNum(u16 num);//??ӡ????
int  OLED_PrintCharEx(int16_t x, int16_t y,char *chr);////????????ӡ?ַ???

#endif

