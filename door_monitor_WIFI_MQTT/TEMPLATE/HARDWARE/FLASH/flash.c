#include "flash.h"


#if 0
void flash_unlock_func(void)
{
	uint32_t d;
	/* ����FLASH�����棩*/
	FLASH_Unlock();
	
	/* ��������4 */
	if(FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
	{
		printf("FLASH_EraseSector error\r\n");
		while(1);
	}
	
	/* ������4�׵�ַд��0x12345678 */
	if(FLASH_ProgramWord(0x8010000,0xAABBCCDD)!= FLASH_COMPLETE)
	{
		printf("FLASH_ProgramWord error\r\n");
		while(1);		
		
	}
	
	/* ������Ҫ�޸����ݣ����������� */
	FLASH_Lock(); 
	
	d = *(__IO uint32_t*)0x8010000;

	printf("read addr at 0x8010000 is 0x%08X\r\n",d);

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
#endif
