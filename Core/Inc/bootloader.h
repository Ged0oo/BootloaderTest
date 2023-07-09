#ifndef	__BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "usart.h"
#include "crc.h"

#define BL_DEBUG_UART							&huart1
#define BL_HOST_COMMUNICATION_UART				&huart2
#define CRC_ENGINE             					&hcrc

#define BL_ENABLE_UART_DEBUG_MESSAGE			0x00
#define BL_ENABLE_SPI_DEBUG_MESSAGE				0x01
#define BL_ENABLE_I2C_DEBUG_MESSAGE				0x02
#define BL_ENABLE_CAN_DEBUG_MESSAGE				0x03

#define BL_DEBUG_METHOD							BL_ENABLE_UART_DEBUG_MESSAGE
#define BL_HOST_BUFFER_RX_LENGTH				200

#define CBL_GET_VER_CMD              			0
#define CBL_GET_HELP_CMD             			1
#define CBL_GET_CID_CMD              			2
/* Get Read Protection Status */
#define CBL_GET_RDP_STATUS_CMD       			3
#define CBL_GO_TO_ADDR_CMD           			4
#define CBL_FLASH_ERASE_CMD          			5
#define CBL_MEM_WRITE_CMD            			6
/* Enable/Disable Write Protection */
#define CBL_ED_W_PROTECT_CMD         			7
#define CBL_MEM_READ_CMD             			8
/* Get Sector Read/Write Protection Status */
#define CBL_READ_SECTOR_STATUS_CMD   			9
#define CBL_OTP_READ_CMD             			10
/* Change Read Out Protection Level */
#define CBL_CHANGE_ROP_Level_CMD     			11

#define CBL_VENDOR_ID                			100
#define CBL_SW_MAJOR_VERSION         			1
#define CBL_SW_MINOR_VERSION         			1
#define CBL_SW_PATCH_VERSION         			0

#define CRC_TYPE_SIZE_BYTE           			4

#define CBL_SEND_NACK                			0xAB
#define CBL_SEND_ACK                 			0xCD


typedef enum
{
	BL_NACK = 0,
	BL_ACK
}BL_Status;



typedef enum
{
	CRC_FAIL = 0,
	CRC_PASS
}tCRC_VERIFY;



void BL_Print_Message(char *format, ...);
BL_Status BL_UART_Fetch_Host_Command(void);

#endif
