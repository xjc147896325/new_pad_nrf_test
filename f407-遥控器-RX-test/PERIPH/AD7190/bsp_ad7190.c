/**
  ******************************************************************************
  * @file    drv_spi.c (移植)
  * @author  jOKERII
  * @version v1.0
  * @date    20-02-23
  * @brief   deive for hardware spi
						 硬件SPI配置 软件待加入
						 void drv_spi_init( void )  SPI初始化
						 uint8_t drv_spi_read_write_byte( uint8_t TxByte ) SPI收发一个字节
						 void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )  SPI收发字符串

  ******************************************************************************
  */
  
#include "bsp_ad7190.h"

volatile uint8_t ADC_flag = 0;    // 启动采集标志
volatile int8_t number;          // 当前处理的通道
volatile int32_t ad7190_data[4]; // AD7190原始转换结果
volatile int32_t bias_data[4];   // 零点电压的AD转换结果
volatile double voltage_data[4]; // 电压值（单位：mV）


void AD7190_Reset(void)
{
  uint8_t registerWord[7];
  uint8_t ReadBuff[7] = {0};
  
  registerWord[0] = 0x01;
  registerWord[1] = 0xFF;
  registerWord[2] = 0xFF;
  registerWord[3] = 0xFF;
  registerWord[4] = 0xFF;
  registerWord[5] = 0xFF;
  registerWord[6] = 0xFF;
  drv_spi_read_write_string(ReadBuff,registerWord, 7);
}


uint32_t AD7190_GetRegisterValue(uint8_t registerAddress,uint8_t bytesNumber)
{
  uint8_t   registerWord[4] = {0, 0, 0, 0}; 
  uint8_t   TxBuff          = 0;
  uint32_t  buffer          = 0x0;
  uint8_t   i               = 0;
  
  TxBuff = AD7190_COMM_READ | AD7190_COMM_ADDR(registerAddress);
  
  drv_spi_read_write_string(0,&TxBuff, 1);    
  drv_spi_read_write_string(registerWord,0,bytesNumber);
  for(i = 0; i < bytesNumber; i++) 
  {
    buffer = (buffer << 8) + registerWord[i];
  }
  return buffer;
}

uint8_t AD7190_Init(void)
{
	unsigned char status = 1;
	uint32_t regVal = 0;

	drv_spi_init();  
	AD7190_Reset();

	/* Allow at least 500 us before accessing any of the on-chip registers. */
	delay_us(500);
	regVal = AD7190_GetRegisterValue(AD7190_REG_ID, 1);
	Serial_Send(Serial1,"ad7190:0x%X\r\n",regVal);  //此处有bug 应为%X //wule
	if( (regVal & AD7190_ID_MASK) != ID_AD7190)
	{
		status = 0;
	}
	return status ;
}

/***************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerAddress - Address of the register.
 * @param registerValue - Data value to write.
 * @param bytesNumber - Number of bytes to be written.
 *
 * @return none.
*******************************************************************************/
void AD7190_SetRegisterValue(unsigned char registerAddress,unsigned int  registerValue,unsigned char bytesNumber)
{
  unsigned char writeCommand[5] = {0, 0, 0, 0, 0};
  unsigned char* dataPointer    = (unsigned char*)&registerValue;
  unsigned char bytesNr         = bytesNumber;
  
  writeCommand[0] = AD7190_COMM_WRITE | AD7190_COMM_ADDR(registerAddress);
  while(bytesNr > 0)
  {
      writeCommand[bytesNr] = *dataPointer;
      dataPointer ++;
      bytesNr --;
  }
  drv_spi_read_write_string(0,writeCommand, bytesNumber+1);
}



/***************************************************************************//**
 * @brief Setting chop enable or disable
 *
 * @param chop - chop setting
 *               Example: 0 - Disable
 *                        1 - enable
 *  
 * @return none.
*******************************************************************************/

