/**
  ******************************************************************************
  * @file    drv_dac.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-30
  * @brief   void Dac1_Init(void)

			 void Dac1_Set_Vol(u16 vol)

  ******************************************************************************
  */


#include "drv_dac.h"


void Dac1_Init(void)
{  
    DAC_InitTypeDef DAC_InitType;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
       
	GPIO_Format_Init(GPIOA,GPIO_Pin_4,GPIO_Mode_AN,GPIO_OType_PP,\
					GPIO_PuPd_DOWN,GPIO_Speed_100MHz);

    DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

    DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DACͨ��1

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp  /=  1000;
	temp=temp*4095/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}





















































