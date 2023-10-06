/* 
 * File:   startup_stm32f103xxx.ld
 * Author: Mohamed_Nagy
 * https://github.com/Ged0oo 
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on March 20, 2023, 6:41 PM
 */

#define NULL 0
extern int main(void);
typedef unsigned long uint32_t; 
extern uint32_t _estack ,_etext,_sdata,_edata,_sbss,_ebss,_sidata, __isr_vector_start__;
uint32_t * const MSP_value = (uint32_t*) &_estack;
volatile uint32_t *VTOR = (volatile uint32_t *)0xE000E010;

void app_reset_handler                       (void);
void NMI_Handler                        (void);
void HardFault_Handler                  (void);
void MemManage_Handler                  (void);
void BusFault_Handler                   (void);
void UsageFault_Handler                 (void);
void SVC_Handler                        (void);
void DebugMon_Handler                   (void);
void PendSV_Handler					    (void);
void SysTick_Handler					(void);
void WWDG_IRQHandler					(void);
void PVD_IRQHandler						(void);
void TAMPER_IRQHandler					(void);
void RTC_IRQHandler						(void);
void FLASH_IRQHandler					(void);
void RCC_IRQHandler						(void);
void EXTI0_IRQHandler					(void);
void EXTI1_IRQHandler					(void);
void EXTI2_IRQHandler					(void);
void EXTI3_IRQHandler					(void);
void EXTI4_IRQHandler					(void);
void DMA1_Channel1_IRQHandler			(void);
void DMA1_Channel2_IRQHandler			(void);
void DMA1_Channel3_IRQHandler			(void);
void DMA1_Channel4_IRQHandler			(void);
void DMA1_Channel5_IRQHandler			(void);
void DMA1_Channel6_IRQHandler			(void);
void DMA1_Channel7_IRQHandler			(void);
void ADC1_2_IRQHandler					(void);
void USB_HP_CAN1_TX_IRQHandler			(void);
void USB_LP_CAN1_RX0_IRQHandler			(void);
void CAN1_RX1_IRQHandler				(void);
void CAN1_SCE_IRQHandler				(void);
void EXTI9_5_IRQHandler					(void);
void TIM1_BRK_IRQHandler				(void);
void TIM1_UP_IRQHandler					(void);
void TIM1_TRG_COM_IRQHandler			(void);
void TIM1_CC_IRQHandler					(void);
void TIM2_IRQHandler					(void);
void TIM3_IRQHandler					(void);
void TIM4_IRQHandler					(void);
void I2C1_EV_IRQHandler					(void);
void I2C1_ER_IRQHandler					(void);
void I2C2_EV_IRQHandler					(void);
void I2C2_ER_IRQHandler					(void);
void SPI1_IRQHandler					(void);
void SPI2_IRQHandler					(void);
void USART1_IRQHandler					(void);
void USART2_IRQHandler					(void);
void USART3_IRQHandler					(void);
void EXTI15_10_IRQHandler 				(void);
void RTC_Alarm_IRQHandler 				(void);
void USBWakeUp_IRQHandler 				(void);
void BootRAM							(void);


uint32_t *vector_Table[] __attribute__ ((section (".isr_vector"))) =
{
	(uint32_t*)MSP_value,
	(uint32_t*)&app_reset_handler,
	(uint32_t*)&NMI_Handler,
	(uint32_t*)&HardFault_Handler,
	(uint32_t*)&MemManage_Handler,
	(uint32_t*)&BusFault_Handler,
	(uint32_t*)&UsageFault_Handler,
	 0, 0, 0, 0,                             
	(uint32_t*)& SVC_Handler,
	(uint32_t*)& DebugMon_Handler,
	 0,
	(uint32_t*)& PendSV_Handler,
	(uint32_t*)& SysTick_Handler,
	(uint32_t*)& WWDG_IRQHandler,
	(uint32_t*)& PVD_IRQHandler,
	(uint32_t*)& TAMPER_IRQHandler,
	(uint32_t*)& RTC_IRQHandler,
	(uint32_t*)& FLASH_IRQHandler,
	(uint32_t*)& RCC_IRQHandler,
	(uint32_t*)& EXTI0_IRQHandler,
	(uint32_t*)& EXTI1_IRQHandler,
	(uint32_t*)& EXTI2_IRQHandler,
	(uint32_t*)& EXTI3_IRQHandler,
	(uint32_t*)& EXTI4_IRQHandler,
	(uint32_t*)& DMA1_Channel1_IRQHandler,
	(uint32_t*)& DMA1_Channel2_IRQHandler,
	(uint32_t*)& DMA1_Channel3_IRQHandler,
	(uint32_t*)& DMA1_Channel4_IRQHandler,
	(uint32_t*)& DMA1_Channel5_IRQHandler,
	(uint32_t*)& DMA1_Channel6_IRQHandler,
	(uint32_t*)& DMA1_Channel7_IRQHandler,
	(uint32_t*)& ADC1_2_IRQHandler,
	(uint32_t*)& USB_HP_CAN1_TX_IRQHandler,
	(uint32_t*)& USB_LP_CAN1_RX0_IRQHandler,
	(uint32_t*)& CAN1_RX1_IRQHandler,
	(uint32_t*)& CAN1_SCE_IRQHandler,
	(uint32_t*)& EXTI9_5_IRQHandler,
	(uint32_t*)& TIM1_BRK_IRQHandler,
	(uint32_t*)& TIM1_UP_IRQHandler,
	(uint32_t*)& TIM1_TRG_COM_IRQHandler,
	(uint32_t*)& TIM1_CC_IRQHandler,
	(uint32_t*)& TIM2_IRQHandler,
	(uint32_t*)& TIM3_IRQHandler,
	(uint32_t*)& TIM4_IRQHandler,
	(uint32_t*)& I2C1_EV_IRQHandler,
	(uint32_t*)& I2C1_ER_IRQHandler,
	(uint32_t*)& I2C2_EV_IRQHandler,
	(uint32_t*)& I2C2_ER_IRQHandler,
	(uint32_t*)& SPI1_IRQHandler,
	(uint32_t*)& SPI2_IRQHandler,
	(uint32_t*)& USART1_IRQHandler,
	(uint32_t*)& USART2_IRQHandler,
	(uint32_t*)& USART3_IRQHandler,
	(uint32_t*)& EXTI15_10_IRQHandler,
	(uint32_t*)& RTC_Alarm_IRQHandler,
	(uint32_t*)& USBWakeUp_IRQHandler,
	 0, 0, 0, 0, 0, 0, 0,
	(uint32_t*)&BootRAM,        
};

