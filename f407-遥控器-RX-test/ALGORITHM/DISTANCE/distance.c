/**
  ******************************************************************************
  * @file    distance.c
  * @author  xjc
  * @version v1.0
  * @date    20-10-11
  * @brief   
  ******************************************************************************
  */
#include "distance.h"


#define L3 147.00f
#define PI 3.14159
#define LASER0 8570
#define LASER1 8400
#define LASER2 8400
#define DIS1_A 1.021f
#define DIS1_B 116.3241f
#define DIS2_A 1.0312f
#define DIS2_B 68.7445f

static uint32_t dis[3] = {0};

/*
  * @brief  ADCµÄ¶ÓÁÐ
  * @param  adc_val (dma get)
  * @retval NULL
  */
uint32_t* ADC_Queue_Updata(uint32_t* adc_val)
{
	static uint16_t i = 0, j = 0;
	static uint32_t temp[20][3] = {0},val[3] = {0}, sum[3] = {0};  //temp[val][adc_num]
	if(i < 20)
	{
		temp[i][0] = (uint32_t)adc_val[0];
		sum[0] += temp[i][0];
		temp[i][1] = (uint32_t)adc_val[1];
		sum[1] += temp[i][1];
		temp[i][2] = (uint32_t)adc_val[2];
		sum[2] += temp[i][2];
		i++;
	}
	else
	{
		val[0] = sum[0]/20;
		val[1] = sum[1]/20;
		val[2] = sum[2]/20;
		sum[0] -= temp[0][0];
		sum[1] -= temp[0][1];
		sum[2] -= temp[0][2];
		for( j = 0; j < 20; j++)
		{
			if(j == 19)
			{
				temp[j][0] = (uint32_t)adc_val[0];
				temp[j][1] = (uint32_t)adc_val[1];
				temp[j][2] = (uint32_t)adc_val[2];
			}
			else
			{
				temp[j][0] = temp[j+1][0];
				temp[j][1] = temp[j+1][1];
				temp[j][2] = temp[j+1][2];
			}
		}
		sum[0] += temp[19][0];
		sum[1] += temp[19][1];
		sum[2] += temp[19][2];

	}
	
//	Serial_Send(Serial1,"Dis[0] = %d\r\n",val[0]);
//	Serial_Send(Serial1,"Dis[1] = %d\r\n",val[1]);
//	Serial_Send(Serial1,"Dis[2] = %d\r\n",val[2]);

	return val;
}


uint32_t* Dis_Caculate(uint32_t* queue_val)
{
	
	dis[0] = (int)((float)(queue_val[0])/4096 * LASER0);
	dis[1] = (int)(((float)(queue_val[1]))*DIS1_A - DIS1_B);
	dis[2] = (int)(((float)(queue_val[2]))*DIS2_A - DIS2_B);
	
//	Serial_Send(Serial1,"Dis[0] = %d\r\n",dis[0]);
//	Serial_Send(Serial1,"Dis[1] = %d\r\n",dis[1]);
//	Serial_Send(Serial1,"Dis[2] = %d\r\n",dis[2]); //OK

	return dis;
}

uint32_t* Dis_get()
{
	return dis;
}


float Get_Beta(void)
{
	return atan2((float)dis[1]-(float)dis[2],L3)/PI*180;
}


