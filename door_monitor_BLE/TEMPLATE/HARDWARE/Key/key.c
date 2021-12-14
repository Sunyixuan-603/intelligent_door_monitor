#include "key.h"
#include "delay.h"
#include "init_USART.h"
#include <string.h>
#include "includes.h"

static GPIO_InitTypeDef GPIO_InitStructure;

void display_key_func(void)
{
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));	//�������ض�Ӧ�����ⲿʱ�ӵķ�ʽ
		if(PAin(0) == Bit_RESET)
		{
				RCC->CFGR |= RCC_CFGR_SW_PLL;
				
		}
		
		if(PEin(2) == Bit_RESET)
		{
				RCC->CFGR |= RCC_CFGR_SW_HSE;
				
		}
		
		if(PEin(3) == Bit_RESET)
		{
				RCC->CFGR |= RCC_CFGR_SW_HSI;
				
		}

}

extern void key_init(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//����ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOF,&GPIO_InitStructure);

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				
		GPIO_Init(GPIOA,&GPIO_InitStructure);
}

extern void matrix_key_init(void)
{
	//�򿪶˿�A��Ӳ��ʱ�ӣ����ǶԶ˿�A����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//�򿪶˿�A��Ӳ��ʱ�ӣ����ǶԶ˿�B����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	//�򿪶˿�A��Ӳ��ʱ�ӣ����ǶԶ˿�C����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//�򿪶˿�A��Ӳ��ʱ�ӣ����ǶԶ˿�E����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//�򿪶˿�A��Ӳ��ʱ�ӣ����ǶԶ˿�G����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

					//������  (���)
////////////////////////////////////////////////////////////////////////////////////	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;  	//PB7
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;	//��RUģʽ
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;	//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��IO��PA0Ϊ����
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_4;  	//PA4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_15;  	//PG15
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;  	//PA4
	GPIO_Init(GPIOC,&GPIO_InitStructure);


					//������  (����)
////////////////////////////////////////////////////////////////////////////////////	
	
	//GPIOF9,F10��ʼ������ 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;		//PC9
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			    //����ģʽ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//�������������LED��Ҫ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		    //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;				    //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//��ʼ��GPIOF�������õ�����д��Ĵ���
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;		//PB6
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;		//PE6
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 ;		//PA8
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//�е�ƽȫ����Ϊ�ߵ�ƽ
	PBout(7)=1;
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=1;

}

char loop_get_key_sta(void)
{
	int line;
	
	//��һ��
	PBout(7)=0; 
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=1;	
	line = 1;
	while(PCin(9)==0) //(��:0,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"A",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'A';
	}
	while(PBin(6)==0)//(��:0,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"#",line);\
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '#';
	}		
	while(PEin(6)==0)//(��:0,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"0",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '0';			
	}		
	while(PAin(8)==0)//(��:0,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"*",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '*';
	}				
	
	//�ڶ���
	PBout(7)=1; 
	PAout(4)=0;
	PGout(15)=1;
	PCout(7)=1;	
	line = 2;
	while(PCin(9)==0) //(��:1,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"C",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'C';
	}
	while(PBin(6)==0)//(��:1,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"9",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '9';
	}		
	while(PEin(6)==0)//(��:1,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"8",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '8';
	}		
	while(PAin(8)==0)//(��:1,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"7",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '7';
	}
	
	//������
	PBout(7)=1; 
	PAout(4)=1;
	PGout(15)=0;
	PCout(7)=1;			
	
	while(PCin(9)==0) //(��:2,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"B",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'B';
	}
	while(PBin(6)==0)//(��:2,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"6",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '6';	
	}		
	while(PEin(6)==0)//(��:2,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"5",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '5';	
	}		
	while(PAin(8)==0)//(��:2,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"4",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '4';	
	}		
	
	//������
	PBout(7)=1; 
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=0;	
	while(PCin(9)==0) //(��:3,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"D",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'D';
	}
	while(PBin(6)==0)//(��:3,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"3",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '3';
	}		
	while(PEin(6)==0)//(��:3,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"2",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '2';
	}		
	while(PAin(8)==0)//(��:3,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"1",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '1';
	}		
	
	return 'n';
}

