/**
  ******************************************************************************
  * @file    transmit.c				
  * @author  jOKERII
  * @version v1.1
  * @date    2020-11-23	
  * @brief   void SX1278_Init(void)                //��ʼ������
			 void SX1278_Transmit(void)            //���ͺ�������������Ϊuint8_t TX_Buffer[20]�� 

			 
  ******************************************************************************
  */
#include "transmit.h"

//ģ�����ò�������
//�ı�ģ�������ֻ��ı��������ֵ��Ȼ���ʼ������
const uint8_t g_ASxx_Param_Config[ 6 ] = { 0xC0, 0x12, 0x34, 0x1A, 0x17, 0xC4 };			//����ģʽ
const uint8_t  g_ASxx_Config_Status_OK[ ] = { 0x4F, 0x4B, 0x0D, 0x0A };

uint8_t TX_Buffer[20] = {0};

//void PackDeal(void)
//{
//	static uint8_t i = 0;
//	
//	TX_Buffer[0] = 0xaa;
//	TX_Buffer[1] = 0x55;
//	
//	for(i = 0;i < 7;i++)
//	{
//		TX_Buffer[2+i*2]   = ADC_Value_G[i] & 0xff;    //С��
//		TX_Buffer[2+i*2+1] = ADC_Value_G[i] >>  8;
//	}
//	TX_Buffer[16] = 0;
//	TX_Buffer[17] = 0;
//	for(i = 0;i < 8 ;i++)
//	{
//		TX_Buffer[16] |= (KEY_G[15-i]&0x01) << (7  - i );
//	}
//	for(i = 8;i < 16;i++)
//	{
//		TX_Buffer[17] |= (KEY_G[15-i]&0x01) << (15 - i);
//	}
//	
//	TX_Buffer[18] = 0x55;
//	TX_Buffer[19] = 0xaa;
//}

#ifdef UART_COMMUNICATION

void Uart_Transmit(void)
{
	static uint8_t i = 0;
	
	PackDeal();
	
	for(i = 0;i < 20;i++)
	{
		Serial_SendByte(USART1,TX_Buffer[i]);
	}
	GUI_Key_Updata();
	for(i = 0;i < 16;i++)
	{
		KEY_G[i] = 0;                   //�������¾���λ ������Ҫ����
	}

}

#endif

#ifdef UART_DEBUG

void Uart_Debug_Transmit(void)
{
	static uint8_t i = 0;
	for(i = 0;i < 10;i++)
	{
		printf("KEY%d:    %d\r\n",i,KEY_G[i]);
	}
	for(i = 10;i < 16;i++)
	{
		printf("KEY%d:   %d\r\n",i,KEY_G[i]);
	}
	for(i = 0;i < 7;i++)
	{
		printf("ADC%d:   %d\r\n",i,ADC_Value_G[i]);
	}
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	GUI_Key_Updata();
	for(i = 0;i < 16;i++)
	{
		KEY_G[i] = 0;                   //�������¾���λ ������Ҫ����
	}
}

#endif

#ifdef SX1278_COMMUNICATION

/*---------------------------
          ��ҫ�Ƽ�
---------------------------*/
/**
  * @brief :ASxxģ���ʼ��
  * @param :��
  * @note  :����Ĭ�ϲ�����ʼ�����޸�Ĭ�ϲ������ɸı�ģ���ʼ������
  * @retval:
  *        @ASxx_Write_OK д��ɹ�
  *        @ASxx_Write_ERROR д��ʧ��
  */
ASxxWriteStatusType ASxx_param_init( void )
{
	uint8_t i = 0;
	uint8_t Read_Buff[ 5 ] = { 0 };
	
	drv_uart_tx_bytes((uint8_t *)g_ASxx_Param_Config, 6 );
	drv_uart_rx_bytes( Read_Buff );
	
	for( i = 0; i < 4; i++ )
	{
		if( Read_Buff[ i ] != g_ASxx_Config_Status_OK[ i ] )
		{
			break;
		}
	}
	
	if( 4 == i )
	{
		return ASxx_Write_OK;		//���óɹ�
	}
	else
	{
		return ASxx_Write_ERROR;	//����ʧ��
	}
	
}

/**
  * @brief :ASxxģ������ò���
  * @param :
  *         @pReadBuffer���������ص�ַ
  * @note  :��
  * @retval:��
  */
void ASxx_read_param( uint8_t *pReadBuffer )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC1, 0xC1, 0xC1 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
	drv_uart_rx_bytes( pReadBuffer );
	
}

/**
  * @brief :ASxxģ���ȡӲ���汾��
  * @param :
  *         @pReadBuffer��Ӳ���汾�ŷ��ص�ַ
  * @note  :��
  * @retval:��
  */
void ASxx_read_version( uint8_t *pReadBuffer )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC3, 0xC3, 0xC3 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
	drv_uart_rx_bytes( pReadBuffer );
}

/**
  * @brief :ASxxģ���ȡʵ�ʵ�ѹֵ
  * @param :
  *         @pReadBuffer����ѹֵ���ص�ַ
  * @note  :��
  * @retval:��
  */
void ASxx_read_voltage( uint8_t *pReadBuffer )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC5, 0xC5, 0xC5 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
	drv_uart_rx_bytes( pReadBuffer );
}

/**
  * @brief :ASxxģ�鸴λ
  * @param :��
  * @note  :��
  * @retval:��
  */
void ASxx_reset( void )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC4, 0xC4, 0xC4 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
}

/**
  * @brief :ASxxģ�鷢�����ݣ�����ģʽ��
  * @param :
  *        @Addr_H����ַ��λ
  *        @Addr_L����ַ��λ
  *        @Channel���ŵ�
  *        @pTxBuff���������ݵ�ַ
  *        @Length���������ݸ���
  * @note  :����ģʽ ���ݸ�����29��
  * @retval:��
  */
void ASxx_tx_packet( uint8_t Addr_H, uint8_t Addr_L, uint8_t Channel, uint8_t *pTxBuff, uint8_t Length )
{
	uint8_t Header[ 3 ] = { 0 };
	
	Header[ 0 ] = Addr_H;
	Header[ 1 ] = Addr_L;
	Header[ 2 ] = Channel;
	
	drv_uart_tx_bytes( Header, 3 );
	//��������
	drv_uart_tx_bytes( pTxBuff, Length );
}

/**
  * @brief :ASxxģ��������ݣ�����ģʽ��
  * @param :��
  * @note  :����ģʽ ���ݸ�����29��
  * @retval:��
  */
uint8_t ASxx_rx_packet( uint8_t *pRxBuff )
{
	uint8_t Length = 0;
	
	Length = drv_uart_rx_bytes( pRxBuff );
	
	return Length;
}

void SX1278_Init(void)
{
    USART2_Init(115200);
//	ASxx_param_init();
}

void SX1278_Transmit(void)
{
	static uint8_t i = 0;
	
	for(i = 0;i < 20;i++)
	{
		TX_Buffer[i] = 'h';
	}
/*-------------------------------
	�Ѳ��� usart3������ñҲ��Ҫ�Ű��߽��� ������
-------------------------------*/

	drv_uart_tx_bytes(TX_Buffer, 20 );			//
	

}


#endif

