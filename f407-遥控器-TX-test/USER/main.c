/**
  ******************************************************************************
  * @file    main.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-03-31
  * @brief   
  ******************************************************************************
  */
#include "main.h"
const char *g_Ashining = "xjcsb\r\n";

int main()
{	
//	double i = 0;
//	double Voltage_Data[4];
	
	System_Init();
	
	//GPIO_Format_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_100MHz);
	
//	GPIO_Format_Init(GPIOC, GPIO_Pin_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	RF24L01_Set_Mode( MODE_TX );		//发送模式
	
	while(1)
	{
		
		if(TX_OK == NRF24L01_TxPacket( (uint8_t *)g_Ashining, 9 ))		//模式1发送固定字符,1S一包
		{
			Serial_Send(Serial1, "TX OK\r\n");
		}
		Delay_TIM3_ms( 500 );	
		Delay_TIM3_ms( 500 );				
			
		
	}
}
