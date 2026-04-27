
ENTRY(_start)

MEMORY {
  iram   (rwx)  : ORIGIN = 0x40380000, LENGTH = 128k 
  dram   (rw)   : ORIGIN = 0x3fc80000, LENGTH = 128k

  irom   (rx)   : ORIGIN = 0x42000000, LENGTH = 4M
  drom   (r)    : ORIGIN = 0x3C000000, LENGTH = 4M
}

SECTIONS {
 
  /* .entry 0x40380000 : {
      KEEP(*(.entry))
  } > iram
  */
  
  .vectors : ALIGN(256) {
      _vector_table = .;
      KEEP(*(.vectors))
  } > iram

  .text     : ALIGN(16) { 
      KEEP(*(.init))
      *(.text) 
      *(.text.*) 
      KEEP(*(.fini))
  } > iram
  
  _sidata = LOADADDR(.data);

  .data : {
    . = ALIGN(16);
    _sdata = .;
    
    KEEP(*(.data))
    *(.data*)
    
    __global_pointer$ = . + 0x800;
    
    KEEP(*(.sdata))
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
  
  .stack (NOLOAD) : {
    . = ALIGN(16);
    _stack_bottom = .;
    . += 2048; 
    _stack_top = .;
    __freertos_irq_stack_top = .;
    _sp = .;    /* Questo simbolo lo userai nello startup.S */
  } > dram 


  heap_start = ALIGN(_stack_top, 16);
  heap_end = _eram;
  
  . = ALIGN(16);
  PROVIDE(end = .);
  PROVIDE(_end = .);
}

EXTERN(__global_pointer$)

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
