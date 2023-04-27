#include "bootloader.h"

static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RX_LENGTH];



void BL_Print_Message(char *format, ...)
{
	char message[100] = {0};

	va_list args;
	va_start(args , format);
	vsprintf(message , format , args);

#if (BL_DEBUG_METHOD==BL_ENABLE_UART_DEBUG_MESSAGE)
	HAL_UART_Transmit(BL_DEBUG_UART , (uint8_t *)message , sizeof(message) , HAL_MAX_DELAY);
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
	BL_Status Status = BL_NACK;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t dataLength = 0;
	uint8_t _command = 0;

	/* Intialize the data Buffer by Zeros */
	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_LENGTH);

	/* Get length of Command Packet from the HOST */
	HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, BL_Host_Buffer, 1, HAL_MAX_DELAY);
	if(HAL_Status != HAL_OK)
	{
		Status = BL_NACK;
	}
	else
	{
		dataLength = BL_Host_Buffer[0]-'0';
		BL_Print_Message("Command Length : %d", dataLength);
		/* Get Command packet from the HOST */
		HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, &(BL_Host_Buffer[1]), dataLength, HAL_MAX_DELAY);
		if(HAL_Status != HAL_OK)
		{
			Status = BL_NACK;
		}
		else
		{
			_command = BL_Host_Buffer[1] - '0';
			BL_Print_Message("Command ID : %d", _command);
			switch(_command)
			{
				case CBL_GET_VER_CMD :
				BL_Print_Message("CBL_GET_VER_CMD Recieved\r\n");
				break;

				case CBL_GET_HELP_CMD :
				BL_Print_Message("CBL_GET_HELP_CMD Recieved\r\n");
				break;

				case CBL_GET_CID_CMD :
				BL_Print_Message("CBL_GET_CID_CMD Recieved\r\n");
				break;

				default :
				BL_Print_Message("Invalid Code Command Recieved From The Host! \r\n");
				break;
			}
		}
	}
	return Status;
}
