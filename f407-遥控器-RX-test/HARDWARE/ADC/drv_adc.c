/**
  ******************************************************************************
  * @file    drv_adc.c
  * @author  xjc
  * @version v1.0
  * @date    20-10-09
  * @brief   
  ******************************************************************************
  */
  
#include "drv_adc.h" 

#ifdef HARDWARE_ADC

uint32_t ADC_Val[3] = {0};

void ADC_GPIO_Configuration(void)
{
	GPIO_Format_Init(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_AN,GPIO_OType_PP,\
					GPIO_PuPd_UP,GPIO_Speed_100MHz);
}



void ADC_DMA_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
	
	ADC_GPIO_Configuration();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	DMA_DeInit(DMA2_Stream0); 
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//等待DMA可配置 
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_Val;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 3;    //与通道数设置一致
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;            //DMA_FIFOMode_Disable        DMA_FIFOMode_Enable
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
		
	DMA_Cmd(DMA2_Stream0, ENABLE);	

	
	ADC_DeInit();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE); //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); //ADC1禁止复位
	
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //ADC独立模式
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //是否提高采样率
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //配置成21M，保证ADC不超过36M
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; //采样通道之间的间隔
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //采样精度为12位
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;  //扫描模式禁止
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //ADC单次模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge =ADC_ExternalTrigConvEdge_None;  //上升沿触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 3; //一个转换序列
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_3Cycles); //配置ADC转换通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,2,ADC_SampleTime_3Cycles); //配置ADC转换通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,3,ADC_SampleTime_3Cycles); //配置ADC转换通道

//	ADC_DMACmd(ADC1,ENABLE);
	
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE); //使能ADC
	
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_SoftwareStartConv(ADC1);/*启动软件转换*/
}



void Laser_Init(void)
{
	ADC_DMA_Config();
}

uint32_t* Get_Laser_Val(void)
{	
	return ADC_Val;
}

#endif

