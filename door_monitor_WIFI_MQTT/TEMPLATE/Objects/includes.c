#include "includes.h"
#include "bmp.h"
#include "PAJ7620.h"

/*����ѡ���־��*/
#define SFM_UNLOCK '1'
#define PASSWORD_UNLOCK '2'
#define RFID_UNLOCK '3'
#define HAND_STATE_UNLOCK '4'
#define GESTURE_SIZE sizeof(gesture_arry)/2

/*Ĭ�ϳ�ʼ����������*/
static const char password[] = "603328";

//����ʶ���ʼ������
static const unsigned char gesture_arry[][2]={
	
	{0xEF,0x00},
	{0x41,0x00},
	{0x42,0x00},
	{0xEF,0x00},
	{0x48,0x3C},
	{0x49,0x00},
	{0x51,0x10},
	{0x83,0x20},
	{0x9F,0xF9},
	{0xEF,0x01},
	{0x01,0x1E},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x41,0x40},
	{0x43,0x30},
	{0x65,0x96},
	{0x66,0x00},
	{0x67,0x97},
	{0x68,0x01},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6B,0xB0},
	{0x6C,0x04},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x74,0x00},
	{0xEF,0x00},
	{0x41,0xFF},
	{0x42,0x01},

};

/*LED��˸�ӷ���������*/
void display(void)
{
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 0;
		delay_xms(800);
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 1;
		
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 0;
		delay_xms(800);
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 1;
		
		/**/
		PFout(8) = 1;delay_xms(300);PFout(8) = 0;
		PFout(8) = 1;delay_xms(300);PFout(8) = 0;

}

/*����Աģʽ�µ�ָ�Ʋ�������*/

//���ָ��
void add_sfm_admin(void)
{
		int32_t rt;
		uint16_t id=1;
		uint32_t timeout=0;
		
		printf("\r\n\r\n=====================================\r\n\r\n");
		printf("ִ�����ָ�Ʋ���,�뽫��ָ�ŵ�ָ��ģ�鴥����Ӧ��\r\n");
		timeout=0;
			
		/* ��ʾ��ɫ */
		sfm_ctrl_led(0x06,0x06,0x32);
			
		while((sfm_touch_check()!=SFM_ACK_SUCCESS) && (timeout<10))
		{
			timeout++;
		}
			
		if(timeout>=10)
		{
			printf("û�м�⵽��ָ�������²���!\r\n");
			
			/* �ָ���Ȧȫ��->ȫ������2�� */
			sfm_ctrl_led(0x00,0x07,0xC8);
			
			return ;
		}
			
		printf("��⵽��ָ�����ڿ�ʼ���ָ��...\r\n");
			
		/* ��ȡδʹ�õ��û�id */
		rt = sfm_get_unused_id(&id);
			
		if(rt != SFM_ACK_SUCCESS)
		{
			printf("��ȡδʹ�õ��û�id %s\r\n",sfm_error_code(rt));
			
			/* �ָ���Ȧȫ��->ȫ������2�� */
			sfm_ctrl_led(0x00,0x07,0xC8);				
			
			return ;
		}
		
		printf("��ʹ�õ��û�idΪ%d\r\n",id);	
			
		rt=sfm_reg_user(id);
		
		if(rt == SFM_ACK_SUCCESS)
		{
			/* �ɹ�:��Ȧ��ʾ��ɫ */
			sfm_ctrl_led(0x05,0x05,0x32);			
			delay_ms(1000);
			
			/* �ɹ�����������һ��ʾ�� */
			PFout(8) = 1;delay_ms(50);PFout(8) = 0;					
			
		}
		else
		{
			/* ʧ��:��Ȧ��ʾ��ɫ */
			sfm_ctrl_led(0x03,0x03,0x32);
			
			delay_ms(1000);			
		}
		
		
		printf("���ָ�� %s\r\n",sfm_error_code(rt));
		
		/* �ָ���Ȧȫ��->ȫ������2�� */
		sfm_ctrl_led(0x00,0x07,0xC8);		

}


//��ȡָ��
void get_sfm_admin(void)
{
		int32_t rt;
		uint16_t user_total;
		printf("\r\n\r\n=====================================\r\n\r\n");

		rt=sfm_get_user_total(&user_total);

		printf("����ָ��ģ���û����� %s %d \r\n",sfm_error_code(rt),user_total);
	
		if(rt == SFM_ACK_SUCCESS)
		{
			/* �ɹ�����������һ��ʾ�� */
			PFout(8) = 1;delay_xms(50);PFout(8) = 0;
		}		

}

