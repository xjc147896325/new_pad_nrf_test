/**
  ******************************************************************************
  * @file    bsp_stepper.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-08
  * @brief   头文件
						 

  ******************************************************************************
  */
	
#include "bsp_stepper.h"

uint8_t rcr_remainder;   //重复计数余数部分
uint8_t is_rcr_finish=1; //重复计数器是否设置完成
long rcr_integer;	//重复计数整数部分
long target_pos=0;  //有符号方向
long current_pos=0; //有符号方向
DIR_Type motor_dir=CW;//顺时针

/************** 驱动器控制信号线初始化 ****************/
void Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6; //DRIVER_DIR DRIVER_OE对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE5,6
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//PE5输出高 顺时针方向  DRIVER_DIR
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);//PE6输出低 使能输出  DRIVER_OE
}

/***************** 启动TIM8 *****************/
void TIM8_Startup(uint32_t frequency)   //启动定时器8
{
	uint16_t temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM8,temp_arr);//设定自动重装值	
	TIM_SetCompare2(TIM8,temp_arr>>1); //匹配值2等于重装值一半，是以占空比为50%	
	TIM_SetCounter(TIM8,0);//计数器清零
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
}
/********************************************
//相对定位函数 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate_Rle(long num,uint32_t frequency,DIR_Type dir) //相对定位函数
{
	if(num <= 0) //数值小等于0 则直接返回
	{
		Serial_Send(Serial1, "\r\nThe num should be greater than zero!!\r\n");
		return;
	}
	if(TIM8->CR1 & 0x01)//上一次脉冲还未发送完成  直接返回
	{
		Serial_Send(Serial1, "\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency < 20) || (frequency > 100000))//脉冲频率不在范围内 直接返回
	{
		Serial_Send(Serial1, "\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	motor_dir = dir;//得到方向	
	if(motor_dir)//设置方向
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	}
	
	if(motor_dir == CW)//顺时针
		target_pos = current_pos+num;//目标位置
	else if(motor_dir == CCW)//逆时针
		target_pos = current_pos-num;//目标位置
	
	rcr_integer = num / (RCR_VAL + 1);//重复计数整数部分
	rcr_remainder = num % (RCR_VAL + 1);//重复计数余数部分
	is_rcr_finish = 0;//重复计数器未设置完成
	TIM8_Startup(frequency);//开启TIM8
}
/********************************************
//绝对定位函数 
//num   -2147483648～2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs(long num,uint32_t frequency)//绝对定位函数
{
	if(TIM8->CR1 & 0x01)//上一次脉冲还未发送完成 直接返回
	{
		Serial_Send(Serial1, "\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency < 20) || (frequency > 100000))//脉冲频率不在范围内 直接返回
	{
		Serial_Send(Serial1, "\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	target_pos = num;//设置目标位置
	if(target_pos != current_pos)//目标和当前位置不同
	{
		if(target_pos > current_pos)
		{
			motor_dir = CW;//顺时针
		}
		else
		{
			motor_dir = CCW;//逆时针
		}
		if(motor_dir)//设置方向
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_5);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		}
		rcr_integer = abs(target_pos - current_pos) / (RCR_VAL + 1);//重复计数整数部分
		rcr_remainder = abs(target_pos - current_pos) % (RCR_VAL + 1);//重复计数余数部分
		is_rcr_finish = 0;//重复计数器未设置完成
		TIM8_Startup(frequency);//开启TIM8
	}
}

uint8_t Get_is_rcr_finish(void)
{
	return is_rcr_finish;
}

void Set_rcr_finish(uint8_t num)
{
	is_rcr_finish = num;
}	

uint8_t Get_rcr_integer(void)
{
	return rcr_integer;
}

void Set_rcr_integer(uint8_t num)
{
	rcr_integer = num;
}	

uint8_t Get_rcr_remainder(void)
{
	return rcr_remainder;
}

void Set_rcr_remainder(uint8_t num)
{
	rcr_remainder = num;
}	


uint8_t Get_current_pos(void)
{
	return current_pos;
}

DIR_Type Get_motor_dir(void)
{
	return motor_dir;
}

void Set_current_pos(long num)
{
	current_pos = num;
}
