/**
  ******************************************************************************
  * @file    drv_dac.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-30
  * @brief   void Dac1_Init(void)

			 void Dac1_Set_Vol(u16 vol)

  ******************************************************************************
  */

#ifndef __DRV_DAC_H
#define __DRV_DAC_H	

#include "stm32f4xx.h"	     			    
#include "drv_gpio.h"	     			    


void Dac1_Init(void);		//DACͨ��1��ʼ��	 	 
void Dac1_Set_Vol(u16 vol);	//����ͨ��1�����ѹ


#endif

















