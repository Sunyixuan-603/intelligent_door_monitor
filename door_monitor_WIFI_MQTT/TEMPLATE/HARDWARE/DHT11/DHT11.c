#include "DHT11.h"
#define PGout(y) (*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x14 - 0x40000000)*32 + y*4))
#define PGin(x)	 (*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x10 - 0x40000000)*32 + x*4))
static GPIO_InitTypeDef GPIO_InitStructure;

void DHT11_INIT(uint8_t op)
{
	//�򿪶˿�G��Ӳ��ʱ�ӣ����ǹ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; 	//9������
	if(op)
	{
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;	//���ģʽ
	}else{
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;	//���ģʽ
	}
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//��©
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	//ֻҪ�����ģʽ���϶����г�ʼ��ƽ��״̬���������豸��˵����
	PGout(9)=1;
}

void DHT11_OUTPUT_MODE(void)	//���ڳ�ʼ�������źŸ�ģ��ȴ�����Ӧ
{
	PGout(9)=0;
	delay_xms(18);
	
	PGout(9)=1;
	delay_xus(30);
}

int16_t DHT11_REC_DATA(uint8_t *array)
{
	DHT11_INIT(1);	//Ĭ�ϳ�ʼ��ģ��Ϊ�������
	
	DHT11_OUTPUT_MODE();
	
	DHT11_INIT(0);	//��ʼ����ģ����Ӧת��Ϊ��������
	
	uint32_t t=0;
	int32_t i=0,j=0;
	uint8_t d=0;
	uint8_t *p= array;
	uint8_t check_sum=0;
	
	//�ȴ��͵�ƽ����
	t=0;
	while(PGin(9))
	{
		t++;
		delay_xus(1);
		
		if(t >= 4000)
			return -1;
	}

	//�ó�ʱ���ķ��������͵�ƽ�ĺϷ���
	t=0;
	while(PGin(9)==0)
	{
		t++;
		delay_xus(1);
		
		if(t >= 100)
			return -2;
	}	
	
	//�ó�ʱ���ķ��������ߵ�ƽ�ĺϷ���
	t=0;
	while(PGin(9))
	{
		t++;
		delay_xus(1);
		
		if(t >= 100)
			return -3;
	}	
	
	for(j=0; j<5; j++)
	{
		//����һ���ֽڵ�����
		for(d=0,i=7; i>=0; i--)
		{
			//�ó�ʱ���ķ��������͵�ƽ�ĺϷ���
			t=0;
			while(PGin(9)==0)
			{
				t++;
				delay_xus(1);
				
				if(t >= 100)
					return -4;
			}

			//��ʱ40us ����ʱʱ����28us ~ 70us��
			delay_xus(40);
			
			if(PGin(9))
			{
				d|=1<<i;	//��d������Ӧ��bit��1
				
				//�ȴ��ߵ�ƽ�������
				t=0;
				while(PGin(9))
				{
					t++;
					delay_xus(1);
					
					if(t >= 100)
						return -5;
				}			
			}
		}	
		p[j]=d;
	}
	
	//��ʱ50us�����Ժ���ͨѶ�����ĵ͵�ƽ
	delay_xus(50);
	
	//����У��ͣ������յ��������Ƿ�׼ȷ
	check_sum = (p[0]+p[1]+p[2]+p[3])&0xFF;
	
	if(check_sum == p[4])
		return 0;
	
	return -6;

}
