#ifndef UTILS_H
    #define UTILS_H
#endif


#ifndef __ASSEMBLER__
    #include <stdint.h>
#endif

#ifdef __ASSEMBLER__
    #define REG(addr)                              (addr)
#else
    #define REG(addr)                              (*(volatile uint32_t *)(addr))
#endif

#define BIT(x)                                     ((uint32_t) 1U << (x))
