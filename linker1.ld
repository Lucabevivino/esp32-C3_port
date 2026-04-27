ENTRY(_start)

MEMORY
{
  iram (rx)  : ORIGIN = 0x40380000, LENGTH = 128K
  dram (rw)  : ORIGIN = 0x3FC80000, LENGTH = 128K

  irom (rx)  : ORIGIN = 0x42000000, LENGTH = 4M
  drom (r)   : ORIGIN = 0x3C000000, LENGTH = 4M
}

SECTIONS
{
  /* =========================
   * Vector table (interrupts)
   * ========================= */
  .vectors :
  {
      . = ALIGN(256);
      _vector_table = .;
      KEEP(*(.vectors))
  } > iram


  /* =========================
   * Code in flash
   * ========================= */
  .text :
  {
    . = ALIGN(4);
    _stext = .;

    *(.entry)
    *(.text*)
    *(.literal*)

    _etext = .;
  } > iram


  /* =========================
   * Read-only data
   * ========================= */
  .rodata :
  {
    . = ALIGN(4);
    _srodata = .;

    *(.rodata*)
    *(.srodata*)

    _erodata = .;
  } > drom


  /* =========================
   * IRAM (ISR veloci)
   * ========================= */
  .iram_text :
  {
    . = ALIGN(4);
    *(.iram*)
    *(.iram.text*)
  } > iram


  /* =========================
   * Data inizializzata
   * ========================= */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;

    *(.data*)
    *(.sdata*)

    _edata = .;
  } > dram AT > drom

  _sidata = LOADADDR(.data);

  __global_pointer$ = _sdata + 0x800;


  /* =========================
   * BSS (heap FreeRTOS sta qui!)
   * ========================= */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;

    *(.bss*)
    *(.sbss*)
    *(COMMON)

    _ebss = .;
  } > dram


  /* =========================
   * Stack
   * ========================= */
  .stack (NOLOAD):
  {
    . = ALIGN(16);

    _stack_top = ORIGIN(dram) + LENGTH(dram);
    _stack_bottom = _stack_top - 8192;

  } > dram

  _sp = _stack_top;
  
  /* =========================
   * IRQ stack (FreeRTOS)
   * ========================= */
  .irq_stack (NOLOAD):
  {
    . = ALIGN(16);

    _irq_stack_bottom = .;
    . += 2048;   /* 2 KB IRQ stack */
    __freertos_irq_stack_top = .;

  } > dram

  /* =========================
   * Debug
   * ========================= */
  _ram_end = ORIGIN(dram) + LENGTH(dram);
}
