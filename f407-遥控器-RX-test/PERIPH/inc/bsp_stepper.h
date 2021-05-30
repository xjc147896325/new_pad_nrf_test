/**
  ******************************************************************************
  * @file    bsp_stepper.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-08
  * @brief   头文件
						 

  ******************************************************************************
  */
	
#ifndef __BSP_STEPPER_H
#define __BSP_STEPPER_H

#include "stm32f4xx.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "stdlib.h"

#define RCR_VAL    255  //每计数（RCR_VAL+1）次，中断一次，这个值（0~255）设置大一些可以降低中断频率

typedef enum
{
	CW 	= 1,	//高电平顺时针
	CCW = 0,	//低电平逆时针
}DIR_Type;		//运行方向

void Driver_Init(void);
void TIM8_Startup(uint32_t frequency);   				//启动定时器8
void Locate_Abs(long num,uint32_t frequency);			//绝对定位函数
void Locate_Rle(long num,uint32_t frequency,DIR_Type dir); 	//相对定位函数
void TIM8_OPM_RCR_Init(uint16_t arr,uint16_t psc);

uint8_t Get_is_rcr_finish(void);
uint8_t Get_rcr_integer(void);
uint8_t Get_rcr_remainder(void);
uint8_t Get_current_pos(void);
void Set_rcr_integer(uint8_t num);
void Set_rcr_remainder(uint8_t num);
void Set_rcr_finish(uint8_t num);
DIR_Type Get_motor_dir(void);
void Set_current_pos(long num);

#endif



