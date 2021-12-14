#include "ADC.h"

static ADC_InitTypeDef ADC_InitStructure;
static ADC_CommonInitTypeDef ADC_CommonInitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

void ADC1_INIT(void)
{
	
	//�˿�A��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//ADC1��Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//����PA5����ΪAN��ģ���źţ�ģʽ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��ʹ������������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����ADC1��ز���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					//����ADC����
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADC1��Ӳ��ʱ��Ƶ��=84MHz/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 	//DMA�ر�
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*(1/42MHz)
	ADC_CommonInit(&ADC_CommonInitStructure);	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;   					//ת�����Ϊ12λ������
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //��ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //��������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����Ҫ���崥��ADC����
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//ת���Ľ���Ҷ���洢
	ADC_InitStructure.ADC_NbrOfConversion = 1;									//����ADCҪɨ���ͨ����Ŀ����ǰ��1��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//PA5�������ӵ�ADC1��ͨ��5���Ϊ1���Բ�����Ĳ���ʱ��Ϊ3��ADCʱ�����ڣ�ʱ��Խ��Խ׼ȷ
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	
	//ʹ��ADC1����
	ADC_Cmd(ADC1, ENABLE);
	
}

void ADC3_INIT(void)
{
	
	//�˿�F��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//ADC3��Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	
	//����PF7����ΪAN��ģ���źţ�ģʽ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ��ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��ʹ������������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����ADC3��ز���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					//����ADC����
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADC1��Ӳ��ʱ��Ƶ��=84MHz/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 	//DMA�ر�
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*(1/42MHz)
	ADC_CommonInit(&ADC_CommonInitStructure);	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;   					//ת�����Ϊ12λ������
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //��ͨ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //��������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����Ҫ���崥��ADC����
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//ת���Ľ���Ҷ���洢
	ADC_InitStructure.ADC_NbrOfConversion = 1;									//����ADCҪɨ���ͨ����Ŀ����ǰ��1��
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//PF7�������ӵ�ADC3��ͨ��5���Ϊ1���Բ�����Ĳ���ʱ��Ϊ3��ADCʱ�����ڣ�ʱ��Խ��Խ׼ȷ
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	
	//ʹ��ADC3����
	ADC_Cmd(ADC3, ENABLE);



}
