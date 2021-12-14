#include <stm32f4xx.h>
#include "beep.h"

static GPIO_InitTypeDef GPIO_InitStruct;

void beep_init(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;				//�������
		GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;			//Ĭ�Ϸ����������Ͳ�����
		GPIO_Init(GPIOF,&GPIO_InitStruct);
		GPIO_ResetBits(GPIOF,8);

}


