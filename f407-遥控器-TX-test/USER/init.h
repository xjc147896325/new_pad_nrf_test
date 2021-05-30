/**
  ******************************************************************************
  * @file    init.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-03-30
  * @brief   ͷ�ļ�
						 

  ******************************************************************************
  */
	
#ifndef __INIT_H
#define __INIT_H


#define ZERO_MODE               0 // 1������ѹ����ģʽ   0��������ѹ�ɼ�ģʽ
                                  // ����ѹ�������ڻ�ȡ�õ�0V������·������ʱ��ƫ�õ�ѹ
                                  // ADת��ֵ�����ݵ�·��ư��˷�����˵�ѹƫ��0.2V����
                                  // ����ģ���������˷ź�ʵ���ˡ��Ŵ���+��ѹƫ�á����ܣ�
                                  // �������Խ���˷����Ư�����⡣

// �Ŵ���=R2/R1=2000/6800��
#define OPA_RES_R1              6800  // 6.8k �˷�����˵���
#define OPA_RES_R2              2000  // 2k �˷ŷ�������
#define REFERENCE_VOLTAGE       3297  // �ο���ѹ���Ŵ�1000����
#define BIAS_VOLTAGE_IN1        0xFAB3E  // ����1ƫ�õ�ѹ������IN1��GND�̽�ʱAD7190ת�����
#define BIAS_VOLTAGE_IN2        0xF9DCA  // ����2ƫ�õ�ѹ������IN2��GND�̽�ʱAD7190ת�����
#define BIAS_VOLTAGE_IN3        0xFA8A4  // ����3ƫ�õ�ѹ������IN3��GND�̽�ʱAD7190ת�����
#define BIAS_VOLTAGE_IN4        0xFA9EB  // ����4ƫ�õ�ѹ������IN4��GND�̽�ʱAD7190ת�����


#include "stm32f4xx.h"
#include "drv_systick.h"
#include "drv_uart.h"
#include "drv_dac.h"
#include "drv_adc.h"
#include "drv_can.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include "drv_adc.h"
#include "drv_spi.h"
#include "bsp_esc.h"
#include "bsp_oled.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_ad7190.h"
#include "bsp_stepper.h"
#include "bsp_joystick.h"
#include "bsp_nrf24l01.h"

void System_Init(void);

#endif
