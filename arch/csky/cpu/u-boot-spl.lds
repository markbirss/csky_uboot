
MEMORY
{
    MEM0   : ORIGIN = 0x0        , LENGTH = 0x17FFFFFF    /* DDR 384MB */
    MEM1   : ORIGIN = 0x1fc00000 , LENGTH = 0x3FFF        /* ROM 16KB */
    MEM2   : ORIGIN = 0x1fd00000 , LENGTH = 0x2FFF        /* SRAM 12KB */
    MEM3   : ORIGIN = 0x1fd03000 , LENGTH = 0xFFF        /* SRAM 4KB */
}

REGION_ALIAS("REGION_TEXT",   MEM2);
REGION_ALIAS("REGION_RODATA", MEM2);
REGION_ALIAS("REGION_DATA",   MEM2);
REGION_ALIAS("REGION_BSS",    MEM2);

PROVIDE (__stack =  0x1fd04000 - 0x8);
PROVIDE (__fstack =  0x1fd04000 - 0x900 - 0x8);
OUTPUT_ARCH(csky)
ENTRY(_start)

SECTIONS
{
    .text ALIGN(0x4):
    {
        __text_start = .;
        *(.text)
        *(.text.*)
        __text_end = .;
    } > REGION_TEXT

    .rodata ALIGN(0x4):
    {
        __rodata_start = .;
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
        __rodata_end = .;
    } > REGION_RODATA

    .data ALIGN(0x4):
    {
        __data_start = .;
        *(.data)
        *(.data.*)
        __data_end = .;
    } > REGION_DATA

    . = ALIGN(4);
    .u_boot_list : {
        KEEP(*(SORT(.u_boot_list*)));
    }

    __init_end = . ;

    .bss ALIGN(0x4):
    {
        __bss_start = .;
        *(.sbss)
        *(.scommon)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > REGION_BSS
    __end = .;
}

