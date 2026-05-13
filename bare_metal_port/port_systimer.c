#include <stdint.h>
#include "port_systimer.h"
#include "port_systimer_regs.h"
#include "FreeRTOSConfig.h"

/**
 * @brief Configures the SYSTIMER as the main Tick Source for FreeRTOS.
 *
 * Since the ESP32-C3 RISC-V core lacks a standard MTIMER, this function
 * implements the tick source using the internal SYSTIMER peripheral.
 * 
 * The initialization follows two main steps:
 * 1. SYSTIMER: Configured as a periodic timer (Unit 0, Target 0).
 * 2. Interrupt Matrix: Maps the SYSTIMER interrupt to CPU Line 1 with priority 2.
 *
 * @note This implementation is hardware-specific (ESP32-C3 TRM) and bypasses 
 *       any HAL/IDF abstraction layers.
 * 
 * @pre Global interrupts should be handled carefully as this function 
 *      temporarily manipulates mstatus.
 */

void portSetupSystimerInterrupt( void )
{ 
    /* --- 1. SYSTIMER PERIODIC TIMER SETUP --- */
    
    // Reset and Enable Clock for SYSTIMER
    SYSTEM_PERIP_CLK_EN0_REG |= SYSTEM_SYSTIMER_CLK_EN_RST;
    SYSTEM_PERIP_RST_EN0_REG |= SYSTEM_SYSTIMER_CLK_EN_RST;
    SYSTEM_PERIP_RST_EN0_REG &= ~(SYSTEM_SYSTIMER_CLK_EN_RST);
    
    // Configure UNIT0: Stall on CPU0 halt, enable counter
    SYSTIMER_CONF_REG &= ~SYSTIMER_TARGET0_WORK_EN;
    SYSTIMER_CONF_REG |= SYSTIMER_TIMER_UNIT0_WORK_EN | SYSTIMER_TIMER_UNIT0_CORE0_STALL_EN;
    
    // Reset UNIT0 counter value
    SYSTIMER_UNIT0_LOAD_LO_REG = 0;
    SYSTIMER_UNIT0_LOAD_HI_REG = 0;
    SYSTIMER_UNIT0_LOAD_REG = 1;

    // Target 0: Clear bit 31 to use UNIT0 as source (as per TRM)
    SYSTIMER_TARGET0_CONF_REG &= ~(SYSTIMER_UNIT0_SEL);

    // Calculate and set the alarm period (Ticks = Fclk / configTICK_RATE_HZ)
    uint32_t period = 40000000/configTICK_RATE_HZ;
    SYSTIMER_TARGET0_CONF_REG &= ~(0x03FFFFFF);
    SYSTIMER_TARGET0_CONF_REG |= (period & 0x03FFFFFF);
    
    SYSTIMER_COMP0_LOAD_REG = 1;  // Apply configuration
    SYSTIMER_TARGET0_CONF_REG |= SYSTIMER_TARGET0_PERIOD_MODE;   
    SYSTIMER_CONF_REG |= SYSTIMER_TARGET0_WORK_EN;  // Start comparing
    
    SYSTIMER_INT_CLR_REG = 1; // Clear pending interrupts
    SYSTIMER_INT_ENA_REG = 1; // Enable SYSTIMER interrupt output

    
    /* --- 2. INTERRUPT MATRIX CONFIGURATION --- */ 
    
    // Disable interrupts globally before remapping (Clear MIE)
    __asm__ volatile ("csrci mstatus, 8");

    // Route SYSTIMER_TARGET0 signal to CPU Interrupt Line 1
    INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG = SYSTIMER_CPU_LINE;
    
    // Set priority for the selected CPU interrupt line
    INTERRUPT_CORE0_CPU_INT_PRI_1_REG = SYSTIMER_PRIORITY;
    
    // Unmask the interrupt line at the CPU level
    INTERRUPT_CORE0_CPU_INT_ENABLE_REG |= (SYSTIMER_CPU_LINE_MASK);
    
    __asm__ volatile ("fence");
    
    // Re-enable interrupts (Set MIE bit in mstatus)
    __asm volatile ( "csrs mstatus, %0" :: "r"(1 << 3) );
}

