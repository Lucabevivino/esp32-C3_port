#include "mdk.h"

extern int main(void);
extern char _sbss, _ebss, _end, _eram;
extern char __global_pointer$; 

extern void freertos_risc_v_trap_handler(void);

__asm__ (
    ".section .vectors, \"ax\"\n"
    ".option push\n"
    ".option norelax\n"
    ".global _vector_table\n"
    "_vector_table:\n"
    "j freertos_risc_v_trap_handler\n"
    "   .align 2\n"
    "   .rept 31\n"
    "   j freertos_risc_v_trap_handler\n"
    "   .endr\n"
    ".option pop\n"
    "\n" 
    ".section .text\n"
    ".global _reset\n"
    "_reset:\n"
    ".option push\n"
    ".option norelax\n"
    "   la gp, __global_pointer$\n"
    "   .option pop\n"
    "   la sp, _eram\n"
    "   j _start\n"
);
void _start(void) {
    
    for (char *p = &_sbss; p < &_ebss;) *p++ = '\0';
    __asm__(
        "la t0, _vector_table\n"
        "csrw mtvec, t0\n"
    );
    wdt_disable();
    soc_init();
    main();
    
    for (;;) (void) 0;
}