//ɾ��ָ��
void del_sfm_admin(void)
{
		int32_t rt;
		printf("\r\n\r\n=====================================\r\n\r\n");

		rt = sfm_del_user_all();

		printf("ɾ�������û� %s\r\n",sfm_error_code(rt));
		
		if(rt == SFM_ACK_SUCCESS)
		{
			/* �ɹ�����������һ��ʾ�� */
			PFout(8) = 1;delay_xms(50);PFout(8) = 0;			
		}
		
		printf("ɾ���û��ɹ�\r\n");			
}

/*�������ݼ�¼*/
int search_unlock_record_admin(int lock_chose,int lock_character)
{
	//��ȡ����
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
	//��ȡʱ��
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);

	//��鵱ǰ�����Ƿ�С��100��
	if(dht11_rec_cnt < 100)
	{
		//��ȡ����
		RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);

		//��ȡʱ��
		RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);

		//��ʽ���ַ���,ĩβ���\r\n��Ϊһ��������ǣ��������Ƕ�ȡʱ������ж�
		sprintf((char *)buf, "[%03d]20%02x/%02x/%02x %02x:%02x:%02x %s %s\r\n", \
				dht11_rec_cnt,\
				RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,\
				RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds,\
				unlock_record_npc[lock_character],lock_func_chose[lock_chose]);
		
		/*����FLASH���������FLASH*/
		FLASH_Unlock();

		/* �����Ӧ�ı�־λ*/  
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
						FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 	
		
		//д�������¼
		if(0 == flash_write_record(buf,dht11_rec_cnt))
		{
			//��ʾ
			printf(buf);

			//��¼�Լ�1
			dht11_rec_cnt++;
		}else{

			//���ݼ�¼����,��ͷ��ʼ�洢����
			dht11_rec_cnt = 0;
		}
		
	}else{
		printf("�ѳ���100��������¼!������\r\n");
		
	}
		
	
	return 0;
}



/*������ģ���ʼ��*/
void INIT_FUNC(void)
{
	int rt;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	Init_LED();
	beep_init();
	
	/*��ʼ������1*/
	usart1_init(115200);
	
	/* ����2 ==>ָ�Ƴ�ʼ�� */
	usart2_init(115200);
	
	/*��ʼ��MFRC522*/
	MFRC522_Initializtion();	
	
	/*��ʪ��*/
	DHT11_INIT(1);
	OLED_Init();
	OLED_Clear();
	delay_xms(500);
	
	//�����ӳ�һ�ᣬȷ��оƬ�ڲ����ȫ����ʼ��,printf���������
	delay_ms(500);
		
	//��������
	printf("This is esp8266 mqtt with aliyun test by teacher.wen\r\n");
	
	while(esp8266_mqtt_init())
	{
		printf("esp8266_mqtt_init ...");
		
		delay_ms(1000);
	}
	
	//��������������D1����˸���Σ�ʾ�����ӳɹ�
	 PFout(9)=0;delay_ms(100);
	 PFout(9)=1;delay_ms(100);	
	 PFout(9)=0;delay_ms(100);
	 PFout(9)=1;delay_ms(100);

	printf("esp8266 connect aliyun with mqtt success\r\n");	
	
	/*�������*/
	matrix_key_init();
	key_init();
	printf("������̳�ʼ������\r\n");
	
	/*���ģ��������ʼ��*/
	sg_init();
	sg_angle(45);
	printf("�����ʼ������\r\n");
	
	/* ������ָ��ģ��������� */
	while(SFM_ACK_SUCCESS!=sfm_init(115200))
	{
		delay_ms(500);
	
		printf("����ָ��ģ�������� ...\r\n");		
	}
	printf("����ָ��ģ����������\r\n");
	
	//PAJ7620U2��������ʼ��
 	while(!paj7620u2_init())
	{
	    printf("����ʶ���ʼ���쳣!!!\r\n");
		delay_ms(500);
	}
	
    printf("����ʶ���ʼ���ɹ� OK\r\n");
	RTC_RST_CONFIG();
}

