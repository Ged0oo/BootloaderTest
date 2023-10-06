/* 
 * File:   LinkerScript.ld
 * Author: Mohamed_Nagy
 * https://github.com/Ged0oo 
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on March 20, 2023, 6:41 PM
 */



/* Highest address of the user mode stack */
/* End of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM); 

/* Define the vector table offset 
 * (start of application code) 
 */
__isr_vector_start__ = ORIGIN(APP_FLASH);

/* Required amount of heap */
_Min_Heap_Size = 0x200 ;  

/* Required amount of stack */
_Min_Stack_Size = 0x400 ; 



/* Memories definition -> Label_Name : ORIGIN = StartMemoryAddress ,   LENGTH = MemoryLength */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 20K
  BL_FLASH  (rx)     : ORIGIN = 0x8000000,   LENGTH = 32K
  APP_FLASH  (rx)     : ORIGIN = 0x8008000,   LENGTH = 32K
}



/* Sections Definition */
SECTIONS
{
  /* The startup code into "FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    *(.isr_vector)
    . = ALIGN(4);
  } >APP_FLASH



  /* .text section, The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    . = ALIGN(4);
    _etext = .;        /* Define a global symbols at end of code */
  } >APP_FLASH



  /* .rodata section, Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >APP_FLASH




  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);



  /* .data section, Initialized data sections into "RAM" Ram type memory */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* Create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    . = ALIGN(4);
    _edata = .;        /* Define a global symbol at data end */
  } >RAM AT> APP_FLASH



  /* .bss section, Uninitialized data section into "RAM" Ram type memory */
   . = ALIGN(4);
  .bss :
  {
    _sbss = .;         /* Define a global symbol at bss start */
    *(.bss)            /* .bss sections */
    *(.bss*)           /* .bss* sections */
    . = ALIGN(4);
    _ebss = .;         /* Define a global symbol at bss end */
  } >RAM



  /* .User_heap_stack section, used to check that there is enough "RAM" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM
}
