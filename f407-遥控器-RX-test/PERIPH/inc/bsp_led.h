#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx.h"
#include "drv_gpio.h"

typedef enum LEDs
{ 
  LED0   = 0x00,
  LED1   = 0x01
}LED_Typedef;


void LED_Init(void);
void LED_Task(LED_Typedef LEDx, uint32_t interval);

#endif