/*	����������ʾ  */
void Display_start(void)
{
		uint8_t date[64] = {0};
		
		//��ȡ����
		RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
		sprintf((char *)date,"20%02x/%02x/%02x",
				RTC_DateStructure.RTC_Year,
				RTC_DateStructure.RTC_Month,
				RTC_DateStructure.RTC_Date);
		printf("\r\n");

		//��������
		OLED_Clear();//�ر���ʾ
		OLED_ShowCHinese(32,0,37);//��
		OLED_ShowCHinese(50,0,38);//ӭ
		OLED_ShowCHinese(68,0,39);//ʹ
		OLED_ShowCHinese(86,0,40);//��	
		
		OLED_ShowCHinese(23,2,6);//��
		OLED_ShowCHinese(41,2,7);//��
		OLED_ShowCHinese(59,2,8);//ָ
		OLED_ShowCHinese(77,2,9);//��
		OLED_ShowCHinese(95,2,5);//��
		
		OLED_ShowCHinese(38,4,10);//��
		OLED_ShowCHinese(56,4,11);//ܲ
		OLED_ShowCHinese(74,4,12);//��
		
		OLED_ShowString(20,6,date,16);
		memset(date,0,sizeof date);
		
		delay_ms(3000);
		OLED_Clear();//�ر���ʾ
		OLED_DrawBMP(0,0,128,8,start_init);//����������ͼ���ʼ
		delay_ms(700);		
		//�����˽�����
		OLED_DrawBMP(0,0,128,8,start_sta1);//����������ͼ��0
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta2);//����������ͼ��1	
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta3);//����������ͼ��2	
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta4);//����������ͼ��3
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta5);//����������ͼ��4	
		delay_ms(400);
		OLED_Clear();//�ر���ʾ
		
}

/*���ܲ˵�*/
void Display_menu(void)
{
	
	uint8_t date[64] = {0};
	uint8_t time[64] = {0};
	//��ȡ����
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
	sprintf((char *)date,"20%02x/%02x/%02x",
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month,
			RTC_DateStructure.RTC_Date);
	
	
	OLED_ShowString(24,0,date,16);
	
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);
	sprintf((char *)time,"%02x:%02x:%02x",
			RTC_TimeStructure.RTC_Hours,
			RTC_TimeStructure.RTC_Minutes,
			RTC_TimeStructure.RTC_Seconds);
	
	delay_ms(100);
	OLED_ShowString(32,2,time,16);
	
	OLED_ShowCHinese(16,4,104);	//��
	OLED_ShowCHinese(32,4,117);
	OLED_ShowCHinese(48,4,107);
	OLED_ShowCHinese(64,4,108);
	OLED_ShowCHinese(80,4,109);
	OLED_ShowCHinese(96,4,110);
	
	OLED_ShowCHinese(10,6,118);
	OLED_ShowCHinese(26,6,119);
	OLED_ShowCHinese(42,6,120);
	OLED_ShowCHinese(58,6,144);
	OLED_ShowCHinese(74,6,8);
	OLED_ShowCHinese(90,6,145);
	OLED_ShowString(106,6,"..",14);
	
}