void AD7190_ChopSetting(uint8_t chop)
{
  uint32_t oldRegValue = 0x0;
  uint32_t newRegValue = 0x0;   
   
  oldRegValue = AD7190_GetRegisterValue(AD7190_REG_CONF, 3);
  if(chop==1)
  {
    newRegValue = oldRegValue | AD7190_CONF_CHOP;
  }
  else
  {
    newRegValue = oldRegValue & (~AD7190_CONF_CHOP); 
  }
  
  AD7190_SetRegisterValue(AD7190_REG_CONF, newRegValue, 3);
}

/***************************************************************************//**
 * @brief Selects the channel to be enabled.
 *
 * @param channel - Selects a channel.
 *  
 * @return none.
*******************************************************************************/
void AD7190_ChannelSelect(uint16_t channel)
{
  uint32_t oldRegValue = 0x0;
  uint32_t newRegValue = 0x0;   
   
  oldRegValue = AD7190_GetRegisterValue(AD7190_REG_CONF, 3);
  oldRegValue &= ~(AD7190_CONF_CHAN(0xFF));
  newRegValue = oldRegValue | AD7190_CONF_CHAN(1 << channel);   
  AD7190_SetRegisterValue(AD7190_REG_CONF, newRegValue, 3);
}

void AD7190_MultiChannelSelect(uint16_t channel)
{
  uint32_t oldRegValue = 0x0;
  uint32_t newRegValue = 0x0;   
   
  oldRegValue = AD7190_GetRegisterValue(AD7190_REG_CONF, 3);
  oldRegValue &= ~(AD7190_CONF_CHAN(0xFF));
  newRegValue = oldRegValue | AD7190_CONF_CHAN(channel);   
  AD7190_SetRegisterValue(AD7190_REG_CONF, newRegValue, 3);
}

/***************************************************************************//**
 * @brief Waits for RDY pin to go low.
 *
 * @return none.
*******************************************************************************/
void AD7190_WaitRdyGoLow(void)
{
  uint32_t timeOutCnt = 0xFFFFF;

  while(AD7190_RDY_STATE && timeOutCnt--)
  {
      ;
  }
}

/***************************************************************************//**
 * @brief Performs the given calibration to the specified channel.
 *
 * @param mode - Calibration type.
 * @param channel - Channel to be calibrated.
 *
 * @return none.
*******************************************************************************/
void AD7190_Calibrate(uint8_t mode, uint8_t channel)
{
  uint32_t oldRegValue = 0x0;
  uint32_t newRegValue = 0x0;
  
  AD7190_ChannelSelect(channel);
  oldRegValue = AD7190_GetRegisterValue(AD7190_REG_MODE, 3);
  oldRegValue &= ~AD7190_MODE_SEL(0x7);
  newRegValue = oldRegValue | AD7190_MODE_SEL(mode);
//    AD7190_CS_ENABLE(); 
  AD7190_SetRegisterValue(AD7190_REG_MODE, newRegValue, 3);
  AD7190_WaitRdyGoLow();
//    AD7190_CS_DISABLE();
}

/***************************************************************************//**
 * @brief setting continuous read data enable or disable
 *
 * @param cread - continuous read data
 *                 Example: 0 - Disable
 *                          1 - enable
 *
 * @return none.
*******************************************************************************/
void AD7190_Continuous_ReadData(unsigned char cread)
{
  unsigned char registerWord=0;
  
  if(cread==1)
  {
    registerWord=0x5C;
  }
  else
  {
    AD7190_WaitRdyGoLow();
    registerWord=0x5e;
  }
  drv_spi_read_write_string(0,&registerWord,1);
}

/***************************************************************************//**
 * @brief Selects the polarity of the conversion and the ADC input range.
 *
 * @param polarity - Polarity select bit. 
                     Example: 0 - bipolar operation is selected.
                              1 - unipolar operation is selected.
* @param range - Gain select bits. These bits are written by the user to select 
                 the ADC input range.     
 *
 * @return none.
*******************************************************************************/
void AD7190_RangeSetup(unsigned char polarity, unsigned char range)
{
  unsigned int oldRegValue = 0x0;
  unsigned int newRegValue = 0x0;
  
  oldRegValue = AD7190_GetRegisterValue(AD7190_REG_CONF,3);
  oldRegValue &= ~(AD7190_CONF_UNIPOLAR | AD7190_CONF_GAIN(0x7));
  newRegValue = oldRegValue | (polarity * AD7190_CONF_UNIPOLAR) | AD7190_CONF_GAIN(range) | AD7190_CONF_BUF; 
  AD7190_SetRegisterValue(AD7190_REG_CONF, newRegValue, 3);
}


