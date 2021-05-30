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
const char *g_Ashining = "ashining";
uint8_t g_RF24L01RxBuffer[ 32 ] = { 0 }; 

int main()
{	
//	double i = 0;
//	double Voltage_Data[4];
	uint8_t i = 0;
	
	System_Init();
	
	//GPIO_Format_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Speed_100MHz);
	
//	GPIO_Format_Init(GPIOC, GPIO_Pin_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	RF24L01_Set_Mode( MODE_RX );		//接收模式
	
	while(1)
	{
		
	
		i = NRF24L01_RxPacket( g_RF24L01RxBuffer );		//接收字节
		if( 0 != i )
		{
			drv_uart_tx_bytes( g_RF24L01RxBuffer,i);	//输出接收到的字节
			
		}
		
			
		
	}
}
