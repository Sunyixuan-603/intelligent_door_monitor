#include "SPI.h"

void W25Q128_INIT(void)
{
	//ʹ�ܶ˿�BӲ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//ʹ��SPI1Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	//����PB3~PB5Ϊ���ù���ģʽ
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4|GPIO_Pin_5; 	//3 4 5������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	
	//��PB3~PB5���ӵ�SPI1Ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);


	//����PB14Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 ; 	//14������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOB,&GPIO_InitStructure);		
	
	//ֻҪ�����ģʽ��Ҫ��������ʼ��ƽ״̬
	PBout(14)=1;
	
	//��ʼ��SPI1��ز���
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��ͨ��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//������ɫ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//���ӻ��������ֲᣬͨ������ÿ�δ�СΪ8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						//���ӻ��������ֲᣬCPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					//���ӻ��������ֲᣬCPHA=1 ��ĿǰΪģʽ3
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//���ӻ��������ֲᣬƬѡ����������������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//���ӻ��������ֲᣬSPI��ʱ��=84MHz/16=5.25MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//���ӻ��������ֲ�
	//SPI_InitStructure.SPI_CRCPolynomial = 7;						//�������������M4����ͨ�ţ�spi flash��֧��
	SPI_Init(SPI1, &SPI_InitStructure);
 	
	
	//ʹ��SPI1Ӳ������
	SPI_Cmd(SPI1,ENABLE);
}


uint8_t SPI1_SendByte(uint8_t byte)
{
	/*!< Loop while DR register in not full,??????????????? */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	/*!< Send byte through the SPI1 peripheral?????SPI1?????????? */
	SPI_I2S_SendData(SPI1,byte);
	
	/*!< Wait to receive a byte?????????????????????SPI?????? */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
	
	/*!< Return the byte read from the SPI bus ???????????????*/
	return SPI_I2S_ReceiveData(SPI1);
	
}

void W25Q128_read_id(uint8_t *m_id,uint8_t *d_id)
{
	//�ӻ�������Ч���ӻ���ʼ������
	W25Q128_CS=0;

	//����0x90ָ�����SPI FLASHҪ�ṩID��Ϣ
	SPI1_SendByte(0x90);
	
	//24bit-addressȫΪ0
	SPI1_SendByte(0x00);	
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	
	/* ����������������� */
	*m_id=SPI1_SendByte(0x8);

	/* ����������������� */
	*d_id=SPI1_SendByte(0x4);

	//ͨѶ��ϣ��ӻ�������Ч���ӻ�ֹͣ������
	W25Q128_CS=1;	
}

/*??????*/
void W25Q128_read_data(uint32_t addr,uint8_t *buf,uint32_t len)
{
	uint8_t *p=buf;
	
	//�ӻ�������Ч���ӻ���ʼ������
	W25Q128_CS=0;	

	//����0x03ָ�����SPI FLASHҪ�ṩ�ڲ��洢����
	SPI1_SendByte(0x03);
	
	//addr = 0x123456
	SPI1_SendByte((addr>>16)&0xFF);//addr>>16 0x123456>>16 &0xFF = 0x12
	SPI1_SendByte((addr>>8) &0xFF);//addr>>8  0x123456>>8  &0xFF = 0x34
	SPI1_SendByte( addr     &0xFF);//addr>>0  0x123456>>0  &0xFF = 0x56


	while(len--)
	/* ����������������� */
		*p++=SPI1_SendByte(0xFF);

	//ͨѶ��ϣ��ӻ�������Ч���ӻ�ֹͣ������
	W25Q128_CS=1;	
}

/*���д����*/
void W25Q128_write_enable(void)
{
	//�ӻ�������Ч���ӻ���ʼ������
	W25Q128_CS=0;	

	//����0x06ָ�����SPI FLASHҪ���д����
	SPI1_SendByte(0x06);
	
	
	//ͨѶ��ϣ��ӻ�������Ч���ӻ�ֹͣ������
	W25Q128_CS=1;
}

/*��д����*/
void W25Q128_write_disable(void)
{
	//�ӻ�������Ч���ӻ���ʼ������
	W25Q128_CS=0;	

	//����0x04ָ�����SPI FLASHҪ����д����
	SPI1_SendByte(0x04);
	
	//ͨѶ��ϣ��ӻ�������Ч���ӻ�ֹͣ������
	W25Q128_CS=1;

}

