#include "WWDG.h"

static NVIC_InitTypeDef NVIC_InitStruct;

void WWDG_INIT(void)
{
	//ʹ�ܴ��ڿ��Ź���Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	//��Ƶֵ������,���Ź��ļ�����ʱ��=(42MHZ/4096)/8 �� 1281HZ
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	//����ֵ
	WWDG_SetWindowValue(80);
	
	//���ü���ֵ,ͬʱʹ�ܿ��Ź�
	WWDG_Enable(127);			//7λ�ĵݼ�������
	
	//�����ж�
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//�����ǰ�����жϱ�־λ
	WWDG_ClearFlag();
	
	//ʹ����ǰ�����ж�
	WWDG_EnableIT();
}

