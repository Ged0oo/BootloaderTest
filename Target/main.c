#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"


extern RCC_Config_t RCC_obj;


GPIO_ConfigType led = 
{
	.GPIOx = GPIOA,
	.GPIO_PinNumber = GPIO_PIN_0,
	.GPIO_PinMode = GPIO_PIN_GENERAL_PURPOSE_OUTPUT_PUSHPULL_MODE_2MHZ,
	.GPIO_Logic = GPIO_LOW
};

int main(void)
{	
	MRCC_voidClockInit(&RCC_obj);
	MRCC_voidPeripheralClockEnable(RCC_APB2_BUS , RCC_GPIOA_CLOCK);
	MRCC_voidPeripheralClockEnable(RCC_APB2_BUS , RCC_GPIOB_CLOCK);
	
	GPIO_voidInitPortPin(&led);
	while(1)
	{
		GPIO_voidWritePortPin(&led, GPIO_LOW);
		for(uint16 c=0;c<40000;c++) __asm volatile("NOP");
		GPIO_voidWritePortPin(&led, GPIO_HIGH);
		for(uint16 c=0;c<40000;c++) __asm volatile("NOP");
	}
}

