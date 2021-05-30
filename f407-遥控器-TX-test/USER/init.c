/**
  ******************************************************************************
  * @file    init.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-03
  * @brief   System_Init(void);
						 

  ******************************************************************************
  */

#include "init.h"




/**
  * @brief  system init
  * @param  none
  * @retval ��
  */
void System_Init(void)
{
	char s[] = "heiheihei";
	
	SystemInit();
	NVIC_Priority_Init();
	SysTick_Init();
	
	USART1_Init(115200);
	
	
	
	
	TIM2_Init(8399,9);
	TIM3_Init(8399,9);    //�����ӳ�
	TIM_Cmd(TIM3, DISABLE);  //ʧ�ܶ�ʱ��
	

	
	NRF24L01_Gpio_Init();
	LED_Init();

	//���nRF24L01
	NRF24L01_check( );			
	RF24L01_Init( );


	Serial_Send(Serial1,"ready %s \r\n",&s);
	
	delay_ms(500);
	
}


