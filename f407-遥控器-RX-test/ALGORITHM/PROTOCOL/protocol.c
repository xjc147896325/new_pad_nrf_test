/**
  ******************************************************************************
  * @file    protocol.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-10-11
  * @brief   pack_deal
			 unpack_deal
  ******************************************************************************
  */
  
#include "protocol.h"

union FloatType
{
	float FloatNum; int IntNum;
};

uint8_t pack_deal(char * buff,uint32_t *distance, float beta)
{
	uint8_t i = 0;
	union FloatType Number;
	Number.FloatNum = beta;
	
	buff[0] = 0xaa;
	buff[1] = 0x55;
	
	for(i = 0; i < 3; i++)
	{
		buff[2 + i*4] = (uint8_t)distance[i];
		buff[3 + i*4] = (uint8_t)(distance[i] >> 8);
		buff[4 + i*4] = (uint8_t)(distance[i] >> 16);
		buff[5 + i*4] = (uint8_t)(distance[i] >> 24);
	}
	buff[14] = 0xc3;
	buff[15] = 0x3c;
	
	buff[16] = (uint8_t)Number.IntNum;
	buff[17] = (uint8_t)(Number.IntNum >> 8);
	buff[18] = (uint8_t)(Number.IntNum >> 16);
	buff[19] = (uint8_t)(Number.IntNum >> 24);
	
	buff[20] = 0x55;
	buff[21] = 0xaa;
	
	return 1;
}

uint8_t unpack_deal(char * buff,uint32_t *distance, float* beta)
{
	uint8_t i = 0;
	union FloatType Number;
	
	if(buff[0] == (char)0xAA && buff[1] == (char)0x55 )
	{

		for(i = 0; i < 3; i++)
		{
			distance[i] = 0;
			distance[i] |= buff[5 + 4*i];
			distance[i] <<= 8;
			distance[i] |= buff[4 + 4*i];
			distance[i] <<= 8;
			distance[i] |= buff[3 + 4*i];
			distance[i] <<= 8;
			distance[i] |= buff[2 + 4*i];
		}
		if(buff[14] == (char)0xc3 && buff[15] == (char)0x3c)
		{
			Number.IntNum  = 0;
			Number.IntNum  = buff[19];
			Number.IntNum  = (Number.IntNum << 8) | buff[18];
			Number.IntNum  = (Number.IntNum << 8) | buff[17];
			Number.IntNum  = (Number.IntNum << 8) | buff[16];
			*beta = Number.FloatNum;
			
			if(buff[20] == (char)0x55 && buff[21] == (char)0xaa)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}




