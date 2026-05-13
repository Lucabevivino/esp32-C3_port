#include <stdint.h>
#include "port_systimer.h"
#include "port_systimer_regs.h"

void portSetupSystimerInterrupt( void )
{ 
    //=== SYSTIMER PERIODIC TIMER SETUP ===

    // Enable clock for SYSTIMER module
    SYSTEM_PERIP_CLK_EN0_REG |= SYSTEM_SYSTIMER_CLK_EN_RST;
    SYSTEM_PERIP_RST_EN0_REG |= SYSTEM_SYSTIMER_CLK_EN_RST;
    SYSTEM_PERIP_RST_EN0_REG &= ~(SYSTEM_SYSTIMER_CLK_EN_RST);
    
    // Config and enable counter UNIT0
    SYSTIMER_CONF_REG &= ~SYSTIMER_TARGET0_WORK_EN;
    SYSTIMER_CONF_REG |= SYSTIMER_TIMER_UNIT0_WORK_EN | SYSTIMER_TIMER_UNIT0_CORE0_STALL_EN;
    
    // Config UNIT0 startig count value
    SYSTIMER_UNIT0_LOAD_LO_REG = 0;
    SYSTIMER_UNIT0_LOAD_HI_REG = 0;
    SYSTIMER_UNIT0_LOAD_REG = 1;

    // Config TARGET UNIT(0)
    SYSTIMER_TARGET0_CONF_REG &= ~(1 << 31);

    // Set period
    uint32_t period = 40000000/configTICK_RATE_HZ;
    SYSTIMER_TARGET0_CONF_REG &= ~(0x03FFFFFF);
    SYSTIMER_TARGET0_CONF_REG |= (period & 0x03FFFFFF);
    
    // Synch alarm period
    SYSTIMER_COMP0_LOAD_REG = 1;
    
    // Set period mode
    SYSTIMER_TARGET0_CONF_REG |= SYSTIMER_TARGET0_PERIOD_MODE; 
    
    // Enable to start comparing
    SYSTIMER_CONF_REG |= SYSTIMER_TARGET0_WORK_EN;
    
    // Clear interrupt register
    SYSTIMER_INT_CLR_REG = 1;
    
    // Interrupt enable 
    SYSTIMER_INT_ENA_REG = 1;

    
    //=== INTERRUPT SETUP IN INTERRUPT MATRIX ==
    
    __asm__ volatile ("csrci mstatus, 8");

    // Set the CPU line for the interrupt
    INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG = 1;
    
    // Set the priority
    INTERRUPT_CORE0_CPU_INT_PRI_1_REG = 3;
    
    INTERRUPT_CORE0_CPU_INT_ENABLE_REG |= (1 << 1);
    
    __asm__ volatile ("fence");
    
    // Set the line 1 bit to enable the interrupt on line 1
    __asm volatile ( "csrs mstatus, %0" :: "r"(1 << 3) );
}

