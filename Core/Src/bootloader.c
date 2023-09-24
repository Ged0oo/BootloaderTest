#include "Bootloader.h"

static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RX_LENGTH];


static void Bootloader_Get_Version(uint8_t *Host_Buffer);
static void BL_Get_Help(uint8_t *Host_buffer);
static void BL_Get_Chip_Idendification_nNumber(uint8_t *Host_buffer);
static void BL_Flash_Erase(uint8_t *Host_buffer);
static void BL_Write_Data(uint8_t *Host_buffer);

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
				case CBL_GET_VER_CMD:
					Bootloader_Get_Version(BL_Host_Buffer);
					status = BL_ACK;

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
    uint8_t BL_Version[4] = { CBL_VENDOR_ID,
	 						  CBL_SW_MAJOR_VERSION,
	 						  CBL_SW_MINOR_VERSION,
							  CBL_SW_PATCH_VERSION};

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
		HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t*)BL_Version,4,HAL_MAX_DELAY);
	}
	else
	{
		Bootloader_Send_NACK();
	}
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
