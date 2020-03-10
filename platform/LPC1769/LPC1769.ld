/**
 * @file LPC1769.ld
 * @author LiYu87
 * @brief Linker script for LPC1769
 * @date 2020.02.17
 */

STACK_SIZE = 0x800;

ENTRY(Reset_Handler)

/**
 * @brief Linker script to configure memory regions.
 * 
 * SRAM  topmost 32 bytes used by IAP functions
 */
MEMORY
{
    FLASH  (RX)  : ORIGIN = 0x00000000, LENGTH = 512K
    SRAM    (RWX) : ORIGIN = 0x10000000, LENGTH = (32K - 32)

    USB_RAM(RWX) : ORIGIN = 0x2007C000, LENGTH = 16K
    ETH_RAM(RWX) : ORIGIN = 0x20080000, LENGTH = 16K
}

/** 
 * Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and SRAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 * 
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 */
SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))

        /* Code Read Protect data */
        . = 0x000002FC;
        KEEP(*(.crp))
        
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))

    } > FLASH

    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    __etext = .;

    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)

        . = ALIGN(8);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(8);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);


        . = ALIGN(8);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);

        . = ALIGN(8);
        /* All data end */
        __data_end__ = .;

    } > SRAM

    
    .bss :
    {
		. = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > SRAM

    
    .heap :
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
        . = ORIGIN(SRAM) + LENGTH(SRAM) - STACK_SIZE;
        __HeapLimit = .;
    } > SRAM

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy :
    {
        *(.stack)
    } > SRAM

    /* Set stack top to end of SRAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM) + LENGTH(SRAM);
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);
    
    /* Check if data + heap + stack exceeds SRAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region SRAM overflowed with stack")


    /* Code can explicitly ask for data to be 
       placed in these higher SRAM banks where
       they will be left uninitialized. 
    */
    .AHBSRAM0 (NOLOAD):
    {
        *(AHBSRAM0)
    } > USB_RAM

    .AHBSRAM1 (NOLOAD):
    {
        *(AHBSRAM1)
    } > ETH_RAM
}