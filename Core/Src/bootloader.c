#include "bootloader.h"


void BL_Print_Message(char *format, ...)
{
	char message[100] = {0};

	va_list args;
	va_start(args , format);

	vsprintf(message , format , args);
	HAL_UART_Transmit(BL_DEBUG_UART , (uint8_t *)message , sizeof(message) , HAL_MAX_DELAY);

	va_end(args);

}
