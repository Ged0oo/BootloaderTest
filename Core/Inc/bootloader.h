#ifndef	__BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "usart.h"

#define BL_DEBUG_UART			&huart3
#define BL_COMMAND_UART			&huart2

void BL_Print_Message(char *format, ...);


#endif