//ʶ���Ӧ�İ���
char get_key_status(void)
{
		char flag_menu;
		int line;
		//��һ��
		PBout(7)=0; 
		PAout(4)=1;
		PGout(15)=1;
		PCout(7)=1;	
		line = 1;
		while(PCin(9)==0) //(��:0,��:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"A",line);
			break;
		}
		while(PBin(6)==0)//(��:0,��:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"#",line);
			flag_menu = '#';
			break;			
		}		
		while(PEin(6)==0)//(��:0,��:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"0",line);
			break;			
		}		
		while(PAin(8)==0)//(��:0,��:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"*",line);
			flag_menu = '*';
			break;			
		}				
		
		//�ڶ���
		PBout(7)=1; 
		PAout(4)=0;
		PGout(15)=1;
		PCout(7)=1;	
		line = 2;
		while(PCin(9)==0) //(��:1,��:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"C",line);
			break;
		}
		while(PBin(6)==0)//(��:1,��:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"9",line);
			break;			
		}		
		while(PEin(6)==0)//(��:1,��:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"8",line);
			break;			
		}		
		while(PAin(8)==0)//(��:1,��:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"7",line);
			break;			
		}
		
		//������
		PBout(7)=1; 
		PAout(4)=1;
		PGout(15)=0;
		PCout(7)=1;			
		line = 3;
		while(PCin(9)==0) //(��:2,��:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"B",line);
			break;
		}
		while(PBin(6)==0)//(��:2,��:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"6",line);
			break;			
		}		
		while(PEin(6)==0)//(��:2,��:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"5",line);
			break;			
		}		
		while(PAin(8)==0)//(��:2,��:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"4",line);
			flag_menu = '4';
			break;			
		}		
		
		//������
		PBout(7)=1; 
		PAout(4)=1;
		PGout(15)=1;
		PCout(7)=0;	
		line = 4;
		while(PCin(9)==0) //(��:3,��:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"D",line);
			break;
		}
		while(PBin(6)==0)//(��:3,��:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"3",line);
			flag_menu = '3';
			break;			
		}		
		while(PEin(6)==0)//(��:3,��:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"2",line);
			flag_menu = '2';
			break;			
		}		
		while(PAin(8)==0)//(��:3,��:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"1",line);
			flag_menu = '1';
			break;			
		}		
		
		return flag_menu;
}


#if 1
//��������
void key_stabilize(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,char *ch,int line)
{

	//��ʱ����
	delay_xms(50);
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) ==0)
	{
		 //�ȴ�����1�ͷ�
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) ==0)
		{
				delay_xms(1);
		}
		
		if(GPIOx == GPIOA && GPIO_Pin == GPIO_Pin_8 && line == 1)
		{
				PFout(8) = 1;
				delay_xms(250);
				PFout(8) = 0;
				PFout(8) = 1;
				delay_xms(100);
				PFout(8) = 0;
				printf("���빦�ܲ˵�\r\n");
		}else{
				PFout(8) = 1;
				delay_xms(100);
				PFout(8) = 0;
		}
	}	
	//�������
	memset(ch,0,sizeof ch);	//���������͵�ch����ֻ�Ǹ��ַ����������ʽд��
}
#endif

uint32_t exti_key_sta_get(void)
{
	uint32_t key_sta=0;
	
	if(PAin(0) == 0)
		key_sta|=1<<0;
	
	if(PEin(2) == 0)
		key_sta|=1<<1;
	
	if(PEin(3) == 0)
		key_sta|=1<<2;
	
	if(PEin(4) == 0)
		key_sta|=1<<3;
	
	return key_sta;
}


