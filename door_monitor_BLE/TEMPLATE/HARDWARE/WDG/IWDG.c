#include "IWDG.h"

void IWDG_INIT(void)
{
	//����������Ź��Ĵ�����д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//���÷�Ƶֵ256,�������Ź���Ӳ����ʱ��ʱ��Ƶ��=32KHZ/256=125HZ
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	//���ü���ֵ�������ĳ�ʱʱ��=1��
	IWDG_SetReload(125-1);
	
	//ˢ�¶������Ź��ļ���ֵ
	IWDG_ReloadCounter();
	
	//ʹ�ܶ������Ź�����
	IWDG_Enable();
	
}

void IWDG_RST_DETECT(void)
{
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		printf("IWDG RESET CPU!\r\n");
	}else
	{
		printf("NORMAL RESET CPU!\r\n");
	}
	
	RCC_ClearFlag();
}
