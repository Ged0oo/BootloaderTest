#ifndef	__BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "usart.h"
#include "crc.h"
#include "../stm32f1xx_hal_flash_ex.h"
#include "../stm32f1xx_hal_def.h"

#define 	BL_DEBUG_UART							&huart1
#define 	BL_HOST_COMMUNICATION_UART				&huart2
#define 	CRC_ENGINE             					&hcrc

#define 	DEBUG_INFO_DISABLE           			0
#define 	DEBUG_INFO_ENABLE            			1
#define 	BL_DEBUG_ENABLE              			DEBUG_INFO_ENABLE

#define 	BL_ENABLE_UART_DEBUG_MESSAGE			0x00
#define 	BL_ENABLE_SPI_DEBUG_MESSAGE				0x01
#define 	BL_ENABLE_I2C_DEBUG_MESSAGE				0x02
#define 	BL_ENABLE_CAN_DEBUG_MESSAGE				0x03

#define 	BL_DEBUG_METHOD							BL_ENABLE_UART_DEBUG_MESSAGE
#define 	BL_HOST_BUFFER_RX_LENGTH				200

/* Bootloader Comands */
#define 	CBL_GET_VER_CMD              			0
#define 	CBL_GET_HELP_CMD             			1
#define 	CBL_GET_CID_CMD              			2

/* Get Read Protection Status */
#define 	CBL_GET_RDP_STATUS_CMD       			3
#define 	CBL_GO_TO_ADDR_CMD           			4
#define 	CBL_FLASH_ERASE_CMD          			5
#define 	CBL_MEM_WRITE_CMD            			6

/* Enable/Disable Write Protection */
#define 	CBL_ED_W_PROTECT_CMD         			7
#define 	CBL_MEM_READ_CMD             			8

/* Get Sector Read/Write Protection Status */
#define 	CBL_READ_SECTOR_STATUS_CMD   			9
#define 	CBL_OTP_READ_CMD             			10

/* Change Read Out Protection Level */
#define 	CBL_CHANGE_ROP_Level_CMD     			11

#define 	CBL_VENDOR_ID                			100
#define 	CBL_SW_MAJOR_VERSION         			1
#define 	CBL_SW_MINOR_VERSION         			1
#define 	CBL_SW_PATCH_VERSION         			0

#define 	CRC_TYPE_SIZE_BYTE           			4

#define 	CBL_SEND_NACK                			0xAB
#define 	CBL_SEND_ACK                 			0xCD

/* Sector 2 Start Address */
#define 	FLASH_SECTOR2_BASE_ADDRESS   			0x08008000U

#define 	ADDRESS_IS_INVALID           			0x00
#define 	ADDRESS_IS_VALID             			0x01

#define 	STM32F103_SRAM_SIZE         			(20 * 1024)
#define 	STM32F103_FLASH_SIZE         			(128 * 1024)
#define 	STM32F103_SRAM_END          			(SRAM_BASE + STM32F103_SRAM_SIZE)
#define 	STM32F103_FLASH_END          			(FLASH_BASE + STM32F103_FLASH_SIZE)

/* CBL_FLASH_ERASE_CMD */
#define 	CBL_FLASH_MAX_SECTOR_NUMBER  			12
#define 	CBL_FLASH_MASS_ERASE         			0xFF
#define 	HAL_SUCCESSFUL_ERASE         			0xFFFFFFFFU
#define 	INVALID_SECTOR_NUMBER        			0x00
#define 	VALID_SECTOR_NUMBER          			0x01
#define 	UNSUCCESSFUL_ERASE           			0x02
#define 	SUCCESSFUL_ERASE             			0x03

/* CBL_MEM_WRITE_CMD */
#define 	FLASH_PAYLOAD_WRITE_FAILED   			0x00
#define 	FLASH_PAYLOAD_WRITE_PASSED   			0x01
#define 	FLASH_LOCK_WRITE_FAILED      			0x00
#define 	FLASH_LOCK_WRITE_PASSED      			0x01

/* CBL_GET_RDP_STATUS_CMD */
#define 	ROP_LEVEL_READ_INVALID       			0x00
#define 	ROP_LEVE_READL_VALID         			0X01

/* CBL_CHANGE_ROP_Level_CMD */
#define 	ROP_LEVEL_CHANGE_INVALID     			0x00
#define 	ROP_LEVEL_CHANGE_VALID       			0X01

#define 	CBL_ROP_LEVEL_1              			0x01
#define 	CBL_ROP_LEVEL_0              			0x00
#define 	CBL_ROP_LEVEL_2              			0x02

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

typedef void (*pMainApp)(void);
typedef void (*Jump_Ptr)(void);


void BL_Print_Message(char *format, ...);
BL_Status BL_UART_Fetch_Host_Command(void);


#endif