/**
  * @brief :多路单端输入AD采集配置
  * @param :无
  *			
  * @note  :无
  * @retval:无
  */
void AD7190_Unipolar_Multichannel_Conf(void)
{
  uint32_t command = 0x0;
  
  // chop enable
  AD7190_ChopSetting(1);
  
  /* Calibrates channel AIN1(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_ZERO, AD7190_CH_AIN1P_AINCOM);
  /* Calibrates channel AIN2(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_ZERO, AD7190_CH_AIN2P_AINCOM);
  /* Calibrates channel AIN3(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_ZERO, AD7190_CH_AIN3P_AINCOM);
  /* Calibrates channel AIN4(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_ZERO, AD7190_CH_AIN4P_AINCOM);
  
  /* Selects unipolar operation and ADC's input range to +-Vref/1. */
  AD7190_RangeSetup(1, AD7190_CONF_GAIN_1);  
  /* Calibrates channel AIN1(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_FULL, AD7190_CH_AIN1P_AINCOM);
  /* Calibrates channel AIN2(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_FULL, AD7190_CH_AIN2P_AINCOM);
  /* Calibrates channel AIN3(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_FULL, AD7190_CH_AIN3P_AINCOM);
  /* Calibrates channel AIN4(+) - AINCOM(-). */
  AD7190_Calibrate(AD7190_MODE_CAL_INT_FULL, AD7190_CH_AIN4P_AINCOM);
     
  // 使能：AD7190_CH_AIN1P_AINCOM
  //       AD7190_CH_AIN2P_AINCOM
  //       AD7190_CH_AIN3P_AINCOM
  //       AD7190_CH_AIN4P_AINCOM
  AD7190_MultiChannelSelect(0xF0);
  
  /* Performs a conversion. */ 
  command = AD7190_MODE_SEL(AD7190_MODE_CONT) | AD7190_MODE_DAT_STA| \
                            AD7190_MODE_CLKSRC(AD7190_CLK_EXT_MCLK1_2) | AD7190_MODE_RATE(8);
  AD7190_SetRegisterValue(AD7190_REG_MODE, command, 3);  
  
  AD7190_Continuous_ReadData(1);

}


uint8_t Get_ADC_flag(void)// 启动采集标志
{
	return ADC_flag;
}	
int8_t Get_number(void)          // 当前处理的通道
{
	return number;
}	

volatile int32_t* Get_ad7190_data(void) // AD7190原始转换结果
{
	return ad7190_data;
}	

volatile int32_t* Get_bias_data(void)   // 零点电压的AD转换结果
{
	return bias_data;
}	

volatile double* Get_voltage_data(void) // 电压值（单位：mV）
{
	return voltage_data;
}	

void Set_ADC_flag(uint8_t num )// 启动采集标志
{
	ADC_flag = num;
}	
void Set_number(int8_t num)          // 当前处理的通道
{
	number = num;
}	

void Set_ad7190_data(int32_t* num) // AD7190原始转换结果
{
	uint8_t i = 0;
	for(i = 0; i < 4; i ++)
	{
		ad7190_data[i] = num[i];
	}
}	

void Set_bias_data(int32_t* num)   // 零点电压的AD转换结果
{
	uint8_t i = 0;
	for(i = 0; i < 4; i ++)
	{
		bias_data[i] = num[i];
	}
}	

void Set_voltage_data(double* num) // 电压值（单位：mV）
{
	uint8_t i = 0;
	for(i = 0; i < 4; i ++)
	{
		voltage_data[i] = num[i];
	}
}	



