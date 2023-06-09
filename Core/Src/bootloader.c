#include "bootloader.h"

static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RX_LENGTH];


static void Bootloader_Get_Version(uint8_t *Host_Buffer);
static void Bootloader_Get_Help(uint8_t *Host_Buffer);
static void Bootloader_Get_Chip_Identification_Number(uint8_t *Host_Buffer);
static void Bootloader_Read_Protection_Level(uint8_t *Host_Buffer);
static void Bootloader_Jump_To_Address(uint8_t *Host_Buffer);
static void Bootloader_Erase_Flash(uint8_t *Host_Buffer);
static void Bootloader_Memory_Write(uint8_t *Host_Buffer);
static void Bootloader_Enable_RW_Protection(uint8_t *Host_Buffer);
static void Bootloader_Memory_Read(uint8_t *Host_Buffer);
static void Bootloader_Get_Sector_Protection_Status(uint8_t *Host_Buffer);
static void Bootloader_Read_OTP(uint8_t *Host_Buffer);
static void Bootloader_Change_Read_Protection_Level(uint8_t *Host_Buffer);


static tCRC_VERIFY Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC);
static void Bootloader_Send_ACK(uint8_t Replay_Len);
static void Bootloader_Send_NACK(void);
static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Len);
static void bootloader_jump_to_user_app(void);
static uint8_t Host_Address_Verification(uint32_t Jump_Address);

static uint8_t Bootloader_Supported_CMDs[12] =
{
    CBL_GET_VER_CMD,
    CBL_GET_HELP_CMD,
    CBL_GET_CID_CMD,
    CBL_GET_RDP_STATUS_CMD,
    CBL_GO_TO_ADDR_CMD,
    CBL_FLASH_ERASE_CMD,
    CBL_MEM_WRITE_CMD,
    CBL_ED_W_PROTECT_CMD,
    CBL_MEM_READ_CMD,
    CBL_READ_SECTOR_STATUS_CMD,
    CBL_OTP_READ_CMD,
    CBL_CHANGE_ROP_Level_CMD
};


void BL_Print_Message(char *format, ...)
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


