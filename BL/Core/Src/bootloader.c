#include "Bootloader.h"

static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RX_LENGTH];

static uint8_t Bootloader_Supported_CMDs[12] =
{
    CBL_GET_VER_CMD,
    CBL_GET_HELP_CMD,
    CBL_GET_CID_CMD,
    CBL_FLASH_ERASE_CMD,
    CBL_MEM_WRITE_CMD,
};

static void Bootloader_Get_Version(uint8_t *Host_Buffer);
static void Bootloader_Get_Help(uint8_t *Host_Buffer);
static void Bootloader_Get_Chip_Idendification_Number(uint8_t *Host_Buffer);
static void Bootloader_Flash_Erase(uint8_t *Host_Buffer);
static void Bootloader_Write_Data(uint8_t *Host_Buffer);
static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Len);
static void bootloader_jump_to_user_app(uint8_t *Host_Buffer);

static tCRC_VERIFY Bootloader_CRC_Verify(uint8_t * pdata,uint32_t DataLen,uint32_t HosrCRC);
static uint8_t Perform_Flash_Erase(uint32_t PageAddress, uint8_t page_Number);
static uint8_t BL_Address_Varification(uint32_t Addresss);
static uint8_t FlashMemory_Paylaod_Write(uint16_t * pdata,uint32_t StartAddress,uint8_t Payloadlen);
static void Bootloader_Send_ACK(uint8_t dataLen);
static void Bootloader_Send_NACK();


void BL_Print_Message(char *format,...)
{
	char message[100] = {0};

	va_list args;
	va_start(args, format);
	vsprintf(message , format , args);

#if (BL_DEBUG_METHOD==BL_ENABLE_UART_DEBUG_MESSAGE)
	HAL_UART_Transmit(BL_DEBUG_UART , (uint8_t *)message , sizeof(message), HAL_MAX_DELAY);

#elif (BL_DEBUG_METHOD==BL_ENABLE_SPI_DEBUG_MESSAGE)
			/*SEND DATA THROUGH SPI*/

#elif (BL_DEBUG_METHOD==BL_ENABLE_I2C_DEBUG_MESSAGE)
			/*SEND DATA THROUGH I2C*/

#elif (BL_DEBUG_METHOD==BL_ENABLE_CAN_DEBUG_MESSAGE)
			/*SEND DATA THROUGH CAN*/

#endif

	va_end(args);
}


BL_Status BL_FeatchHostCommand()
{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("\rBootloader Fetch\r");
#endif

	BL_Status status = BL_NACK;
	HAL_StatusTypeDef Hal_status = HAL_ERROR;

	uint8_t dataLength = 0;
	uint8_t _command = 0;

	/* Intialize the data Buffer by Zeros */
	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_LENGTH);

	/* Get length of Command Packet from the HOST */
	Hal_status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, BL_Host_Buffer, 1, HAL_MAX_DELAY);

	if(Hal_status != HAL_OK)
	{
		status = BL_NACK;
	}
	else
	{
		dataLength = BL_Host_Buffer[0];

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("Command Length : %d \r\n", dataLength);
#endif

		/* Get Command packet from the HOST */
		Hal_status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, &BL_Host_Buffer[1], dataLength, HAL_MAX_DELAY);

		if(Hal_status != HAL_OK)
		{
			status = BL_NACK;
		}
		else
		{
			_command = BL_Host_Buffer[1];
			switch(_command)
			{
				case CBL_GET_VER_CMD :
					Bootloader_Get_Version(BL_Host_Buffer);
					status = BL_ACK;
					break;

				case CBL_GET_HELP_CMD :
					Bootloader_Get_Help(BL_Host_Buffer);
					status = BL_ACK;
					break;

				case CBL_GET_CID_CMD :
					Bootloader_Get_Chip_Idendification_Number(BL_Host_Buffer);
					status = BL_ACK;
					break;

				case CBL_FLASH_ERASE_CMD :
					Bootloader_Flash_Erase(BL_Host_Buffer);
					status = BL_ACK;
					break;

				case CBL_MEM_WRITE_CMD :
					Bootloader_Write_Data(BL_Host_Buffer);
					status = BL_ACK;
					break;

				case CBL_JMP_USER_APP_CMD :
					bootloader_jump_to_user_app(BL_Host_Buffer);
					status = BL_ACK;
					break;

				default:
					BL_Print_Message("Invalid command code received from host !! \r\n");
					status = BL_NACK;
					break;
			}
		}
	}
	return status;
}


