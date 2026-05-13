#ifndef REGS_SYSTIMER_H
#define REGS_SYSTIMER_H

/**
 * @file port_systimer_regs.h
 * @brief Register definitions for ESP32-C3 SYSTIMER and Interrupt Matrix.
 *
 * This file provides memory-mapped register definitions for the SYSTIMER 
 * peripheral and the Core Interrupt Matrix, specifically tailored for 
 * FreeRTOS bare-metal porting on RISC-V.
 *
 * Reference: ESP32-C3 Technical Reference Manual (TRM).
 */

#ifndef __ASSEMBLER__
    #include <stdint.h>
    #include <FreeRTOS.h>
#endif

/* --------------------------------------------------------------------------
 *   BASE ADDRESSES
 * -------------------------------------------------------------------------- */
#define SYSTEM_BASE_ADDRESS                             0x600C0000
#define SYSTIMER_BASE_ADDRESS                           0x60023000
#define INTERRUPT_MATRIX_BASE_ADDRESS                   0x600C2000 

/* --------------------------------------------------------------------------
 *   ACCESS MACROS
 *   Provides compatibility between C and Assembly source files.
 * -------------------------------------------------------------------------- */
#ifdef __ASSEMBLER__
    #define REG_ADDR(addr)                              (addr)
#else
    #define REG_ADDR(addr)                              (*(volatile uint32_t *)(addr))
#endif


/* --------------------------------------------------------------------------
 *   PERIPHEREAL CONTROL REGISTERS
 * -------------------------------------------------------------------------- */
#define SYSTEM_PERIP_CLK_EN0_REG                        REG_ADDR(SYSTEM_BASE_ADDRESS + 0x0010)
#define SYSTEM_PERIP_RST_EN0_REG                        REG_ADDR(SYSTEM_BASE_ADDRESS + 0x0018)


/* --------------------------------------------------------------------------
 *   SYSTIMER REGISTERS
 * -------------------------------------------------------------------------- */
#define SYSTIMER_CONF_REG                               REG_ADDR(SYSTIMER_BASE_ADDRESS)
#define SYSTIMER_UNIT0_LOAD_HI_REG                      REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x000C)
#define SYSTIMER_UNIT0_LOAD_LO_REG                      REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x0010)
#define SYSTIMER_TIMER_TARGET0_LO_REG                   REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x0020) 
#define SYSTIMER_TIMER_TARGET0_HI_REG                   REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x001C)
#define SYSTIMER_TARGET0_CONF_REG                       REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x0034)
#define SYSTIMER_UNIT0_OP_REG                           REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x0004)
#define SYSTIMER_COMP0_LOAD_REG                         REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x0050)
#define SYSTIMER_UNIT0_LOAD_REG                         REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x005C)
#define SYSTIMER_INT_ENA_REG                            REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x0064)
#define SYSTIMER_INT_CLR_REG                            REG_ADDR(SYSTIMER_BASE_ADDRESS + 0x006C)

/* SYSTIMER Configuration Bit Masks */
#define SYSTEM_SYSTIMER_CLK_EN_RST (1U << 29)           //SYSTEM_PERIP_CLK_EN0_REG to enable the clock 
#define SYSTIMER_TIMER_UNIT0_UPDATE (1U << 30)          //SYSTIMER_UNIT0_OP_REG setup for periodic timer
#define SYSTIMER_UNIT0_SEL (1U << 30)                   // SYSTIMER_TARGET0_CONF_REG UNIT0-1 config

//SYSTIMER_CONF_REG setup for periodic timer
#define SYSTIMER_TIMER_UNIT0_WORK_EN (1U << 30)
#define SYSTIMER_TIMER_UNIT0_CORE0_STALL_EN (1U << 28)
#define SYSTIMER_TARGET0_WORK_EN (1U << 24)
#define SYSTIMER_TARGET0_PERIOD_MODE (1U << 30) 

/* --------------------------------------------------------------------------
 *   INTERRUPT MATRIX REGISTERS
 * -------------------------------------------------------------------------- */
#define INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG    REG_ADDR(INTERRUPT_MATRIX_BASE_ADDRESS+ 0x0094)
#define INTERRUPT_CORE0_CPU_INT_PRI_1_REG               REG_ADDR(INTERRUPT_MATRIX_BASE_ADDRESS+ 0x0118)
#define INTERRUPT_CORE0_CPU_INT_ENABLE_REG              REG_ADDR(INTERRUPT_MATRIX_BASE_ADDRESS + 0x0104)
#define SYSTIMER_CPU_LINE                               1

/*CPU line configuration*/
#define SYSTIMER_CPU_LINE_MASK (1U << 1)

#endif
