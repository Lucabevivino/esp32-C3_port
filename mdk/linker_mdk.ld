MEMORY {
  iache  (rwx)  : ORIGIN = 0X4037c000, LENGTH = 16k
  iram   (rwx)  : ORIGIN = 0x40380400, LENGTH = 32k 
  dram   (rw)   : ORIGIN = 0x3fc80000 + LENGTH(iram), LENGTH = 128k
}

_eram = ORIGIN(dram) + LENGTH(dram);

__freertos_irq_stack_top = _eram; 
PROVIDE(__freertos_irq_stack_top = _eram);

ENTRY(_reset)

SECTIONS {
  .text     : {
    . = ALIGN(256);
    KEEP(*(.vectors))
    *(.text) 
    *(.text*) 
 } > iram

  .data : {
    . = ALIGN(16);
    _sdata = .;
    __global_pointer$ = . + 0x800;
    *(.data)
    *(.data*)
    *(.sdata)
    *(.sdata*)
    *(.srodata)
    *(.srodata*)
    *(.rodata)
    *(.rodata*)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    *(.riscv.*)
    . = ALIGN(16);
    _edata = .;
  } > dram

  .bss (NOLOAD): {
    . = ALIGN(16);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(.sbss)
    *(.sbss*)
    *(COMMON)
    . = ALIGN(16);
    _ebss = .;
  } > dram

  . = ALIGN(16);
  PROVIDE(end = .);
  PROVIDE(_end = .);
}