/*����ѡ�����*/
void Display_func(void)
{
	OLED_Clear();
	OLED_DrawBMP(0,0,50,7,lock);
	OLED_ShowChar(53,0,'1',12);	
	OLED_ShowCHinese(65,0,83);
	OLED_ShowCHinese(81,0,84);
	OLED_ShowCHinese(97,0,4);
	OLED_ShowCHinese(113,0,5);
	
	OLED_ShowChar(53,3,'2',12);
	OLED_ShowCHinese(65,3,20);
	OLED_ShowCHinese(81,3,21);
	OLED_ShowCHinese(97,3,4);
	OLED_ShowCHinese(113,3,5);
	OLED_ShowChar(53,6,'3',12);
	OLED_ShowCHinese(65,6,113);
	OLED_ShowCHinese(81,6,114);
	OLED_ShowCHinese(97,6,115);
	OLED_ShowCHinese(113,6,116);
	while(1)
	{
		/*��������*/
		int32_t rt;
		uint16_t id=1;
		uint32_t timeout=0;
		static char flag = 0;
		char num[16] = {0};
		int32_t card_flag = 0;
		int32_t j = 0;
		int32_t hand_rt = 0;
		
		
		flag = get_key_status();
		Gesture_test();
		sg_angle(0);
		switch(flag)
		{
			case '*':
			printf("������һ���˵�\r\n");	
			break;
			/*����ʶ��*/
			case HAND_STATE_UNLOCK:
				printf("���ѽ�������ʶ��ģʽ\r\n");
				OLED_Clear();
				OLED_DrawBMP(0,0,64,8,hand);
				OLED_ShowCHinese(60,2,135);
				OLED_ShowCHinese(76,2,136);
				OLED_ShowCHinese(92,2,137);
				OLED_ShowCHinese(108,2,138);

				while(hand_rt == 0)
				{
					hand_rt = Gesture_test();	
				}
					
				if(hand_rt == 2 || hand_rt == 3)
				{
					OLED_Clear();
					OLED_DrawBMP(0,0,64,8,hand);
					OLED_ShowCHinese(60,0,141);
					OLED_ShowCHinese(76,0,142);
					OLED_ShowCHinese(92,0,135);
					OLED_ShowCHinese(108,0,136);
					delay_xms(50);
					OLED_ShowCHinese(60,2,135);
					OLED_ShowCHinese(76,2,136);
					OLED_ShowCHinese(92,2,53);
					OLED_ShowCHinese(108,2,79);
					
					
					delay_xms(50);
					
					display();
					display();
					
					/*ģ�⿪��*/
					sg_init();
					sg_angle(0);
					sg_angle(45);
					sg_angle(135);
					sg_angle(180);
					
					OLED_ShowCHinese(60,4,4);
					OLED_ShowCHinese(76,4,2);
					OLED_ShowCHinese(92,4,64);
					OLED_ShowCHinese(108,4,65);
					printf("����ʶ�����ɹ�\r\n");
					search_unlock_record_admin(4,rand()%4);
				}else if(hand_rt < 0)
				{
					OLED_Clear();
					OLED_DrawBMP(0,0,64,8,hand);
					OLED_ShowCHinese(62,2,8);
					OLED_ShowCHinese(78,2,9);
					OLED_ShowCHinese(60,2,135);
					OLED_ShowCHinese(76,2,136);
					
					OLED_ShowString(63,4,"*",16);
					OLED_ShowCHinese(77,4,33);
					OLED_ShowCHinese(93,4,34);
				}
				
	
			break;
			
			/*ָ�ƽ���*/
			case SFM_UNLOCK:
				OLED_Clear();
				OLED_DrawBMP(0,0,64,6,touch);
				OLED_ShowCHinese(62,2,26);
				OLED_ShowCHinese(78,2,113);
				OLED_ShowCHinese(94,2,8);
				OLED_ShowCHinese(110,2,9);
				
				printf("\r\n\r\n=====================================\r\n\r\n");
				printf("ִ��ˢָ�Ʋ���,�뽫��ָ�ŵ�ָ��ģ�鴥����Ӧ��\r\n");
				timeout=0;
			
				/* ��ʾ��ɫ */
				sfm_ctrl_led(0x06,0x06,0x32);
				
				while((sfm_touch_check()!=SFM_ACK_SUCCESS) && (timeout<10))
				{
					timeout++;
				}
				
				if(timeout>=10)
				{
					printf("û�м�⵽��ָ�������²���!\r\n");
					
					/* �ָ���Ȧȫ��->ȫ������2�� */
					sfm_ctrl_led(0x00,0x07,0xC8);
					
					OLED_Clear();
					OLED_DrawBMP(0,0,64,6,touch);
					OLED_ShowCHinese(62,2,8);
					OLED_ShowCHinese(78,2,9);
					OLED_ShowCHinese(94,2,120);
					OLED_ShowCHinese(110,2,121);
					OLED_ShowString(63,4,"*",16);
					OLED_ShowCHinese(77,4,33);
					OLED_ShowCHinese(93,4,34);
					PFout(8) = 1;delay_ms(500);PFout(8) = 0;
					continue;
				}
				
				printf("��⵽��ָ�����ڿ�ʼˢָ��...\r\n");
				
				rt=sfm_compare_users(&id);
				
				if(rt == SFM_ACK_SUCCESS)
				{
					/* �ɹ�:��Ȧ��ʾ��ɫ */
					sfm_ctrl_led(0x05,0x05,0x32);
					
					delay_ms(1000);
					
					/* �ɹ�����������һ��ʾ�� */
					PFout(8) = 1;delay_ms(50);PFout(8) = 0;						
				}
				else
				{
					/* ʧ��:��Ȧ��ʾ��ɫ */
					sfm_ctrl_led(0x03,0x03,0x32);
					
					OLED_Clear();
					OLED_DrawBMP(0,0,64,6,touch);
					OLED_ShowCHinese(62,2,8);
					OLED_ShowCHinese(78,2,9);
					OLED_ShowCHinese(94,2,120);
					OLED_ShowCHinese(110,2,121);
					OLED_ShowString(63,4,"*",16);
					OLED_ShowCHinese(77,4,33);
					OLED_ShowCHinese(93,4,34);
					PFout(8) = 1;delay_ms(500);PFout(8) = 0;
					
					/* ��idΪ0����ȶԲ��ɹ���*/
					if(id == 0)
					{
						printf("ָ�ƱȶԲ��ɹ�\r\n");
					}
					
					/* �ָ���Ȧȫ��->ȫ������2�� */
					sfm_ctrl_led(0x00,0x07,0xC8);
					
					delay_ms(1000);		
					continue;
				}
				
				/* ��idΪ0����ȶԲ��ɹ���*/
				printf("ˢָ�� %s ʶ��id=%d\r\n",sfm_error_code(rt),id);
				
				/* �ָ���Ȧȫ��->ȫ������2�� */
				sfm_ctrl_led(0x00,0x07,0xC8);
				
				/*ʶ��ɹ�*/
				display();
				
				/*ģ�⿪��*/
				sg_init();
				sg_angle(0);
				sg_angle(45);
				sg_angle(135);
				sg_angle(180);
				
				search_unlock_record_admin(0,id-1);
				printf("ָ�ƽ����ɹ�\r\n");
				
				OLED_Clear();
				OLED_DrawBMP(0,0,64,6,touch);
				OLED_ShowCHinese(62,2,4);
				OLED_ShowCHinese(78,2,2);
				OLED_ShowCHinese(94,2,64);
				OLED_ShowCHinese(110,2,65);
				
				
			break;
			
			/*�������*/
			case PASSWORD_UNLOCK:
				
				OLED_Clear();
				OLED_ShowCHinese(18,0,26);
				OLED_ShowCHinese(36,0,27);
				OLED_ShowCHinese(54,0,28);
				OLED_ShowCHinese(72,0,20);
				OLED_ShowCHinese(90,0,21);
				OLED_ShowString(18,3,"  -------- ",12);
				OLED_ShowString(0,6,"#:",12);
				OLED_ShowCHinese(14,6,29);
				OLED_ShowCHinese(28,6,30);
				OLED_ShowString(56,6,"C:",12);		
				OLED_ShowCHinese(70,6,33);		
				OLED_ShowCHinese(84,6,34);			
				
				for(j = 0;j < 16;j++)
				{
					key_state = 'n';
					while(key_state == 'n')
					{
						key_state = loop_get_key_sta();
					}
					if(key_state == 'C')	
					{
						OLED_Clear();
						OLED_ShowString(16,3,"*",12);
						OLED_ShowCHinese(32,3,29);
						OLED_ShowCHinese(48,3,30);
						OLED_ShowCHinese(64,3,33);
						OLED_ShowCHinese(80,3,34);
						goto end_of_loop;
					}
					if(key_state == '#')	{break;}
					switch(j)
					{
						case 0:
						OLED_ShowString(18,3," *------- ",12);
						break;
						case 1:
						OLED_ShowString(18,3," **------ ",12);
						break;
						case 2:
						OLED_ShowString(18,3," ***----- ",12);						
						break;
						case 3:
						OLED_ShowString(18,3," ****---- ",12);
						break;
						case 4:
						OLED_ShowString(18,3," *****--- ",12);
						break;
						case 5:
						OLED_ShowString(18,3," ******-- ",12);
						break;
						case 6:
						OLED_ShowString(18,3," *******- ",12);
						break;
						case 7:
						OLED_ShowString(18,3," ******** ",12);
						break;
					}
					num[j] = key_state;
					printf("%c\r\n",key_state);
				}num[j] = '\0';
				
				printf("�������������Ϊ %s\r\n",num);
				/*������ȷ����*/
				if(strstr(password,num))
				{
					OLED_Clear();
					OLED_DrawBMP(0,0,64,7,house);
					OLED_ShowCHinese(65,2,27);
					OLED_ShowCHinese(81,2,28);
					OLED_ShowCHinese(97,2,53);
					OLED_ShowCHinese(113,2,79);
					
					display();
					sg_init();
					sg_angle(0);
					sg_angle(45);
					sg_angle(135);
					sg_angle(180);
					delay_xms(200);
					
					search_unlock_record_admin(1,rand()%4);
					OLED_ShowString(68,4,"*",16);
					OLED_ShowCHinese(84,4,33);
					OLED_ShowCHinese(100,4,34);
					
					
				}else{
					
					OLED_Clear();
					OLED_DrawBMP(0,0,64,7,house);
					OLED_ShowCHinese(65,2,20);
					OLED_ShowCHinese(81,2,21);
					OLED_ShowCHinese(97,2,35);
					OLED_ShowCHinese(113,2,36);
					display();
					OLED_ShowString(68,4,"*",16);
					OLED_ShowCHinese(84,4,33);
					OLED_ShowCHinese(100,4,34);
				}
				
				
			break;

			/*RFID����*/
			case RFID_UNLOCK:
				MFRC522_Initializtion();	
				OLED_Clear();
				delay_xms(10);
				OLED_DrawBMP(0,0,60,4,RFID);
				OLED_ShowCHinese(66,3,26);
				OLED_ShowCHinese(82,3,113);
				OLED_ShowCHinese(98,3,116);
				while(card_flag == 0)
				{
					card_flag = MFRC522Test();
				}
				if(card_flag == 1 || card_flag == 2)
				{
					sg_init();
					sg_angle(0);
					sg_angle(45);
					sg_angle(135);
					sg_angle(180);
					
					printf("ˢ���ɹ�\r\n");
					printf("���ڱ����¼\r\n");
					
					switch(card_flag)
					{
						case 1:search_unlock_record_admin(2,2);break;	//����
						case 2:search_unlock_record_admin(2,3);break;	//Ů��
					}
					
					OLED_Clear();
					delay_xms(300);
					OLED_DrawBMP(0,0,60,4,RFID);
					OLED_ShowCHinese(64,1,113);
					OLED_ShowCHinese(80,1,116);
					OLED_ShowCHinese(96,1,24);
					OLED_ShowCHinese(112,1,25);
					OLED_ShowCHinese(68,3,14);
					OLED_ShowCHinese(84,3,4);
					OLED_ShowCHinese(100,3,2);
					display();
				
				}else{
					OLED_Clear();
					printf("ˢ��ʧ��\r\n");
					delay_xms(300);
					OLED_DrawBMP(0,0,60,4,RFID);
					OLED_ShowCHinese(64,1,113);
					OLED_ShowCHinese(80,1,116);
					OLED_ShowCHinese(96,1,120);
					OLED_ShowCHinese(112,1,121);
					OLED_ShowString(68,3,"*",16);
					OLED_ShowCHinese(84,3,33);
					OLED_ShowCHinese(100,3,34);
				
					display();
					display();
				
				}
			break;			
		}
	end_of_loop:
		if(flag == '*')	{OLED_Clear();break;}

	}
}


