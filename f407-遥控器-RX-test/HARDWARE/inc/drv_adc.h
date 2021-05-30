/**
  ******************************************************************************
  * @file    drv_adc.h
  * @author  xjc
  * @version v1.0
  * @date    20-10-08
  * @brief   �޸�HARDWARE_ADC(SOFT_ADC) (Ĭ��HARD��
			
  ******************************************************************************
  */
  


#ifndef __DRV_ADC_H
#define __DRV_ADC_H

//#define SOFTWARE_ADC 
#define HARDWARE_ADC 

#include "stm32f4xx.h"
#include "drv_timer.h" 
#include "drv_gpio.h"

void ADC_DMA_Config(void);
void ADC_GPIO_Configuration(void);
void Laser_Init(void);
uint32_t* Get_Laser_Val(void);
#endif

