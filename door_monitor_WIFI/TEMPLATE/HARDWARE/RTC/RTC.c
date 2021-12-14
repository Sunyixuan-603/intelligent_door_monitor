#include "RTC.h"

static NVIC_InitTypeDef  NVIC_InitStructure;
static EXTI_InitTypeDef  EXTI_InitStructure;
static RTC_DateTypeDef  RTC_DateStructure;
static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_InitTypeDef  RTC_InitStructure;
static RTC_AlarmTypeDef RTC_AlarmStructure;

void RTC_CONFIG(void)
{
	/*Enable the PWR clock*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	/*Allow access to RTC*/
	PWR_BackupAccessCmd(ENABLE);
	
	#if 0
	/*ʹ�ܵ����ⲿʱ��LSE*/
	RCC_LSEConfig(RCC_LSE_ON);
	
	/*����LSE�Ƿ���Ч*/
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/*ѡ��LSE��ΪRTC��Ӳ��ʱ��Դ*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	#else
	/*��LSE�޷�����ʱ������LSI*/
	
	/*ʹ�ܵ����ڲ�ʱ��LSI*/
	RCC_LSICmd(ENABLE);
	
	/*����LSI�Ƿ���Ч*/
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	/*ѡ��LSI��ΪRTC��Ӳ��ʱ��Դ*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	#endif
	
	/*ʹ��RTCʱ��*/
	RCC_RTCCLKCmd(ENABLE);
	
	/*�ȴ�RTC��ؼĴ�������*/
	RTC_WaitForSynchro();
	
	#if 0  //LSE
	
	/*Configure the RTC data Register and RTC prescaler*/
	RTC_InitStructure.RTC_AsynchPrediv = 127;					//�첽��Ƶϵ��
	RTC_InitStructure.RTC_SynchPrediv = 255;				//ͬ����Ƶϵ��
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
	#else
	RTC_InitStructure.RTC_AsynchPrediv = 127;				//�첽��Ƶϵ��
	RTC_InitStructure.RTC_SynchPrediv = 249;				//ͬ����Ƶϵ��
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
	
	#endif
	
	/*Set the date*/
	RTC_DateStructure.RTC_Year = 0x21;
	RTC_DateStructure.RTC_Month = RTC_Month_August;
	RTC_DateStructure.RTC_Date = 0x01;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Sunday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
	/*Set the time*/
	RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	RTC_TimeStructure.RTC_Hours   = 0x19;
	RTC_TimeStructure.RTC_Minutes = 0x07;
	RTC_TimeStructure.RTC_Seconds = 0x55; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); 
	
	//�رջ��ѹ���
	RTC_WakeUpCmd(DISABLE);
	
	//Ϊ���ѹ���ѡ��RTC���úõ�ʱ��Դ
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//���û��Ѽ���ֵΪ�Զ����أ�д��ֵĬ����0
	RTC_SetWakeUpCounter(1-1);
	
	//���RTC�����жϱ�־
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//ʹ��RTC�����ж�
	RTC_ITConfig(RTC_IT_WUT,ENABLE);
	
	//ʹ�ܻ��ѹ���
	RTC_WakeUpCmd(ENABLE);
	
	/*Configure EXTI LINE22 �����ⲿ�жϿ�����22*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//��ǰʹ���ⲿ�жϿ�����22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//�����ش����ж� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ʹ���ⲿ�жϿ�����22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;			//����RTC�����жϴ���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�Ϊ0x3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//��Ӧ���ȼ�Ϊ0x3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_GET_TIME(void)
{
	//��ȡʱ��
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);
	printf("%02x:%02x:%02x\r\n",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);

	//��ȡ����
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
	printf("20%02x/%02x/%02xWeek:%x\r\n",RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,RTC_DateStructure.RTC_WeekDay);	

}

void RTC_RST_CONFIG(void)
{
		/* ��������ȡ���ݼĴ�����ֵ������0x1688�������rtc���ں�ʱ������� */
		if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x8888)
		{
			
			RTC_CONFIG();
			/* �Ա��ݼĴ���0д������0x1688�����ڱ���Ѿ���ʼ���� */
			RTC_WriteBackupRegister(RTC_BKP_DR0,0x8888);
		}
		else
		{
		
			/* Enable the PWR clock ��ʹ�ܵ�Դ����ʱ��*/
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
			
			/* Allow access to RTC��������ʱ��ݵ�· */
			PWR_BackupAccessCmd(ENABLE);


			#if 0
			/* ʹ��LSE*/
			RCC_LSEConfig(RCC_LSE_ON);
			
			/* ����LSE�Ƿ���Ч*/  
			while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

			/* ѡ��LSE��ΪRTC��Ӳ��ʱ��Դ*/
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
			

			#else  
			//��LSE�޷������������ڲ�LSI
			/* ʹ��LSI*/
			RCC_LSICmd(ENABLE);
			
			/* ����LSI�Ƿ���Ч*/  
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

			/* ѡ��LSI��ΪRTC��Ӳ��ʱ��Դ*/
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

			#endif

			/* Enable the RTC Clock��ʹ��RTCʱ�� */
			RCC_RTCCLKCmd(ENABLE);
			
			/* Wait for RTC APB registers synchronisation���ȴ�RTC��ؼĴ������� */
			RTC_WaitForSynchro();
			
			#if 0 //LSE
			/* Configure the RTC data register and RTC prescaler������RTC���ݼĴ�����RTC�ķ�Ƶֵ */
			RTC_InitStructure.RTC_AsynchPrediv = 127;				//�첽��Ƶϵ��
			RTC_InitStructure.RTC_SynchPrediv = 255;				//ͬ����Ƶϵ��
			RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
			RTC_Init(&RTC_InitStructure);
			#else //LSI
			/* Configure the RTC data register and RTC prescaler������RTC���ݼĴ�����RTC�ķ�Ƶֵ */
			RTC_InitStructure.RTC_AsynchPrediv = 127;				//�첽��Ƶϵ��
			RTC_InitStructure.RTC_SynchPrediv = 249;				//ͬ����Ƶϵ��
			RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
			RTC_Init(&RTC_InitStructure);

			#endif
			//�رջ��ѹ���
			RTC_WakeUpCmd(DISABLE);
			
			//Ϊ���ѹ���ѡ��RTC���úõ�ʱ��Դ
			RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
			
			//���û��Ѽ���ֵΪ�Զ����أ�д��ֵĬ����0
			RTC_SetWakeUpCounter(1-1);
			
			//���RTC�����жϱ�־
			RTC_ClearITPendingBit(RTC_IT_WUT);
			
			//ʹ��RTC�����ж�
			RTC_ITConfig(RTC_IT_WUT, ENABLE);

			//ʹ�ܻ��ѹ���
			RTC_WakeUpCmd(ENABLE);

			/* Configure EXTI Line22�������ⲿ�жϿ�����22 */
			EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//��ǰʹ���ⲿ�жϿ�����22
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽ
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//�����ش����ж� 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//ʹ���ⲿ�жϿ�����22
			EXTI_Init(&EXTI_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;				//����RTC�����жϴ���
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�Ϊ0x3
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//��Ӧ���ȼ�Ϊ0x3
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ��
			NVIC_Init(&NVIC_InitStructure);	
		}
}

void RTC_ALARM_A_INIT(void)
{
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours 	 = 0x19;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x09;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0X00;
	
	#if 0
	//RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	//RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;	//�������ں����ڣ���������ÿ�춼��Ч
	#endif
	
	#if 0
	//ָ��1����Ч
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel_Date = 0x01;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel 		= RTC_AlarmDateWeekDaySel_Date;	//����ָ��������Ч
	RTC_AlarmStructure.RTC_AlarmMask 				= RTC_AlarmMask_None;
	#endif
	
	#if 1
	//ָ����������Ч
	RTC_AlarmStructure.RTC_AlarmDateWeekDay 	= RTC_Weekday_Sunday;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel 	= RTC_AlarmDateWeekDaySel_WeekDay;//ָ��������Ч
	RTC_AlarmStructure.RTC_AlarmMask 			= RTC_AlarmMask_None;	//���������ں����ڣ��������ں����ڵ�ָ��
	#endif
	
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStructure);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	/*Enable RTC Alarm A Interrupt*/
	/*ʹ������A�ж�*/
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
	
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 					 = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd  			     = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}