void app_reset_handler(void)
{
	// Initialize stack pointer
    __asm volatile("LDR  R0, =_estack");
    __asm volatile("MOV  SP, R0");
	*VTOR = __isr_vector_start__;

	uint32_t section_Size = 0;
	
	/* Copieing data segment from flash to RAM */
	uint32_t *MemsourceAdd = (uint32_t*)&_sidata;
	uint32_t *MemDestAdd = (uint32_t*)&_sdata;
	section_Size = &_edata - &_sdata;
	for(uint32_t memoryCounter = 0 ; memoryCounter < section_Size ; memoryCounter++) *MemDestAdd++ = *MemsourceAdd++;
	
	/* Initializing the .bss segment by zero */
	section_Size = &_ebss - &_sbss;
	MemDestAdd = &_sbss;
	for(uint32_t memoryCounter = 0 ; memoryCounter < section_Size ; memoryCounter++) *MemDestAdd++ = 0;

	/* Calls the main function */
	main();
	
	// If main() returns, we should not return from Reset_Handler
    while (1) {}
}

void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{

}

void MemManage_Handler(void)
{

}
	
void BusFault_Handler(void)
{

}

void UsageFault_Handler(void)
{

}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{
	
}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{

}

void WWDG_IRQHandler(void)
{

}

void PVD_IRQHandler(void)
{

}

void TAMPER_IRQHandler(void)
{

}

void RTC_IRQHandler(void)
{

}

void FLASH_IRQHandler(void)
{

}

void RCC_IRQHandler(void)
{

}

void EXTI0_IRQHandler(void)
{
	
}

void EXTI1_IRQHandler(void)
{
	
}

void EXTI2_IRQHandler(void)
{
	
}

void EXTI3_IRQHandler(void)
{
	
}

void EXTI4_IRQHandler(void)
{
	
}

void DMA1_Channel1_IRQHandler(void)
{
	
}

void DMA1_Channel2_IRQHandler(void)
{
	
}

void DMA1_Channel3_IRQHandler(void)
{
	
}

void DMA1_Channel4_IRQHandler(void)
{
	
}

void DMA1_Channel5_IRQHandler(void)
{
	
}

void DMA1_Channel6_IRQHandler(void)
{
	
}

void DMA1_Channel7_IRQHandler(void)
{
	
}

void ADC1_2_IRQHandler(void)
{
	
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	
}

void CAN1_RX1_IRQHandler(void)
{

}

void CAN1_SCE_IRQHandler(void)
{

}

void EXTI9_5_IRQHandler(void)
{

}

void TIM1_BRK_IRQHandler(void)
{

}

void TIM1_UP_IRQHandler(void)
{

}

void TIM1_TRG_COM_IRQHandler(void)
{

}

void TIM1_CC_IRQHandler(void)
{

}

void TIM2_IRQHandler(void)
{
	
}

void TIM3_IRQHandler(void)
{
	
}

void TIM4_IRQHandler(void)
{
	
}

void I2C1_EV_IRQHandler(void)
{
	
}

void I2C1_ER_IRQHandler(void)
{
	
}

void I2C2_EV_IRQHandler(void)
{
	
}

void I2C2_ER_IRQHandler(void)
{
	
}

void SPI1_IRQHandler(void)
{
	
}

void SPI2_IRQHandler(void)
{
	
}

void USART1_IRQHandler(void)
{
	
}

void USART2_IRQHandler(void)
{
	
}

void USART3_IRQHandler(void)
{
	
}

void EXTI15_10_IRQHandler(void)
{
	
}

void RTC_Alarm_IRQHandler(void)
{
	
}

void USBWakeUp_IRQHandler(void)
{
	
}

void BootRAM(void)
{
	
}

void Default_Handler(void)
{
	
}