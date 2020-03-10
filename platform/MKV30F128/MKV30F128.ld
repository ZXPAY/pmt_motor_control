/**
 * @file MKV30F128.ld
 * @author LiYu87
 * @brief Linker script for MKV30F128 series
 * @date 2020.02.17
 */

STACK_SIZE = 0x800;

ENTRY(Reset_Handler)

/**
 * @brief Linker script to configure memory regions.
 *
 * SRAM_L used for text and data.
 * SRAM_U used for stack and heap.
 */
MEMORY
{
    FLASH  (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00020000
    SRAM_L (RWX) : ORIGIN = 0x1FFFE000, LENGTH = 0x00002000
    SRAM_U (RWX) : ORIGIN = 0x20000000, LENGTH = 0x00002000
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
        /* isr_vector 1024 bytes */
        KEEP(*(.isr_vector))

        /* flash_config 16 bytes */
        . = 0x00000400;
        KEEP(*(.flash_config))

        /* secure region */
        . = 0x00000410;

        /* code and text */
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

    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > FLASH

    __etext = .;

    .data : AT (__etext)
    {
        . = ALIGN(4);
        __DATA_RAM = .;
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

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > SRAM_L

    .bss :
    {
		. = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > SRAM_L

    .heap :
    {
        . = ALIGN(8);
        __end__ = .;
        end = __end__;
        *(.heap*)
        . = ORIGIN(SRAM_U) + LENGTH(SRAM_U) - STACK_SIZE;
        __HeapLimit = .;
    } > SRAM_U

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy :
    {
        . = ALIGN(8);
        . += STACK_SIZE;
    } > SRAM_U

    /* Set stack top to end of SRAM_U, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM_U) + LENGTH(SRAM_U);
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds SRAM_U limit */
    ASSERT(__StackLimit >= __HeapLimit, "region SRAM_U overflowed with stack")
}

