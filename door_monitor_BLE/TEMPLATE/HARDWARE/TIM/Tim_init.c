#include "Tim_init.h"

static TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
static NVIC_InitTypeDef 	   NVIC_InitStruct;
static TIM_OCInitTypeDef  	   TIM_OCInitStruct;
void TIM3_Init(void)		//500ms��ʱ�ж�
{
	//ʹ��TIM3��Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//����TIM3�Ķ�ʱʱ��
	TIM_TimeBaseStruct.TIM_Period = 20000/2-1;		//�Զ�����ֵ
	TIM_TimeBaseStruct.TIM_Prescaler = 4200 - 1;	//Ԥ��ƵֵΪ4200
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; 
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//��0��ʼ������TIM_Periodʱ�����ж�
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct);
	
	//����TIM3���жϴ�����ʱ�����
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//����TIM3�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;				//�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//��ͨ����NVIC����
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ��TIM3����
	TIM_Cmd(TIM3,ENABLE);
}

void TIM1_Init(void)		//100ms��ʱ�ж�
{
	//ʹ��TIM1��Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//����TIM1�Ķ�ʱʱ��
	TIM_TimeBaseStruct.TIM_Period = 4000-1;		//�Զ�����ֵ
	TIM_TimeBaseStruct.TIM_Prescaler = 4200-1;	//Ԥ��ƵֵΪ4200
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//��0��ʼ������TIM_Periodʱ�����ж�
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStruct);
	
	//����TIM3���жϴ�����ʱ�����
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	//����TIM3�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;				//�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//��ͨ����NVIC����
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ��TIM3����
	TIM_Cmd(TIM1,ENABLE);
}

void TIM2_Init(void)		//200ms��ʱ�ж�
{
	//ʹ��TIM2��Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//����TIM3�Ķ�ʱʱ��
	TIM_TimeBaseStruct.TIM_Period = 1000-1;		//�Զ�����ֵ
	TIM_TimeBaseStruct.TIM_Prescaler = 8400 - 1;	
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; 
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//��0��ʼ������TIM_Periodʱ�����ж�
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct);
	
	//����TIM3���жϴ�����ʱ�����
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//����TIM3�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;				//�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//��ͨ����NVIC����
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ��TIM3����
	TIM_Cmd(TIM2,ENABLE);

}

void TIM8_Init(void) 
{
	//ʹ��TIM8��Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	//����TIM8�Ķ�ʱʱ��
	TIM_TimeBaseStruct.TIM_Period = 40000-1;		//�Զ�����ֵ
	TIM_TimeBaseStruct.TIM_Prescaler = 16800 - 1;  
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; 
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//��0��ʼ������TIM_Periodʱ�����ж�
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStruct);
	
	//����TIM8���жϴ�����ʱ�����
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
	
	//����TIM8�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;				//�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//��ͨ����NVIC����
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ��TIM4����
	TIM_Cmd(TIM8,ENABLE);

}

void TIM14_Init(uint16_t t)
{
	//ʹ��TIM14��Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);

	//����TIM14�Ķ�ʱʱ��
	TIM_TimeBaseStruct.TIM_Period = 500-1;
	TIM_TimeBaseStruct.TIM_Prescaler = 8400-1;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputNState_Enable;	//
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//��Ч״̬Ϊ�ߵ�ƽ
	
	//��ʼ��TIM14ͨ��
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	//ʹ��TIM14����
	TIM_Cmd(TIM14,ENABLE);
	
}

static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  TIM_OCInitStructure;
static GPIO_InitTypeDef GPIO_InitStruct;

void breath_led_init(void)
{	
	/* GPIOA, GPIOB and GPIOE Clocks enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	TIM_TimeBaseStructure.TIM_Period = 100-1;        //10000
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;   	 //84000000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //0����99 down�Ļ����Ǵ�99����0��
	
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 100-1;        //10000
	TIM_TimeBaseStructure.TIM_Prescaler = 16799;   	 //168000000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //0����99 down�Ļ����Ǵ�99����0��
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 99;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 99;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
	
}

