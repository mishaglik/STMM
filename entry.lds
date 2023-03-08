 ENTRY(__reset_handler);

FLASH_VADDR = 0x08000000;
FLASH_SIZE  = 0x00010000;
SRAM_VADDR  = 0x20000000;
SRAM_SIZE   = 0x00002000;

MEMORY
{
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 0x02000
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 0x10000
}

SECTIONS
{
    . = 0x00000000;

    .text :
    {
        KEEP(*(.vector_table));
        *(.text)    
        *(.rodata)
    }>FLASH

    . = ALIGN(4);
    _etext = .;
    _sidata = _etext;

    .data : AT(_sidata)
    {
        . = ALIGN(4);
        _sdata = .;

        *(.data)
        *(.data.*)

        . = ALIGN(4);
        _edata = .;
    } >RAM 

    __stack_start = SRAM_VADDR + SRAM_SIZE;

    /DISCARD/ :
    {
        *(.ARM.attributes)
        *(.comment)
        *(.symtab)
    }
}
