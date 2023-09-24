#ifndef	__BOOTLOADER_H__
#define __BOOTLOADER_H__

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"
#include "crc.h"


#define 	BL_DEBUG_UART							&huart2
#define 	BL_HOST_COMMUNICATION_UART				&huart1
#define 	CRC_ENGINE             					&hcrc

#define 	DEBUG_INFO_DISABLE           			0
#define 	DEBUG_INFO_ENABLE            			1
#define 	BL_DEBUG_ENABLE              			DEBUG_INFO_DISABLE

#define 	BL_ENABLE_UART_DEBUG_MESSAGE			0x00
#define 	BL_ENABLE_SPI_DEBUG_MESSAGE				0x01
#define 	BL_ENABLE_I2C_DEBUG_MESSAGE				0x02
#define 	BL_ENABLE_CAN_DEBUG_MESSAGE				0x03

#define 	BL_DEBUG_METHOD							BL_ENABLE_UART_DEBUG_MESSAGE
#define 	BL_HOST_BUFFER_RX_LENGTH				200

#define 	CBL_GET_VER_CMD              			0x10
#define 	CBL_GET_HELP_CMD             			0x11
#define 	CBL_GET_CID_CMD              			0x12
#define 	CBL_GET_RDP_STATUS_CMD       			0x13
#define 	CBL_GO_TO_ADDR_CMD           			0x14
#define 	CBL_FLASH_ERASE_CMD          			0x15
#define 	CBL_MEM_WRITE_CMD            			0x16
#define 	CBL_ED_W_PROTECT_CMD         			0x17
#define 	CBL_MEM_READ_CMD             			0x18
#define 	CBL_READ_SECTOR_STATUS_CMD   			0x19
#define 	CBL_OTP_READ_CMD             			0x20
#define 	CBL_CHANGE_ROP_Level_CMD     			0x21

#define SEND_NACK        0xAB
#define SEND_ACK         0xCD


#define 	CBL_VENDOR_ID                			76
#define 	CBL_SW_MAJOR_VERSION         			4
#define 	CBL_SW_MINOR_VERSION         			4
#define 	CBL_SW_PATCH_VERSION         			1


#define 	CRC_TYPE_SIZE_BYTE           			4


typedef enum
{
	BL_NACK=0,
	BL_ACK
}BL_Status;


typedef enum
{
	CRC_FAIL = 0,
	CRC_PASS
}tCRC_VERIFY;

BL_Status BL_FeatchHostCommand();
void BL_Print_Message(char *format,...);

#endif //__BOOTLOADER_H__