BL_Status BL_UART_Fetch_Host_Command(void)
{
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("\rBootloader Fetch\r");
#endif

	BL_Status Status = BL_NACK;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t dataLength = 0;
	uint8_t _command = 0;

	/* Intialize the data Buffer by Zeros */
	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_LENGTH);

	/* Get length of Command Packet from the HOST */
	HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, &BL_Host_Buffer, 1, HAL_MAX_DELAY);

	if(HAL_Status != HAL_OK) Status = BL_NACK;
	else
	{
		dataLength = BL_Host_Buffer[0]-'0';

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("Command Length : %d \r\n", dataLength);
#endif

		/* Get Command packet from the HOST */
		HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, &(BL_Host_Buffer[1]), dataLength, HAL_MAX_DELAY);

		if(HAL_Status != HAL_OK) Status = BL_NACK;
		else
		{
			_command = BL_Host_Buffer[1] - '0';
			BL_Print_Message("Command ID : %d\r", _command);
			switch(_command)
			{
			case CBL_GET_VER_CMD:
				BL_Print_Message("CBL_GET_VER_CMD\r\n");
				Bootloader_Get_Version(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_GET_HELP_CMD:
				BL_Print_Message("CBL_GET_HELP_CMD\r\n");
				Bootloader_Get_Help(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_GET_CID_CMD:
				BL_Print_Message("CBL_GET_CID_CMD\r\n");
				Bootloader_Get_Chip_Identification_Number(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_GET_RDP_STATUS_CMD:
				BL_Print_Message("CBL_GET_RDP_STATUS_CMD\r\n");
				Bootloader_Read_Protection_Level(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_GO_TO_ADDR_CMD:
				BL_Print_Message("CBL_GO_TO_ADDR_CMD\r\n");
				Bootloader_Jump_To_Address(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_FLASH_ERASE_CMD:
				BL_Print_Message("CBL_FLASH_ERASE_CMD\r\n");
				Bootloader_Erase_Flash(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_MEM_WRITE_CMD:
				BL_Print_Message("CBL_MEM_WRITE_CMD\r\n");
				Bootloader_Memory_Write(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_ED_W_PROTECT_CMD:
				BL_Print_Message("CBL_ED_W_PROTECT_CMD\r\n");
				BL_Print_Message("Enable or Disable write protect on different sectors of the user flash \r\n");
				Bootloader_Enable_RW_Protection(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_MEM_READ_CMD:
				BL_Print_Message("CBL_MEM_READ_CMD\r\n");
				BL_Print_Message("Read data from different memories of the microcontroller \r\n");
				Bootloader_Memory_Read(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_READ_SECTOR_STATUS_CMD:
				BL_Print_Message("CBL_READ_SECTOR_STATUS_CMD\r\n");
				BL_Print_Message("Read all the sector protection status \r\n");
				Bootloader_Get_Sector_Protection_Status(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_OTP_READ_CMD:
				BL_Print_Message("CBL_OTP_READ_CMD\r\n");
				BL_Print_Message("Read the OTP contents \r\n");
				Bootloader_Read_OTP(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			case CBL_CHANGE_ROP_Level_CMD:
				BL_Print_Message("CBL_CHANGE_ROP_Level_CMD\r\n");
				Bootloader_Change_Read_Protection_Level(BL_Host_Buffer);
				Status = BL_ACK;
				break;

			default:
				BL_Print_Message("Invalid command code received from host !! \r\n");
				break;
			}
		}
	}
	return Status;
}



static void Bootloader_Get_Version(uint8_t *Host_Buffer)
{
	uint8_t BL_Version[4] = {CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION};

	/* Extract Packet length Sent by the HOST */
	uint16_t Host_CMD_Packet_Length = Host_Buffer[0]+1-'0';

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Packet Length : %d\r", Host_CMD_Packet_Length);
#endif

	/* Extract CRC32 Sent by the HOST */
	uint32_t Host_CRC32 = (uint32_t *)(Host_Buffer + Host_CMD_Packet_Length - CRC_TYPE_SIZE_BYTE);

	/* CRC32 Verification */
	if(Bootloader_CRC_Verify( (uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Length-CRC_TYPE_SIZE_BYTE, Host_CRC32) == CRC_PASS)
	{
		/* send ack */
		Bootloader_Send_ACK(4);
		Bootloader_Send_Data_To_Host((uint8_t *)BL_Version, 4);
	}
	else
	{
		/* send not ack */
		Bootloader_Send_NACK();
	}
}


static void Bootloader_Get_Help(uint8_t *Host_Buffer)
{
	/* Extract Packet length Sent by the HOST */
	uint16_t Host_CMD_Packet_Length = Host_Buffer[0]+1-'0';

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Packet Length : %d\r", Host_CMD_Packet_Length);
#endif

	/* Extract CRC32 Sent by the HOST */
	uint32_t Host_CRC32 = (uint32_t *)(Host_Buffer + Host_CMD_Packet_Length - CRC_TYPE_SIZE_BYTE);

	/* CRC32 Verification */
	if(Bootloader_CRC_Verify( (uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Length-CRC_TYPE_SIZE_BYTE, Host_CRC32) == CRC_PASS)
	{
		/* send ack */
		Bootloader_Send_ACK(12);
		Bootloader_Send_Data_To_Host((uint8_t *)Bootloader_Supported_CMDs, 12);
	}
	else
	{
		/* send not ack */
		Bootloader_Send_NACK();
	}
}


static void Bootloader_Get_Chip_Identification_Number(uint8_t *Host_Buffer)
{
	/* Extract Packet length Sent by the HOST */
	uint16_t Host_CMD_Packet_Length = Host_Buffer[0]+1-'0';

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Packet Length : %d\r", Host_CMD_Packet_Length);
#endif

	/* Extract CRC32 Sent by the HOST */
	uint32_t Host_CRC32 = (uint32_t *)(Host_Buffer + Host_CMD_Packet_Length - CRC_TYPE_SIZE_BYTE);

	/* CRC32 Verification */
	if(Bootloader_CRC_Verify( (uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Length-CRC_TYPE_SIZE_BYTE, Host_CRC32) == CRC_PASS)
	{
		/* Get the MCU Chip Identification Number */
		uint16_t Chip_Identification_Number= (uint16_t)((DBGMCU->IDCODE) & 0x00000FFF);

		/* Report the Chip Identification Number to Host */
		Bootloader_Send_ACK(2);
		Bootloader_Send_Data_To_Host((uint8_t *)Chip_Identification_Number, 2);
	}
	else
	{
		/* send not ack */
		Bootloader_Send_NACK();
	}
}


static void Bootloader_Read_Protection_Level(uint8_t *Host_Buffer)
{

}


static void Bootloader_Jump_To_Address(uint8_t *Host_Buffer)
{
	/* Extract Packet length Sent by the HOST */
	uint16_t Host_CMD_Packet_Length = Host_Buffer[0]+1-'0';

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Packet Length : %d\r", Host_CMD_Packet_Length);
#endif

	/* Extract CRC32 Sent by the HOST */
	uint32_t Host_CRC32 = (uint32_t *)(Host_Buffer + Host_CMD_Packet_Length - CRC_TYPE_SIZE_BYTE);

	/* CRC32 Verification */
	if(Bootloader_CRC_Verify( (uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Length-CRC_TYPE_SIZE_BYTE, Host_CRC32) == CRC_PASS)
	{
		/* send ack */
		Bootloader_Send_ACK(1);

		uint32_t HOST_Jump_Address = 0;
		uint8_t Address_Verification = ADDRESS_IS_INVALID;

		/* Extract Address form HOST Packet */
		HOST_Jump_Address = *((uint32_t *)&Host_Buffer[2]);

		/* Verify the Extracted address to be valid address */
		Address_Verification = Host_Address_Verification(HOST_Jump_Address);
		if(ADDRESS_IS_VALID == Address_Verification)
		{
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("Address verification succeeded \r\n");
#endif

		/* Report address verification succeeded */
		Bootloader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);

		/* Prepare the address to jump */
		Jump_Ptr Jump_Address = (Jump_Ptr)(HOST_Jump_Address + 1);

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("Jump to : 0x%X \r\n", Jump_Address);
#endif

		Jump_Address();
		}
	}
	else
	{
		/* send not ack */
		Bootloader_Send_NACK();
	}
}


static void Bootloader_Erase_Flash(uint8_t *Host_Buffer)
{

}


static void Bootloader_Memory_Write(uint8_t *Host_Buffer)
{

}


static void Bootloader_Enable_RW_Protection(uint8_t *Host_Buffer)
{

}


static void Bootloader_Memory_Read(uint8_t *Host_Buffer)
{

}


static void Bootloader_Get_Sector_Protection_Status(uint8_t *Host_Buffer)
{

}


static void Bootloader_Read_OTP(uint8_t *Host_Buffer)
{

}


static void Bootloader_Change_Read_Protection_Level(uint8_t *Host_Buffer)
{

}


static tCRC_VERIFY Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC)
{
	tCRC_VERIFY CRC_Status = CRC_FAIL;
	uint32_t Calculated_CRC=0, Data_Buffer=0;
	uint8_t lDataCounter=0;

	for(lDataCounter=0 ; lDataCounter<Data_Len ; lDataCounter++)
	{
		Data_Buffer = (uint32_t)pData[lDataCounter];
		Calculated_CRC = HAL_CRC_Accumulate(CRC_ENGINE, &Data_Buffer, 1);
	}

	/* Reset CRC calculation unit */
	  __HAL_CRC_DR_RESET(CRC_ENGINE);

	if(Calculated_CRC == Host_CRC) CRC_Status=CRC_PASS;
	return CRC_Status;
}


static void Bootloader_Send_ACK(uint8_t Replay_Len)
{
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Sending Acknowledgment.\r");
#endif
	uint8_t ACK_Value[2] = {0};
	ACK_Value[0] = CBL_SEND_ACK;
	ACK_Value[1] = Replay_Len;
	Bootloader_Send_Data_To_Host((uint8_t *)ACK_Value, 2);
}


static void Bootloader_Send_NACK(void)
{
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("Sending Not Acknowledgment.\r");
#endif
	uint8_t ACK_Value = CBL_SEND_NACK;
	Bootloader_Send_Data_To_Host(&ACK_Value, 1);
}


static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Len)
{
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, Host_Buffer, Data_Len, HAL_MAX_DELAY);
}


static void bootloader_jump_to_user_app(void)
{
	/* Value of the main stack pointer of our main application */
	uint32_t MSP_Value = *((volatile uint32_t *)FLASH_SECTOR2_BASE_ADDRESS);

	/* Reset Handler definition function of our main application */
	uint32_t MainAppAddr = *((volatile uint32_t *)(FLASH_SECTOR2_BASE_ADDRESS + 4));

	/* Fetch the reset handler address of the user application */
	pMainApp ResetHandler_Address = (pMainApp)MainAppAddr;

	/* Set Main Stack Pointer */
	__set_MSP(MSP_Value);

	/* DeInitialize / Disable of modules */
	/* DeInitialize the RCC clock configuration to the default reset state. */
	/* Disable Maskable Interrupt */
	HAL_RCC_DeInit();

	/* Jump to Application Reset Handler */
	ResetHandler_Address();
}


static uint8_t Host_Address_Verification(uint32_t Jump_Address)
{
	uint8_t Address_Verification = ADDRESS_IS_INVALID;

	if((Jump_Address >= SRAM_BASE) && (Jump_Address <= STM32F103_SRAM_END))				Address_Verification = ADDRESS_IS_VALID;
	else if((Jump_Address >= FLASH_BASE) && (Jump_Address <= STM32F103_FLASH_END))		Address_Verification = ADDRESS_IS_VALID;
	else		Address_Verification = ADDRESS_IS_INVALID;

	return Address_Verification;
}
