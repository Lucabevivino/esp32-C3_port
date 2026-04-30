.section .entry, "ax"
.global _start

_start:
    /* 1. Inizializzazione immediata registri critici */
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    la sp, _stack_top          /* Fondamentale prima di mtvec! */

    /* 2. Configura mtvec con allineamento corretto */
    la t0, _vector_table
    /* Assicuriamoci che mtvec sia in Direct Mode (bit 0-1 = 0) */
    csrw mtvec, t0

    j reset_handler

/* Spostiamo la tabella in una sezione allineata seriamente */
.section .vectors, "ax"
.align 8                      /* 2^8 = 256 byte: Obbligatorio per ESP32-C3 */
.global _vector_table
_vector_table:
    /* Questo è il punto 0x40380100 del tuo debug */
    la t0, freertos_risc_v_trap_handler
    jr t0
    /* Riempiamo il resto della tabella con nop o jump per sicurezza */
    .align 4
    nop
    .endr

.section .text
reset_handler:
/* --- 1. Disabilita Watchdog (Fondamentale per ESP32-C3) --- */
    
// Disable write protection
    la t0, 0x60008000
    li t1, 0x50d83aa1
    sw t1, 0xA8(t0)
// Disable RTC wdt
    sw zero, 0x90(t0)
    sw zero, 0x8C(t0)
// Autofeed
    li t1, 0x8F1D312A
    sw t1, 0xB0(t0)
    
    lw t1, 0xAC(t0)
    li t2, (1 << 31)
    or t1, t1, t2
    sw t1, 0xAC(t0)
    sw zero, 0xB4(t0)

// TIMG_REGCLK -> disable TIMG_WDT_CLK    
    la t0, 0x6001F000
    li      t1, 0x50d83aa1
    sw      t1, 0x64(t0)
    sw zero, 0x48(t0)

// Disable TG1 WDT   
    la t0, 0x60020000
    li      t1, 0x50d83aa1
    sw      t1, 0x64(t0)
    sw zero, 0x48(t0)




/* --- Configurazione Trap Vector --- */
    la t0, _vector_table
    csrw mtvec, t0

    /* --- 2. Inizializzazione Registri --- */
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    la sp, _stack_top

    la t0, _siram_extras
    la t1, _eiram_extras
    la t2, _siram_extras_flash        
    beq t0, t1, copy_data    
copy_iram_extras_loop:
    lw t3, 0(t2)
    sw t3, 0(t0)
    addi t0, t0, 4
    addi t2, t2, 4
    bltu t0, t1, copy_iram_extras_loop
    
copy_data:    
    la t0, _sdata
    la t1, _edata
    la t2, _sidata
    beq t0, t1, zero_bss
copy_data_loop:
    lw t3, 0(t2)
    sw t3, 0(t0)
    addi t0, t0, 4
    addi t2, t2, 4
    bltu t0, t1, copy_data_loop 

zero_bss:
    la t0, _sbss
    la t1, _ebss
    li t2, 0
    beq t0, t1,jump_to_main
zero_bss_loop:
    la t4, 0x60008000 
    li t3, 1
    sw t3, 0xB4(t4)

    sw t2, 0(t0)
    addi t0, t0, 4
zero_bss_test:
    bltu t0, t1, zero_bss_loop

jump_to_main:
    /* --- 5. Salto al Main --- */
    jal main

    /* In caso di ritorno dal main, loop infinito */
1:  j 1b