static void Bootloader_Get_Version(uint8_t *Host_buffer)
{
    uint8_t BL_Version[4] =
    {
		CBL_VENDOR_ID,
		CBL_SW_MAJOR_VERSION,
		CBL_SW_MINOR_VERSION,
		CBL_SW_PATCH_VERSION
    };

	/* Extract Packet length Sent by the HOST */
	uint16_t Host_CMD_Packet_Length = Host_buffer[0]+1;

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Packet Length : %d\r", Host_CMD_Packet_Length);
#endif

	/* Extract CRC32 Sent by the HOST */
	uint32_t Host_CRC32 = *(uint32_t *)(Host_buffer + Host_CMD_Packet_Length - CRC_TYPE_SIZE_BYTE);


	if(CRC_PASS == Bootloader_CRC_Verify((uint8_t*)&Host_buffer[0] , Host_CMD_Packet_Length-4, Host_CRC32))
	{
		Bootloader_Send_ACK(4);
		Bootloader_Send_Data_To_Host((uint8_t *)&BL_Version, 4);
	}
	else
	{
		Bootloader_Send_NACK();
	}
}


static void Bootloader_Get_Chip_Idendification_Number(uint8_t *Host_Buffer)
{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Read the MCU chip identification number \r\n");
#endif

	/* Extract the CRC32 and packet length sent by the HOST */
	uint16_t Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	uint32_t Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC Verification */
	if(CRC_PASS == Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0] , Host_CMD_Packet_Len - 4, Host_CRC32))
	{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif

		/* Get the MCU chip identification number */
		uint16_t MCU_Identification_Number = (uint16_t)((DBGMCU->IDCODE) & 0x00000FFF);

		/* Report chip identification number to HOST */
		Bootloader_Send_ACK(2);
		Bootloader_Send_Data_To_Host((uint8_t *)&MCU_Identification_Number, 2);
	}
	else
	{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif

		Bootloader_Send_NACK();
	}
}


static void Bootloader_Get_Help(uint8_t *Host_Buffer)
{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Read Commands Supported by Bootloader \r\n");
#endif

	/* Extract the CRC32 and packet length sent by the HOST */
	uint16_t Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	uint32_t Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC Verification */
	if(CRC_PASS == Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0] , Host_CMD_Packet_Len - 4, Host_CRC32))
	{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif

		Bootloader_Send_ACK(12);
		Bootloader_Send_Data_To_Host((uint8_t *)(&Bootloader_Supported_CMDs[0]), 12);

	}
	else
	{

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif

		Bootloader_Send_NACK();
	}
}


static void Bootloader_Flash_Erase(uint8_t *Host_Buffer)
{
	uint8_t Erase_status = UNSUCCESSFUL_ERASE;
	uint16_t Host_Packet_Len=0;
	uint32_t CRC_valu=0;
	Host_Packet_Len =  Host_Buffer[0]+1;
	CRC_valu = *(uint32_t*)(Host_Buffer+Host_Packet_Len -4);
	if(CRC_PASS == Bootloader_CRC_Verify((uint8_t*)&Host_Buffer[0],Host_Packet_Len-4,CRC_valu))
	{
		Erase_status = Perform_Flash_Erase(*((uint32_t*)&Host_Buffer[2]),Host_Buffer[6]);
		Bootloader_Send_ACK(1);
		HAL_UART_Transmit(&huart1,(uint8_t*)&Erase_status,1,HAL_MAX_DELAY);
	}
	else
	{
		Bootloader_Send_NACK();
	}
}


