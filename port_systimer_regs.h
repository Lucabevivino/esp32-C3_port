#ifndef REGS_SYSTIMER_H
#define REGS_SYSTIMER_H

#include <stdint.h>

/*==========================================
             Base Addresses 
  ==========================================*/

#define SYSTEM_BASE_ADDRESS 0x600C0000
#define SYSTIMER_BASE_ADDRESS 0x60023000
#define INTERRUPT_MATRIX_BASE_ADDRESS 0x600C2000 

/*==========================================
         Periphereal Control Registers 
  ==========================================*/

#define SYSTEM_PERIP_CLK_EN0_REG (*(volatile uint32_t *)(SYSTEM_BASE_ADDRESS + 0x0010))

/*========================================== 
             SYSTIMER Registers
  ==========================================*/

#define SYSTIMER_CONF_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS))
#define SYSTIMER_TIMER_TARGET0_LO_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x001C)) 
#define SYSTIMER_TIMER_TARGET0_HI_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x0020))
#define SYSTIMER_TARGET0_CONF_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x0034))
#define SYSTIMER_UNIT0_OP_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x0004))
#define SYSTIMER_COMP0_LOAD_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x0050))
#define SYSTIMER_INT_ENA_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x0064))

#ifdef __ASSEMBLER__
    #define SYSTIMER_INT_CLR_REG (SYSTIMER_BASE_ADDRESS + 0x006C) //#define for the .macro CLEAR_SYSTIMER_INTERRUPT in port_asm.S
#else
    #define SYSTIMER_INT_CLR_REG (*(volatile uint32_t *)(SYSTIMER_BASE_ADDRESS + 0x006C))

/*==========================================
          Interrupt Matrix Registers
  ==========================================*/

#define INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG (*(volatile uint32_t *)(INTERRUPT_MATRIX_BASE_ADDRESS+ 0x0094))
#define INTERRUPT_CORE0_CPU_INT_PRI_1_REG  (*(volatile uint32_t *)(INTERRUPT_MATRIX_BASE_ADDRESS+ 0x0118+(0x4⋅1)))

/*==========================================
                 Bit masks          
  ==========================================*/

//SYSTEM_PERIP_CLK_EN0_REG to enable the clock 
#define SYSTEM_SYSTIMER_CLK_EN (1U << 29)

//SYSTIMER_UNIT0_OP_REG setup for periodic timer
#define SYSTIMER_TIMER_UNIT0_UPDATE (1U << 30)

//SYSTIMER_CONF_REG setup forn periodic timer
#define SYSTIMER_TIMER_UNIT0_WORK_EN (1U << 30)
#define SYSTIMER_TIMER_UNIT0_CORE0_STALL_EN (1U << 28)
#define SYSTIMER_TARGET0_WORK_EN (1U << 24)

//SYSTIMER_TARGET0_COMF_REG setup for periodic timer
#define SYSTIMER_TARGET0_PERIOD_MODE (1U << 30)
 
#endif
