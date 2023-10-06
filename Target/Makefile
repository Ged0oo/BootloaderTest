TCH = arm-none-eabi-
OPT = -mcpu=cortex-m3 -mthumb
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all : Application.bin

Application.bin : Application.hex
	$(TCH)objcopy -I ihex -O binary $< $@
	
Application.hex : Application.elf
	$(TCH)objcopy -O ihex $< $@

Application.elf : $(OBJ)
	$(TCH)ld -T LinkerScript.ld -Map=output.map $(OBJ) -o $@

%.o : %.c
	$(TCH)gcc -c $(OPT) $< -o $@
		
clean : 
	del *.o *.hex *.elf