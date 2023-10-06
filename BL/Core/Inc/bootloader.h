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
#define 	CBL_FLASH_ERASE_CMD          			0x13
#define 	CBL_MEM_WRITE_CMD            			0x14
#define 	CBL_JMP_USER_APP_CMD            		0x15


#define 	INVALID_PAGE_NUMBER          			0x00
#define 	VALID_PAGE_NUMBER            			0x01
#define 	UNSUCCESSFUL_ERASE           			0x02
#define 	SUCCESSFUL_ERASE             			0x03

#define 	CBL_FLASH_MAX_PAGE_NUMBER    			0x12
#define 	CBL_FLASH_MASS_ERASE         			0xFF

#define 	HAL_SUCCESSFUL_ERASE         			0xFFFFFFFFU

#define 	ADDRESS_IS_INVALID           			0x00
#define 	ADDRESS_IS_VALID             			0x01

#define 	STM32F103_SRAM_SIZE         			(20 * 1024)
#define 	STM32F103_FLASH_SIZE         			(64 * 1024)
#define 	STM32F103_SRAM_END          			(SRAM_BASE + STM32F103_SRAM_SIZE)
#define 	STM32F103_FLASH_END          			(FLASH_BASE + STM32F103_FLASH_SIZE)

#define 	FLASH_PAYLOAD_WRITE_FAILED  			0x00
#define 	FLASH_PAYLOAD_WRITE_PASSED  			0x01

#define 	FLASH_SECTOR2_BASE_ADDRESS   			0x08008000U

#define 	SEND_NACK        						0xAB
#define 	SEND_ACK         						0xCD


#define 	CBL_VENDOR_ID                			76
#define 	CBL_SW_MAJOR_VERSION         			4
#define 	CBL_SW_MINOR_VERSION         			4
#define 	CBL_SW_PATCH_VERSION         			1


#define 	CRC_TYPE_SIZE_BYTE           			4


typedef enum
{
	BL_NACK=0,
	BL_ACK,
	BL_NEW_APP
}BL_Status;


typedef enum
{
	CRC_FAIL = 0,
	CRC_PASS
}tCRC_VERIFY;

BL_Status BL_FeatchHostCommand();
void BL_Print_Message(char *format,...);


typedef void (*pMainApp)(void);
typedef void (*Jump_Ptr)(void);
typedef void (*FunctionPointer)(void);

#endif //__BOOTLOADER_H__
