#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/*��׼C��*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/*λ������*/
#include "sys.h"

/*����Ӳ�����*/
#include "init_led.h"
#include "key.h"
#include "EXTI.h"
#include "beep.h"
#include "init_USART.h"
#include "delay.h"			
#include "Tim_init.h"		
#include "WWDG.h"
#include "RTC.h"
#include "ADC.h"
#include "SPI.h"
#include "I2C.h"
#include "flash.h"

/*������ع���*/
#include "DHT11.h"		//��ʪ��
#include "OLED.h"		//OLED
#include "PAJ7620.h"
#include "MFRC522.h"	//RFID
#include "sg90.h"		//���
#include "sfm.h"		//ָ��ģ��


/*����1����ر���*/
static uint8_t  g_usart1_buf[64] = {0};	//����1�Ļ�����
static uint32_t g_usart1_cnt 	  		  = 0;		//����1ͳ�ƴ�����ַ�����
static uint32_t g_usart1_event            = 0;		//����1ͳ���¼�	

/*RTCʱ�ӱ���*/
static volatile uint32_t g_rtc_wakeup_event  = 0;	//RTCʱ�ӻ����¼���־
static volatile uint32_t g_rtc_alarm_a_event = 0;	//RTC����A�����¼���־

/*�¶ȼ�¼�¼�*/
static volatile uint32_t DHT11_buf[4] = {0}; 
static volatile uint32_t g_tim_DHT11_event = 0;
static volatile uint32_t g_dht11_start=0;

/*������ر���*/
static uint32_t g_ble_event = 0;					//����ͳ���¼�
static uint32_t g_ble_cnt   = 0;					//����ͳ�ƴ�����ַ�����
static uint8_t g_ble_buf[50] = {0};		//����������

static uint8_t 	dht11_data[4];
static uint32_t	dht11_rec_cnt=0;

static uint32_t key_sta = 0;						//����Աģʽʶ��
static uint32_t key_sta_back = 0;
static char key_state = 'n';
/*��ʼ���ṹ��*/
static RTC_DateTypeDef  	 RTC_DateStructure;
static RTC_TimeTypeDef  	 RTC_TimeStructure;
static GPIO_InitTypeDef 	 GPIO_InitStructure;
static EXTI_InitTypeDef 	 EXTI_InitStructure;
static ADC_InitTypeDef       ADC_InitStructure;
static ADC_CommonInitTypeDef ADC_CommonInitStructure;
static NVIC_InitTypeDef 	 NVIC_InitStructure;

/*�������*/
static uint32_t uwStartSector 	= 0;
static uint32_t uwEndSector 	= 0;
static uint32_t uwSectorCounter = 0;

/*����Աģʽ*/
static uint32_t EXTI_flag = 0;
static char unlock_record_npc[][10] = {{"�ְ�"},{"����"},{"����"},{"Ů��"}};
static char lock_func_chose[][15] = {{"ָ��"},{"����"},{"�Ž���"},{"����"},{"����"}}; 

/*RFID ģ����ر���*/
#if 0
//���ֵ�ASCII��
uc8 numberascii[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//��ʾ������
u8  dispnumber5buf[6];
u8  dispnumber3buf[4];
u8  dispnumber2buf[3];
//MFRC522������
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];
//SM05-S������
u8  sm05cmdbuf[15]={14,128,0,22,5,0,0,0,4,1,157,16,0,0,21};

#endif


/*��ʱ����*/
static uint32_t	i=0;
static char buf[128]={0};

/*��ع��ܺ���*/
void INIT_FUNC(void);
void WDG_RST_FLAG(void);
void Display_start(void);
void Display_menu(void);
void Display_func(void);
void usart_str_load(uint8_t *,uint32_t *,uint32_t *);


/*RFID����SPI��غ���*/
void flash_read_record(char *pbuf,uint32_t record_count);
void flash_erase_record(void);
uint32_t flash_write_record(char *pbuf,uint32_t record_count);

/*����Աģʽ�µ����*/
void add_sfm_admin(void);
void get_sfm_admin(void);
void del_sfm_admin(void);
int search_unlock_record_admin(int , int );

/*����ʶ��*/
int Gesture_test(void);

void display(void);

#endif

