/**
  ******************************************************************************
  * @file    bsp_stepper.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-08
  * @brief   ͷ�ļ�
						 

  ******************************************************************************
  */
	
#ifndef __BSP_STEPPER_H
#define __BSP_STEPPER_H

#include "stm32f4xx.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "stdlib.h"

#define RCR_VAL    255  //ÿ������RCR_VAL+1���Σ��ж�һ�Σ����ֵ��0~255�����ô�һЩ���Խ����ж�Ƶ��

typedef enum
{
	CW 	= 1,	//�ߵ�ƽ˳ʱ��
	CCW = 0,	//�͵�ƽ��ʱ��
}DIR_Type;		//���з���

void Driver_Init(void);
void TIM8_Startup(uint32_t frequency);   				//������ʱ��8
void Locate_Abs(long num,uint32_t frequency);			//���Զ�λ����
void Locate_Rle(long num,uint32_t frequency,DIR_Type dir); 	//��Զ�λ����
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



