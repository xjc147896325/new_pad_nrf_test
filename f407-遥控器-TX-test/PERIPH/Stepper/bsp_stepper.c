/**
  ******************************************************************************
  * @file    bsp_stepper.c
  * @author  jOKERII
  * @version v1.0
  * @date    20-11-08
  * @brief   ͷ�ļ�
						 

  ******************************************************************************
  */
	
#include "bsp_stepper.h"

uint8_t rcr_remainder;   //�ظ�������������
uint8_t is_rcr_finish=1; //�ظ��������Ƿ��������
long rcr_integer;	//�ظ�������������
long target_pos=0;  //�з��ŷ���
long current_pos=0; //�з��ŷ���
DIR_Type motor_dir=CW;//˳ʱ��

/************** �����������ź��߳�ʼ�� ****************/
void Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6; //DRIVER_DIR DRIVER_OE��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE5,6
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//PE5����� ˳ʱ�뷽��  DRIVER_DIR
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);//PE6����� ʹ�����  DRIVER_OE
}

/***************** ����TIM8 *****************/
void TIM8_Startup(uint32_t frequency)   //������ʱ��8
{
	uint16_t temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM8,temp_arr);//�趨�Զ���װֵ	
	TIM_SetCompare2(TIM8,temp_arr>>1); //ƥ��ֵ2������װֵһ�룬����ռ�ձ�Ϊ50%	
	TIM_SetCounter(TIM8,0);//����������
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8
}
/********************************************
//��Զ�λ���� 
//num 0��2147483647
//frequency: 20Hz~100KHz
//dir: CW(˳ʱ�뷽��)  CCW(��ʱ�뷽��)
*********************************************/
void Locate_Rle(long num,uint32_t frequency,DIR_Type dir) //��Զ�λ����
{
	if(num <= 0) //��ֵС����0 ��ֱ�ӷ���
	{
		Serial_Send(Serial1, "\r\nThe num should be greater than zero!!\r\n");
		return;
	}
	if(TIM8->CR1 & 0x01)//��һ�����廹δ�������  ֱ�ӷ���
	{
		Serial_Send(Serial1, "\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency < 20) || (frequency > 100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		Serial_Send(Serial1, "\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	motor_dir = dir;//�õ�����	
	if(motor_dir)//���÷���
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	}
	
	if(motor_dir == CW)//˳ʱ��
		target_pos = current_pos+num;//Ŀ��λ��
	else if(motor_dir == CCW)//��ʱ��
		target_pos = current_pos-num;//Ŀ��λ��
	
	rcr_integer = num / (RCR_VAL + 1);//�ظ�������������
	rcr_remainder = num % (RCR_VAL + 1);//�ظ�������������
	is_rcr_finish = 0;//�ظ�������δ�������
	TIM8_Startup(frequency);//����TIM8
}
/********************************************
//���Զ�λ���� 
//num   -2147483648��2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs(long num,uint32_t frequency)//���Զ�λ����
{
	if(TIM8->CR1 & 0x01)//��һ�����廹δ������� ֱ�ӷ���
	{
		Serial_Send(Serial1, "\r\nThe last time pulses is not send finished,wait please!\r\n");
		return;
	}
	if((frequency < 20) || (frequency > 100000))//����Ƶ�ʲ��ڷ�Χ�� ֱ�ӷ���
	{
		Serial_Send(Serial1, "\r\nThe frequency is out of range! please reset it!!(range:20Hz~100KHz)\r\n");
		return;
	}
	target_pos = num;//����Ŀ��λ��
	if(target_pos != current_pos)//Ŀ��͵�ǰλ�ò�ͬ
	{
		if(target_pos > current_pos)
		{
			motor_dir = CW;//˳ʱ��
		}
		else
		{
			motor_dir = CCW;//��ʱ��
		}
		if(motor_dir)//���÷���
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_5);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		}
		rcr_integer = abs(target_pos - current_pos) / (RCR_VAL + 1);//�ظ�������������
		rcr_remainder = abs(target_pos - current_pos) % (RCR_VAL + 1);//�ظ�������������
		is_rcr_finish = 0;//�ظ�������δ�������
		TIM8_Startup(frequency);//����TIM8
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
