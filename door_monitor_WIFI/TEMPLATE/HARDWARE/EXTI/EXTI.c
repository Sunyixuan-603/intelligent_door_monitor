#include "EXTI.h"

static EXTI_InitTypeDef EXTI_InitStruct;
static NVIC_InitTypeDef NVIC_InitStruct;

void EXTI_INIT(void)
{
	//Ϊ������APB2�����ϵ�SYSCFG�˿ڹ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//��PA0�������ӵ��ⲿ�ն�EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	
	//��PE2,PE3,PE4���ӵ��ⲿ�ж�EXTI2,3,4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	//���ó�ʼ��EXTI0����
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//���ó�ʼ��NVIC����
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
	
	//���ó�ʼ��NVIC����
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
	
	//���ó�ʼ��NVIC����
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
	
	//���ó�ʼ��NVIC����
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
}