/*����ʶ��*/
int Gesture_test(void)
{
	uint8_t i;
    uint8_t status;

	uint8_t data[2]={0x00};
	uint16_t gesture_data;
	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	i=0;

	//��ȡ����״̬			
	status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);
	
	if(!status)
	{   
		gesture_data =(uint16_t)data[1]<<8 | data[0];
		
		if(gesture_data) 
		{
			switch(gesture_data)
			{
				
				case GES_CLOCKWISE:             
							   printf("��⵽����˳ʱ��\r\n");     return 1;
				case GES_COUNT_CLOCKWISE:   
							   printf("��⵽������ʱ��\r\n"); return 2; //��ʱ��

				default:   return -1;
				
			}

			
		}
	}
	
	delay_ms(50);
	return 0;
}

void display_mqtt_unlock(void)
{
	PFout(9)=0;//���Ƶ���
							
	OLED_Clear();
	OLED_DrawBMP(0,0,64,7,wifi);
	OLED_ShowCHinese(60,2,120);
	OLED_ShowCHinese(76,2,144);
	OLED_ShowCHinese(92,2,4);
	OLED_ShowCHinese(108,2,2);
	OLED_ShowCHinese(64,4,13 );
	
	display();
	display();
	
	sg_angle(0);
	sg_angle(45);
	sg_angle(135);
	sg_angle(180);
	
	delay_xms(40);
	OLED_Clear();
	OLED_DrawBMP(0,0,64,7,wifi);
	OLED_ShowCHinese(64,3,12);
	OLED_ShowCHinese(80,3,144);
	OLED_ShowCHinese(96,3,4);
	OLED_ShowCHinese(112,3,2);
	OLED_ShowCHinese(64,6,24);
	OLED_ShowCHinese(80,6,25);
	
	OLED_Clear();
	printf("���߽����ɹ�\r\n");
	search_unlock_record_admin(3,rand()%4);
}

