ENTRY(_start)

MEMORY
{
  flash (rx) : ORIGIN = 0x0, LENGTH = 4M 
  /* IRAM: Primi 64KB della SRAM (Bus Istruzioni) */
  iram (rx) : ORIGIN = 0x40380000, LENGTH = 64K
  /* DRAM: Restante SRAM (Bus Dati). Inizia dopo i 64KB della IRAM fisica */
  /* 0x3FC80000 + 64K (0x10000) = 0x3FC90000 */
  dram (rw) : ORIGIN = 0x3FC90000, LENGTH = 200K 
}

SECTIONS
{
  /* 1. Codice eseguibile (IRAM) */
  .text :
  {
    _stext = .;
    KEEP(*(.entry))       /* _start deve essere qui */
    
    . = ALIGN(256);       /* Forza l'allineamento per i vettori */
    _vector_table = .;
    KEEP(*(.vectors))
    *(.iram .iram.*)
    *(.text .text.*)
    . = ALIGN(4);
    _etext = .;
  } > iram AT > flash
 
  _sitext = LOADADDR(.text);

  /* Calcoliamo il nuovo indirizzo di carico per i dati */
  _sidata = ALIGN(LOADADDR(.text) + SIZEOF(.text), 4);

  /* 2. Dati inizializzati (DRAM) */
  /* Verranno caricati in Flash ma risiederanno in RAM */
  .data : AT(_sidata)
  {
     __global_pointer$ = . + 0x800;
    _sdata = .;
    *(.data .data.*)
    *(.rodata .rodata.*)
    *(.srodata .srodata.*)
    *(.sdata .sdata.*)
    . = ALIGN(4);
    _edata = .;
  } > dram

  /* 3. Dati non inizializzati (BSS) */
  .bss (NOLOAD) :
  {  
     . = ALIGN(4);
    _sbss = .;
    *(.bss .bss.*)
    *(.sbss .sbss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > dram

  
  /* 4. Definizione dello Stack (punta alla fine della DRAM) */
  _stack_top = ORIGIN(dram) + LENGTH(dram);
  __freertos_irq_stack_top = _stack_top;

  _end = _ebss;
  PROVIDE(end = _end);
  /*
  /DISCARD/ :
  {
    *(.riscv.attributes)
    *(.comment)
    *(.note.GNU-stack)
    *(.debug_*)
    *(.eh_frame)
    *(.eh_frame_hdr)
  }
  */
}
