#include "I2C.h"


void I2C_INIT(void)
{
    //ʹ�ܶ˿�BӲ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //����I2C SCL��SDA�������ģʽ
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //��©ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;     //IO�ٶ�Ϊ�ɸߵ�ƽ�������͵�ƽ��Ӧ��ʱ��
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;       
    GPIO_Init(GPIOB,&GPIO_InitStructure);


    //ֻҪ�����״̬��������г�ʼ��ƽ
    //����I2CģʽĬ�Ͽ�ʼSCL��SDAΪ�ߵ�ƽ
    SCL_W = 1;
    SDA_W = 1;
}

void SDA_Pin_Mode(GPIOMode_TypeDef Pin_Mode)
{
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = Pin_Mode;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //��©ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;     //IO�ٶ�Ϊ�ɸߵ�ƽ�������͵�ƽ��Ӧ��ʱ��
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;       
    GPIO_Init(GPIOB,&GPIO_InitStructure);

}

void I2C_Start(void)
{
    //��֤SDA����Ϊ���ģʽ
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 1;
    SDA_W = 1;
    delay_xus(5);

    SDA_W = 0;
    delay_xus(5);

    SCL_W = 0;
    delay_xus(5);

}

void I2C_Stop(void)
{
    //��֤SDA����Ϊ���ģʽ
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 1;
    SDA_W = 0;
    delay_xus(5);

    SDA_W = 1;
    delay_xus(5);

}

void I2C_Send_Byte(uint8_t byte)
{
    int32_t i;
    //��֤SDA����Ϊ���ģʽ
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 0;
    SDA_W = 0;
    delay_xus(5);

    //8bit����
    for(i = 7;i >= 0;i--)
    {
        if(byte & (1<<i))
            SDA_W = 1;

        delay_xus(5);

        SCL_W = 1;
        delay_xus(5);

        SCL_W = 0;
        delay_xus(5);

    }

}

uint8_t I2C_Recv_Byte(void)
{
    uint8_t d = 0;
    int32_t i;

    //��֤SDA����Ϊ����ģʽ
    SDA_Pin_Mode(GPIO_Mode_IN);

    /*8bit��ȡ*/
    for(i = 7;i >= 0;i--)
    {
        SCL_W = 1;
        delay_xus(5);

        if(SDA_R)
            d |= 1<<i;
        
        //��������ռ������
        SCL_W = 0;
        delay_xus(5);

    }

    return d;
}

/*I2C�ȴ�ACK�ӻ���Ӧ*/
uint8_t I2C_Wait_ACK(void)
{

    uint8_t ack;
    //��֤SDA����Ϊ����ģʽ
    SDA_Pin_Mode(GPIO_Mode_IN);

    //�����ŵھŸ�ʱ������,��SCL����
    SCL_W = 1;
    delay_xus(5);

    if(SDA_R)   //��ȡSDA�Ƿ���Ӧ���źţ���Ӧ���ǵ͵�ƽ,��Ӧ����֮
        ack = 1;

    SCL_W = 0;  //��������ռ������
    delay_xus(5);

    return ack;

}

int32_t AT24C02_Write(uint8_t addr,uint8_t *buf,uint8_t len)
{

    uint8_t ack;
    uint8_t *p = buf;

    //������ʼ�ź�
    I2C_Start();

    //�����豸Ѱַ��д���ʵ�ַ0xA0
    I2C_Send_Byte(0xA0);
    
    //�ȴ�Ӧ����
    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("device address error\r\n");
        return -1;
    }

    //����Ҫ��EEPROM���ݵ���ʼ��ַ
    I2C_Send_Byte(addr);

    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("word address error\r\n");
        return -2;
    }
    //������������
    while(len--)
    {
        I2C_Send_Byte(*p++);

        ack = I2C_Wait_ACK();

        if(ack)
        {
            printf("write data error\r\n");
            return -3;
        }
    }

    I2C_Stop();
    printf("write data success\r\n");
    return 0;
}

void I2C_ACK(uint8_t ack)
{
    //��֤SDA����Ϊ���ģʽ
    SDA_Pin_Mode(GPIO_Mode_OUT);

    SCL_W = 0;
    SDA_W = 0;
    delay_xus(5);

    if(ack)
        SDA_W = 1;

    delay_xus(5);

    SCL_W = 1;
    delay_xus(5);

    SCL_W = 0;
    delay_xus(5);
}

int32_t AT24C02_Read(uint8_t addr,uint8_t *buf,uint8_t len)
{
    uint8_t ack;
    uint8_t *p = buf;

    //������ʼ�ź�
    I2C_Start();

    //�����豸Ѱַ��д���ʵ�ַΪ0xA0
    I2C_Send_Byte(0xA0);

    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("device address error\r\n");
        return -1;

    }

    //����Ҫ����EEPROM���ݵ���ʼ��ַ
    I2C_Send_Byte(addr);

    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("word address error\r\n");
        return -2;
    }

    //������ʼ�źţ������л����ʷ��򣬴�д�����л�������
    I2C_Start();

    //�����豸Ѱַ��д���ʵ�ַΪ0XA1,1010Ϊ�̶�����MSB�ĸ�4λ
    //1Ϊ����дΪʹ��R����Ч��ʹΪ�ߵ�ƽ
    I2C_Send_Byte(0xA1);
    ack = I2C_Wait_ACK();
    if(ack)
    {
        printf("device address error\r\n");
        return -3;
    }

    //������ȡ����
    len = len-1;
    
    while(len--)
    {
        //��ȡ����
        *p = I2C_Recv_Byte();

        //����Ӧ���ź�
        I2C_ACK(0);

        p++;
    }

    //��ȡ���һ���ֽ�
    *p = I2C_Recv_Byte();

    //������Ӧ���ź�
    I2C_ACK(1);

    I2C_Stop();

    return 0;
}