/*���Ź���־�ж�*/
void WDG_RST_FLAG(void)
{
		if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)      
		{													
			printf("IWDG reset cpu\r\n");					
		}													
		else if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) 
		{													
			printf("WWDG reset cpu\r\n");					
		}													
		else												
		{													
			printf("normal reset cpu\r\n");					
		}													
}


/*��������ת��ʵ��*/
void usart_str_load(uint8_t *str_buf,uint32_t *usart_event,uint32_t *usart_cnt)
{
	static uint32_t i = 0;
	char *p;

	printf("str_buf = %s\r\n",str_buf);
	
	if(strstr((const char *)str_buf,"UNLOCK"))
	{
		OLED_Clear();
		OLED_DrawBMP(0,0,64,7,wifi);
		OLED_ShowCHinese(60,2,120);
		OLED_ShowCHinese(76,2,144);
		OLED_ShowCHinese(92,2,4);
		OLED_ShowCHinese(108,2,2);
		OLED_ShowCHinese(64,4,13 );
		
		display();
		display();
		
		sg_angle(0);
		sg_angle(45);
		sg_angle(135);
		sg_angle(180);
		
		delay_xms(40);
		OLED_Clear();
		OLED_DrawBMP(0,0,64,7,wifi);
		OLED_ShowCHinese(64,3,12);
		OLED_ShowCHinese(80,3,144);
		OLED_ShowCHinese(96,3,4);
		OLED_ShowCHinese(112,3,2);
		OLED_ShowCHinese(64,6,24);
		OLED_ShowCHinese(80,6,25);
		
		OLED_Clear();
		printf("���߽����ɹ�\r\n");
		search_unlock_record_admin(rand()%4,3);
		
	}
	
	if(strstr((const char *)str_buf,"RECORD_PRINT"))
	{
		/*����FLASH���������FLASH*/
		FLASH_Unlock();

		/* �����Ӧ�ı�־λ*/  
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
						FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 	
		
		printf("�����ǽ�����¼\r\n");
		//���Ի�ȡ100����¼
		for(i = 0;i < 100;i++)
		{
			//��ȡ�洢��¼
			flash_read_record(buf,i);

			//����¼�Ƿ���ڻ��з��ţ��������򲻴�ӡ���
			if(strstr(buf,"\n") == 0)
				
				break;

			//��ӡ��¼
			printf(buf);
		}

		//���i = 0������û��һ����¼
		if(i == 0)
		{
			printf("û�п�����¼\r\n");
		}
	}
	
	
	if(strstr((const char *)str_buf,"DATE SET"))
	{
		//�ԵȺŷָ��ַ���\r\n
		strtok((char *)str_buf,"-");

		//��ȡ��
		p=strtok(NULL,"-");
		//2017-2000=17 
		i = atoi(p)-2000;
		//ת��Ϊ16���� 17 ->0x17
		i = (i/10)*16+i%10;
		RTC_DateStructure.RTC_Year = i;
		

		//��ȡ��
		p=strtok(NULL,"-");
		i= atoi(p);
		//ת��Ϊ16����
		i= (i/10)*16+i%10;						
		RTC_DateStructure.RTC_Month=i;

		p=strtok(NULL,"-");
		i=atoi(p);
		//ת��Ϊ16����
		i= (i/10)*16+i%10;		
		RTC_DateStructure.RTC_Date = i;
		printf("%x\r\n",i);
		//��ȡ����
		p=strtok(NULL,"-");
		i=atoi(p);
		//ת��Ϊ16����
		i= (i/10)*16+i%10;						
		RTC_DateStructure.RTC_WeekDay = i;
		printf("%x\r\n",i);
		//��������
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		printf("�����޸ĳɹ�\r\n");
		
		printf("��ǰ����Ϊ %x/%x/%x \r\n",RTC_DateStructure.RTC_Year,
										 RTC_DateStructure.RTC_Month,
										 RTC_DateStructure.RTC_Date);
	}													

	/* ����ʱ�� */
	if(strstr((const char *)str_buf,"TIME SET"))
	{
		//�ԵȺŷָ��ַ���
		strtok((char *)str_buf,"-");
		 
		//��ȡʱ
		p=strtok(NULL,"-");
		i = atoi(p);
		
		//ͨ��ʱ���ж���AM����PM
		if(i<12)
			RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
		else
			RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
			
		//ת��Ϊ16����
		i= (i/10)*16+i%10;
		RTC_TimeStructure.RTC_Hours   = i;
		
		//��ȡ��
		p=strtok(NULL,"-");
		i = atoi(p);						
		//ת��Ϊ16����
		i= (i/10)*16+i%10;	
		RTC_TimeStructure.RTC_Minutes = i;
		
		//��ȡ��
		p=strtok(NULL,"-");
		i = atoi(p);						
		//ת��Ϊ16����
		i= (i/10)*16+i%10;					
		RTC_TimeStructure.RTC_Seconds = i; 					
		
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); 
		
		printf("ʱ���޸ĳɹ�\r\n");
		
		printf("��ǰʱ��Ϊ %x/%x/%x \r\n",RTC_TimeStructure.RTC_Hours,
										 RTC_TimeStructure.RTC_Minutes,
										 RTC_TimeStructure.RTC_Seconds);
	}
	
	/*��ʪ�����ݲɼ�*/
	if(strstr((const char *)str_buf,"start"))
	{
		/*��ʼ��ʪ�����ݲɼ�*/
		g_dht11_start = 1;
		
		printf("������ʪ�ȵ����ݲɼ�\r\n");
	}

	/*�жϽ��յ����ַ���Ϊֹͣ*/
	if(strstr((const char *)str_buf,"stop") )
	{
		//ֹͣ��ʪ�����ݲɼ�
		g_dht11_start = 0;

		printf("ֹͣ��ʪ�ȵ����ݲɼ�\r\n");
	}

	/*���*/
	if(strstr((const char *)str_buf,"CLEAR_RECORD") )
	{
		//������м�¼
		printf("������м�¼......\r\n");

		//��������
		flash_erase_record();

		printf("���м�¼����ɾ��\r\n");

		//�����¼����ֵ
		dht11_rec_cnt = 0;

	}

	
	//��մ���1���ݼ���ֵ
	*usart_cnt = 0;			
	
	//��մ���1���������¼�
	*usart_event = 0;
	
	//��մ���1�������ݻ�����
	memset((char *)str_buf,0,50);
	
}


