#include "init_led.h"

static GPIO_InitTypeDef GPIO_InitStruct;

void Init_LED(void)
{
	
			//�򿪶˿�F��Ӳ��ʱ�ӣ����ǹ���
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
			
			//�򿪶˿�E��Ӳ��ʱ�ӣ����ǹ���
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
			
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9; 	//9������
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;//���ù���ģʽ
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
			GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
			GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
			GPIO_Init(GPIOF,&GPIO_InitStruct);
			
			//��PF9�������ӵ�TIM14
			GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);
			
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10; 	//10������
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
			GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
			GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
			GPIO_Init(GPIOF,&GPIO_InitStruct);	
			
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14; 	//9 10������
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
			GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
			GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������	
			
			GPIO_Init(GPIOE,&GPIO_InitStruct);

			GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_SET);
	
}
