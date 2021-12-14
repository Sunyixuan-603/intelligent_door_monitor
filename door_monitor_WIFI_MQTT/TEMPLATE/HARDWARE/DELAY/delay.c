#include "delay.h"

void delay_xus(uint32_t n)
{

	SysTick->CTRL = 0; 		// Disable SysTick���ر�ϵͳ��ʱ��
	SysTick->LOAD = SystemCoreClock/1000000*n-1; // ����nus
	SysTick->VAL  = 0; 		// Clear current value as well as count flag����յ�ǰ����ֵ������count flag
	SysTick->CTRL = 5; 		// Enable SysTick timer with processor clock������ϵͳ��ʱ������ʱ��ԴΪ������ʱ��168MHz
	while ((SysTick->CTRL & 0x10000)==0);// Wait until count flag is set
	SysTick->CTRL = 0; 		// Disable SysTick���ر�ϵͳ��ʱ��
}


void delay_xms(uint32_t n)
{

	while(n--)
	{
		SysTick->CTRL = 0; 		// Disable SysTick���ر�ϵͳ��ʱ��
		SysTick->LOAD = SystemCoreClock/1000-1; // Count from 167999 to 0 (168000 cycles)
		SysTick->VAL  = 0; 		// Clear current value as well as count flag����յ�ǰ����ֵ������count flag
		SysTick->CTRL = 5; 		// Enable SysTick timer with processor clock������ϵͳ��ʱ������ʱ��ԴΪ������ʱ��168MHz
		while ((SysTick->CTRL & 0x10000)==0);// Wait until count flag is set
	}
	
	SysTick->CTRL = 0; 		// Disable SysTick���ر�ϵͳ��ʱ��
}