static uint8_t Perform_Flash_Erase(uint32_t PageAddress, uint8_t page_Number)
{
	FLASH_EraseInitTypeDef pEraseInit;
	HAL_StatusTypeDef Hal_status = HAL_ERROR;
	uint32_t PageError = 0;
	uint8_t PageStatus = INVALID_PAGE_NUMBER;

	if(page_Number>CBL_FLASH_MAX_PAGE_NUMBER)
	{
		PageStatus=INVALID_PAGE_NUMBER;
	}
	else
	{
		PageStatus = VALID_PAGE_NUMBER;

		if( (page_Number <= (CBL_FLASH_MAX_PAGE_NUMBER - 1)) || (PageAddress == CBL_FLASH_MASS_ERASE))
		{
			if(PageAddress == CBL_FLASH_MASS_ERASE )
			{
				pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
				pEraseInit.Banks = FLASH_BANK_1;
				pEraseInit.PageAddress = 0x8008000;
				pEraseInit.NbPages = 12;
			}
			else
			{
				pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
				pEraseInit.Banks = FLASH_BANK_1;
				pEraseInit.PageAddress = PageAddress;
				pEraseInit.NbPages = page_Number;
			}

			HAL_FLASH_Unlock();
			Hal_status = HAL_FLASHEx_Erase(&pEraseInit,&PageError);
			HAL_FLASH_Lock();

			if(PageError == HAL_SUCCESSFUL_ERASE)
			{
				PageStatus = SUCCESSFUL_ERASE;
			}
			else
			{
				PageStatus = UNSUCCESSFUL_ERASE;
			}
		}
		else
		{
			PageStatus = INVALID_PAGE_NUMBER;
		}
	}
return PageStatus;
}


static void Bootloader_Write_Data(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint32_t HOST_Address = 0;
	uint8_t Payload_Len = 0;
	uint8_t Address_Verification = ADDRESS_IS_INVALID;
	uint8_t Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;

	/* Extract the CRC32 and packet length sent by the HOST */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC Verification */
	if(CRC_PASS == Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0] , Host_CMD_Packet_Len - 4, Host_CRC32))
	{
		/* Send acknowledgement to the HOST */
		Bootloader_Send_ACK(1);

		/* Extract the start address from the Host packet */
		HOST_Address = *((uint32_t *)(&Host_Buffer[2]));

		/* Extract the payload length from the Host packet */
		Payload_Len = Host_Buffer[6];

		/* Verify the Extracted address to be valid address */
		Address_Verification = BL_Address_Varification(HOST_Address);

		if(ADDRESS_IS_VALID == Address_Verification)
		{
			/* Write the payload to the Flash memory */
			Flash_Payload_Write_Status = FlashMemory_Paylaod_Write((uint8_t *)&Host_Buffer[7], HOST_Address, Payload_Len);

			/* Report payload writing state */
			Bootloader_Send_Data_To_Host((uint8_t *)&Flash_Payload_Write_Status, 1);
		}
		else
		{
			/* Report address verification failed */
			Address_Verification = ADDRESS_IS_INVALID;
			Bootloader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);
		}
	}
	else
	{
		/* Send Not acknowledge to the HOST */
		Bootloader_Send_NACK();
	}
}


static uint8_t FlashMemory_Paylaod_Write(uint16_t * pdata,uint32_t StartAddress,uint8_t Payloadlen)
{
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	uint16_t Payload_Counter = 0;

	uint32_t Address=0;
	uint8_t UpdataAdress=0;

	/* Unlock the FLASH control register access */
	HAL_Status = HAL_FLASH_Unlock();

	if(HAL_Status != HAL_OK)
	{
		Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	}
	else
	{
		for(Payload_Counter=0 , UpdataAdress=0 ; Payload_Counter < Payloadlen/2 ; Payload_Counter++ , UpdataAdress+=2)
		{
			Address = StartAddress + UpdataAdress;

			/* Program a byte at a specified address */
			HAL_Status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address , pdata[Payload_Counter]);

			if(HAL_Status != HAL_OK)
			{
				Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
				break;
			}
			else
			{
				Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_PASSED;
			}
		}
	}

	if((FLASH_PAYLOAD_WRITE_PASSED == Flash_Payload_Write_Status) && (HAL_OK == HAL_Status))
	{
		/* Locks the FLASH control register access */
		HAL_Status = HAL_FLASH_Lock();

		if(HAL_Status != HAL_OK)
		{
			Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
		}
		else
		{
			Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_PASSED;
		}
	}
	else
	{
		Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	}
	return Flash_Payload_Write_Status;
}


