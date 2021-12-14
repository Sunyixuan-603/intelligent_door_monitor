#ifndef __ES8266_MQTT_H__
#define __ES8266_MQTT_H__

#include "stm32f4xx.h"

//�˴��ǰ����Ʒ������Ĺ���ʵ����½����-------------------------------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������


#define MQTT_BROKERADDRESS 		"iot-06z00h1t18fjx47.mqtt.iothub.aliyuncs.com"
#define MQTT_CLIENTID 			"0001|securemode=3,signmethod=hmacsha1|"
#define MQTT_USARNAME 			"Arctic_light&gc5l6wx0mp5"
#define MQTT_PASSWD 			"1A207114034CE2632C69D4B709C3094841D00BC4"
#define	MQTT_PUBLISH_TOPIC 		"/sys/gc5l6wx0mp5/Arctic_light/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC 	"/sys/gc5l6wx0mp5/Arctic_light/thing/service/property/set"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	

//MQTT���ӷ�����
extern int32_t mqtt_connect(char *client_id,char *user_name,char *password);

//MQTT��Ϣ����
extern int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);

//MQTT��Ϣ����
extern uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);

//MQTT����������
extern void mqtt_send_heart(void);

extern int32_t esp8266_mqtt_init(void);

extern void mqtt_report_devices_status(void);

#endif
