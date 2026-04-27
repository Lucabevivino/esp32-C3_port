.section .entry, "ax"
.global _start
.align 4

_start:
    la t0, reset_handler
    jr t0

.section .vectors, "ax"
.align 8
.global _vector_table

_vector_table:
    .option push
    .option norelax
     j trap_handler


reset_handler:
.option push
.option norelax
    la gp, __global_pointer$
.option pop
    la      sp, _sp
    
    la t0, _vector_table
    csrw    mtvec, t0
    
    /* Copy data sections from ROM into RAM */
    la      t0, _sidata
    la      t1, _edata
    la      t2, _sdata
    j       copy_data_loop_end
copy_data_loop:
    lw      t3, 0(t0)
    addi    t0, t0, 4
    addi    t2, t2, 4
    sw      t3, -4(t2)
copy_data_loop_end:
    bltu    t2, t1, copy_data_loop

    /* Clear bss sections */
    la      t0, _sbss
    la      t1, _ebss
    j       clear_bss_loop_end
clear_bss_loop:
    sw      zero, 0(t0)
    addi    t0, t0, 4
clear_bss_loop_end:
    bltu    t0, t1, clear_bss_loop

    /* Run the main function */
    la t0,   main
    jalr ra, t0

    ebreak
end_loop:
    j       end_loop

trap_handler:
    la t0, freertos_risc_v_trap_handler
    jr t0
/*
.global riscv_cycles
riscv_cycles:
    rdcycleh a1
    rdcycle a0
    rdcycleh t0
    bne     a1, t0, riscv_cycles
    ret

.global riscv_instret
riscv_instret:
    rdinstreth a1
    rdinstret a0
    rdinstreth t0
    bne     a1, t0, riscv_instret
    ret
*/
