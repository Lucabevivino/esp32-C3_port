MEMORY {
  iache  (rwx)  : ORIGIN = 0X4037c000, LENGTH = 16k
  iram   (rwx)  : ORIGIN = 0x40380400, LENGTH = 32k 
  dram   (rw)   : ORIGIN = 0x3fc80000 + LENGTH(iram), LENGTH = 128k
}

_eram = ORIGIN(dram) + LENGTH(dram);
ENTRY(_start)

SECTIONS {
 
  .entry : {
      KEEP(*(.entry))
  } > iram
  
  .vectors : ALIGN(256) {
      _vector_table = .;
      KEEP(*(.vectors))
  } > iram

  .text     : { 
      KEEP(*(.init))
      *(.text) 
      *(.text*) 
      KEEP(*(.fini))
  } > iram
  
  _sidata = LOADADDR(.data);

  .data : {
    . = ALIGN(16);
    PROVIDE(__global_pointer$ = . + 0x800);
    _sdata = .;
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
  } > dram AT > iram

  .stack (NOLOAD) : {
    . = ALIGN(16);
    _stack_bottom = .;
    . += 2048; 
    _stack_top = .;
    __freertos_irq_stack_top = .;
    _sp = .;    /* Questo simbolo lo userai nello startup.S */
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

  heap_start = _ebss;
  heap_end = _eram;
  
  . = ALIGN(16);
  PROVIDE(end = .);
  PROVIDE(_end = .);
}

/*
PROVIDE(uart_tx_one_char = 0x40000068);
PROVIDE(uart_tx_one_char2 = 0x4000006c);
PROVIDE(uart_rx_one_char = 0x40000070);
PROVIDE(uart_rx_one_char_block = 0x40000074);
PROVIDE(uart_rx_readbuff = 0x40000078);

PROVIDE(memset = 0x40000354);
PROVIDE(memcpy = 0x40000358);
PROVIDE(memmove = 0x4000035c);

PROVIDE(memccpy = 0x400003c4);
PROVIDE(memchr = 0x400003c8);
PROVIDE(memrchr = 0x400003cc);
PROVIDE(memcmp = 0x40000360);
PROVIDE(printf = 0x40000040);

PROVIDE(__udivdi3 = 0x400008ac);
PROVIDE(__umoddi3 = 0x400008bc);
PROVIDE(__divdi3 = 0x400007b4);
*/