static void bootloader_jump_to_user_app(uint8_t *Host_buffer)
{
		/* Extract Packet length Sent by the HOST */
		uint16_t Host_CMD_Packet_Length = Host_buffer[0]+1;

	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("Packet Length : %d\r", Host_CMD_Packet_Length);
	#endif

		/* Extract CRC32 Sent by the HOST */
		uint32_t Host_CRC32 = *(uint32_t *)(Host_buffer + Host_CMD_Packet_Length - CRC_TYPE_SIZE_BYTE);


		if(CRC_PASS == Bootloader_CRC_Verify((uint8_t*)&Host_buffer[0] , Host_CMD_Packet_Length-4, Host_CRC32))
		{
			Bootloader_Send_ACK(1);
			Bootloader_Send_Data_To_Host(FLASH_SECTOR2_BASE_ADDRESS, 4);

			//just a function pointer to hold the address of the reset handler of the user app.
			void (*app_reset_handler)(void);

			//disbale interuppts
		    __set_PRIMASK(1);
		    __disable_irq();

		    SCB->VTOR = FLASH_SECTOR2_BASE_ADDRESS;

		    // 1. configure the MSP by reading the value from the base address of the sector 2
		    uint32_t msp_value = *(__IO uint32_t *)FLASH_SECTOR2_BASE_ADDRESS;

		    __set_MSP(msp_value);

		    uint32_t resethandler_address = *(__IO uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS + 4);

		    app_reset_handler = (void*) resethandler_address;

		    //3. jump to reset handler of the user application
		    app_reset_handler();
		}
		else
		{
			Bootloader_Send_NACK();
		}
}


static uint8_t BL_Address_Varification(uint32_t Addresss)
{
	uint8_t Adress_varfiy=ADDRESS_IS_INVALID;
	if(Addresss>=FLASH_BASE &&Addresss<=STM32F103_FLASH_END)
	{
		Adress_varfiy=ADDRESS_IS_VALID;
	}
	else if(Addresss>=SRAM_BASE &&Addresss<=STM32F103_SRAM_END)
	{
		Adress_varfiy=ADDRESS_IS_VALID;
	}
	else{
		Adress_varfiy=ADDRESS_IS_INVALID;
	}
	return Adress_varfiy;
}


static tCRC_VERIFY Bootloader_CRC_Verify(uint8_t * pdata,uint32_t DataLen,uint32_t HostCRC)
{
	tCRC_VERIFY CRC_Status = CRC_FAIL;
	uint32_t Calculated_CRC = 0, Data_Buffer = 0;
	uint8_t counter = 0;

	for(counter=0 ; counter<DataLen ; counter++)
	{
		Data_Buffer = (uint32_t)pdata[counter];
		Calculated_CRC = HAL_CRC_Accumulate(CRC_ENGINE, &Data_Buffer, 1);
	}

	/* Reset CRC calculation unit */
	  __HAL_CRC_DR_RESET(CRC_ENGINE);

	if(Calculated_CRC == HostCRC)
	{
		CRC_Status = CRC_PASS;
	}

	return CRC_Status;
}


static void Bootloader_Send_ACK(uint8_t dataLen)
{
	uint8_t ACK_value[2] = {0};
	ACK_value[0] = SEND_ACK;
	ACK_value[1] = dataLen;
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t*)ACK_value, 2, HAL_MAX_DELAY);
}


static void Bootloader_Send_NACK()
{
	uint8_t ACk_value=SEND_NACK;
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, &ACk_value, sizeof(ACk_value), HAL_MAX_DELAY);
}


static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Len)
{
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, Host_Buffer, Data_Len, HAL_MAX_DELAY);
}