uint8_t W25Q128_read_status_1(void)
{
	uint8_t status = 0;
	
	//�ӻ�������Ч
	W25Q128_CS = 0;
	
	//����0x05ָ�����SPI_FLASHҪ�ṩ״̬�Ĵ���1��Ϣ
	SPI1_SendByte(0x05);
	
	status = SPI1_SendByte(0xFF);
	
	//ͨѶ��ϣ��ӻ�������Ч���ӻ�ֹͣ������
	W25Q128_CS = 1;
	
	return status;

}

/*����*/
void W25Q128_erase_sector(uint32_t addr)
{
	uint8_t status = 0;
	
	//ִ��дʹ��ָ��
	W25Q128_write_enable();
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��w25qxxʶ��
	delay_xus(10);
	
	//Ƭѡ��������
	W25Q128_CS = 0;
	
	//����0x20
	SPI1_SendByte(0x20);
	
	//����24bit�ĵ�ַ
	SPI1_SendByte((addr >> 16) & 0xFF);
	SPI1_SendByte((addr >> 8) & 0xFF);
	SPI1_SendByte( addr & 0xFF);
	
	//Ƭѡ��������
	W25Q128_CS = 1;
	
	//��ʱһ���,��Ƭѡ�����ܹ����ָߵ�ƽһ��ʱ�䣬��W25Q128ʶ��
	delay_xus(10);
	
	//��鵱ǰ�Ƿ��Ѿ������ɹ�
	while(1)
	{
		status = W25Q128_read_status_1();
		
		if((status & 0x01) == 0)
			break;
		
		//ÿ��1ms���в�ѯ
		delay_xms(1);
	}
	
	//����д����
	W25Q128_write_disable();

}

void W25Q128_write_page(uint32_t addr,uint8_t *pbuf,uint32_t len)
{
	uint8_t status = 0;
	
	//ִ��дʹ��ָ��
	W25Q128_write_enable();
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��W25QXXʶ��
	delay_xus(10);
	
	//Ƭѡ��������
	W25Q128_CS = 0;

	//����0x02
	SPI1_SendByte(0x02);

	//����24bit�ĵ�ַ
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);
	
	//��������
	while(len--)
		SPI1_SendByte(*pbuf++);

	//Ƭѡ��������	
	W25Q128_CS=1;
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��w25qxxʶ��
	delay_xus(10);
	
	
	//��鵱ǰ�Ƿ��Ѿ�д��ɹ�
	while(1)
	{
		status=	W25Q128_read_status_1();
		
		if((status & 0x01) ==0)
			break;
	
		//ÿ��1������в�ѯ
		delay_xms(1);
	
	}
	
	//����д����
	W25Q128_write_disable();
}

void W25Q128_INIT_MODE(uint8_t ch)
{
	//ʹ�ܶ˿�B��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	//���ö˿�B PB3 PB5 PB14λ���ģʽ
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//���ö˿�B PB4 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//��ʱ��ͼ��ֻҪ�����ģʽ,��ʼ״̬Ҫ��Ӧ��Ӧ��ƽ
	W25Q128_CS = 1;		//SS����
	if(ch == 0)
	W25Q128_SCLK = 0; 	//SCLK����
	else if(ch == 3)
	W25Q128_SCLK = 1;

	
	W25Q128_MOSI = 1;	//MOSI����

}


uint8_t SPI1_SendByte_MODE0(uint8_t byte)
{
	int32_t i = 0;
	uint8_t d = 0;

	for(i = 7;i >= 0;i--)
	{
		//MSB,�����Чλ��������
		if(byte & (1<<i))
			W25Q128_MOSI = 1;
		else	
			W25Q128_MOSI = 0;		

		//����SCLK����Ϊ�ߵ�ƽ,���ߴӻ����ҿ�ʼ�ɼ�MISO���ŵ�ƽ
		W25Q128_SCLK = 1;
		delay_xus(2);

		//�����ɼ�MISO���ŵ�ƽ
		if(W25Q128_MISO)
			d |= 1<<i;

		//����SCLK����Ϊ�͵�ƽ����ʱ���ӻ���ʼ��ȡMOSI��ƽ
		W25Q128_SCLK = 0;
		delay_xus(2);
	}

	return d;
}

uint8_t SPI1_SendByte_MODE3(uint8_t byte)
{
	int32_t i = 0;
	uint8_t d = 0;

	for(i = 7;i >= 0;i--)
	{
		//MSB,�����Чλ��������
		if(byte & (1<<i))
			W25Q128_MOSI = 1;
		else
			W25Q128_MOSI = 0;	
		
		//����SCLK����Ϊ�͵�ƽ
		W25Q128_SCLK = 0;
		delay_xus(2);

		//�����ɼ�MISO���ŵ�ƽ
		if(W25Q128_MISO)
			d|=1<<i;

	}

	return d;
	
}