void flash_read_record(char *pbuf,uint32_t record_count)
{
	uint32_t addr_start=ADDR_FLASH_SECTOR_4+record_count*64;
	uint32_t addr_end  =addr_start+64;

	uint32_t i=0;
	
	while (addr_start < addr_end)
	{
		*(uint32_t *)&pbuf[i] = *(__IO uint32_t*)addr_start;

		addr_start+=4;
		
		i = i + 4;
	}

}

void flash_erase_record(void)
{
	
	/* �����������д���ʧ�ܣ���ȡ�����ݶ�Ϊ0 */
	printf("FLASH_EraseSector start\r\n");
	
	if (FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
	{ 
			printf("Erase error\r\n");
			return;
	}

	printf("FLASH_EraseSector ends\r\n");

}

uint32_t flash_write_record(char *pbuf,uint32_t record_count)
{
	uint32_t addr_start=ADDR_FLASH_SECTOR_4+record_count*64;
	uint32_t addr_end  =addr_start+64;

	uint32_t i=0;
	
	while (addr_start < addr_end)
	{
		//ÿ��д��������4���ֽ�
		if (FLASH_ProgramWord(addr_start, *(uint32_t *)&pbuf[i]) == FLASH_COMPLETE)
		{
			//��ַÿ��ƫ��4���ֽ�
			addr_start +=4;
			
			i+=4;
		}

		else
		{ 
			printf("flash write record fail,now goto erase sector!\r\n");
			
			//���²�������
			flash_erase_record();

			return 1;
		}
	}
	return 0;
}


