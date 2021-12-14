#include "PAJ7620.h"
#include <stdio.h>

#define INIT_SIZE sizeof(init_Array)/2
//�ϵ��ʼ������
static const unsigned char init_Array[][2] = {

    {0xEF,0x00},
	{0x37,0x07},
    {0x38,0x17},
	{0x39,0x06},
	{0x41,0x00},
	{0x42,0x00},
	{0x46,0x2D},
	{0x47,0x0F},
	{0x48,0x3C},
	{0x49,0x00},
	{0x4A,0x1E},
	{0x4C,0x20},
	{0x51,0x10},
	{0x5E,0x10},
	{0x60,0x27},
	{0x80,0x42},
	{0x81,0x44},
	{0x82,0x04},
	{0x8B,0x01},
	{0x90,0x06},
	{0x95,0x0A},
	{0x96,0x0C},
	{0x97,0x05},
	{0x9A,0x14},
	{0x9C,0x3F},
	{0xA5,0x19},
	{0xCC,0x19},
	{0xCD,0x0B},
	{0xCE,0x13},
	{0xCF,0x64},
	{0xD0,0x21},
	{0xEF,0x01},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x25,0x01},
	{0x27,0x39},
	{0x28,0x7F},
	{0x29,0x08},
	{0x3E,0xFF},
	{0x5E,0x3D},
	{0x65,0x96},
	{0x67,0x97},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x72,0x01},
	{0x73,0x35},
	{0x74,0x00},
	{0x77,0x01},
};

#define PROXIM_SIZE sizeof(proximity_arry)/2
//�ӽ�����ʼ������
static const unsigned char proximity_arry[][2]={

	{0xEF,0x00},
	{0x41,0x00},
	{0x42,0x00},
	{0x48,0x3C},
	{0x49,0x00},
	{0x51,0x13},
	{0x83,0x20},
	{0x84,0x20},
	{0x85,0x00},
	{0x86,0x10},
	{0x87,0x00},
	{0x88,0x05},
	{0x89,0x18},
	{0x8A,0x10},
	{0x9f,0xf8},
	{0x69,0x96},
	{0x6A,0x02},
	{0xEF,0x01},
	{0x01,0x1E},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x41,0x50},
	{0x43,0x34},
	{0x65,0xCE},
	{0x66,0x0B},
	{0x67,0xCE},
	{0x68,0x0B},
	{0x69,0xE9},
	{0x6A,0x05},
	{0x6B,0x50},
	{0x6C,0xC3},
	{0x6D,0x50},
	{0x6E,0xC3},
	{0x74,0x05},


};

#define GESTURE_SIZE sizeof(gesture_arry)/2
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

extern void delay_us(uint32_t nus);
extern void delay_ms(uint32_t nms);

//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}		
}

//PAJ7620U2����
uint8_t paj7620u2_wakeup(void)
{ 
	uint8_t data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
uint8_t paj7620u2_init(void)
{
	uint8_t i;
	uint8_t status;
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) 
		return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

#if 0
extern void led_single_ctrl(uint32_t led_pos,uint32_t sta);
extern void led_race_d2u(void);
extern void led_race_u2d(void);


//����ʶ�����
void Gesture_test(void)
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
	while(1)
	{
		//��ȡ����״̬			
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);
		
		if(!status)
		{   
			gesture_data =(uint16_t)data[1]<<8 | data[0];
			
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:               
					                           printf("Up\r\n");
											   if(i < 5) i++;
											   led_single_ctrl(i,0);
						 break; //����
					case GES_DOWM:                  
               						           printf("Dowm\r\n"); 
											   if(i > 1) i--;
											   led_single_ctrl(i,0);					
						 break; //����
					case GES_LEFT:                       
  						                       printf("Left\r\n");                break; //����
					case GES_RIGHT:               
                						       printf("Right\r\n");               break; //����
					case GES_FORWARD:                 
						                       printf("Forward\r\n");             break; //��ǰ
					case GES_BACKWARD:            
            						           printf("Backward\r\n");            break; //���
					case GES_CLOCKWISE:             
                						       printf("Clockwise\r\n");     led_race_u2d();    break; //˳ʱ��
					case GES_COUNT_CLOCKWISE:   
                   						       printf("AntiClockwise\r\n"); led_race_d2u();    break; //��ʱ��
					case GES_WAVE:                 
						                       printf("Wave\r\n");                break; //�Ӷ�
					default:   break;
					
				}		
			}
		}

		delay_ms(50);
	}
}
#endif 



//�ӽ�������
void Ps_test(void)
{
	uint8_t i;

	uint8_t data[2]={0x00};
	uint8_t obj_brightness=0;
	uint16_t obj_size=0;
	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//�ӽ����ģʽ��ʼ��
	}
	
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	i=0;

	while(1)
	{	

		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//��ȡ��������
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//��ȡ�����С
		data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
		obj_size = ((uint16_t)data[1] & 0x0f)<<8 | data[0];
		printf("obj_brightness: %d\r\n",obj_brightness);
        printf("obj_size: %d\r\n",obj_size);
		
		delay_ms(100);
		
		i++;
	}	
}

