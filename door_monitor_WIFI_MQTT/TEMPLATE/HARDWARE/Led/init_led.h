#ifndef __INIT_LED_H_
#define __INIT_LED_H_

#include <stm32f4xx.h>
#include "sys.h"
//IDR�������־�Ĵ��� ƫ�ƼĴ����ṹ���ַΪ0x14,ODR�������־�Ĵ���,0x10

extern void Init_LED(void);
int32_t delay_ms(uint32_t nms);
void display_led(void);

#endif 
