/**
  ******************************************************************************
  * @file    drv_spi.h
  * @author  jOKERII
  * @version v1.0
  * @date    20-02-23
  * @brief   SPI .h file
						 宏定义__USE_SOFT_SPI_INTERFACE__决定是否使用软件spi（待完善）
						 
  ******************************************************************************
  */

#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "drv_gpio.h"

//SPI引脚定义
#define SPI_CLK_GPIO_PORT			GPIOB
#define SPI_CLK_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define SPI_CLK_GPIO_PIN			GPIO_Pin_13

#define SPI_MISO_GPIO_PORT			GPIOB
#define SPI_MISO_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define SPI_MISO_GPIO_PIN			GPIO_Pin_14

#define SPI_MOSI_GPIO_PORT			GPIOB
#define SPI_MOSI_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define SPI_MOSI_GPIO_PIN			GPIO_Pin_15

#define SPI_NSS_GPIO_PORT			GPIOA
#define SPI_NSS_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define SPI_NSS_GPIO_PIN			GPIO_Pin_12


#define spi_set_nss_high( )			SPI_NSS_GPIO_PORT->ODR |= SPI_NSS_GPIO_PIN						            		//片选置高
#define spi_set_nss_low( )			SPI_NSS_GPIO_PORT->ODR &= (uint32_t)( ~((uint32_t)SPI_NSS_GPIO_PIN ))	//片选置低

	#ifdef __USE_SOFT_SPI_INTERFACE__			/** 只有使用软件SPI才需要的封装 */			

	#define spi_set_clk_high( )			SPI_CLK_GPIO_PORT->ODR |= SPI_CLK_GPIO_PIN								//时钟置高
	#define spi_set_clk_low( )			SPI_CLK_GPIO_PORT->ODR &= (uint32_t)( ~((uint32_t)SPI_CLK_GPIO_PIN ))	//时钟置低

	#define spi_set_mosi_hight( )		SPI_MOSI_GPIO_PORT->ODR |= SPI_MOSI_GPIO_PIN							//发送脚置高
	#define spi_set_mosi_low( )			SPI_MOSI_GPIO_PORT->ODR &= (uint32_t)( ~((uint32_t)SPI_MOSI_GPIO_PIN ))	//发送脚置低

	#define spi_get_miso( )				(( SPI_MISO_GPIO_PORT->IDR & (uint32_t)SPI_MISO_GPIO_PIN) != SPI_MISO_GPIO_PIN ) ? 0 : 1 // 若相应输入位为低则得到0，相应输入位为高则得到1


	void drv_spi_init( void );
	uint8_t drv_spi_read_write_byte( uint8_t TxByte );
	void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );


	#else								                	/** 只有使用硬件SPI时会使用 */


	//SPI接口定义
	#define SPI_PORT				  	SPI2				        		//SPI接口
	#define SPI_PORT_CLK				RCC_APB1Periph_SPI2			//SPI时钟


	void    drv_spi_init( void );
	uint8_t drv_spi_read_write_byte( uint8_t TxByte );
	void    drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );

	#endif

#endif

