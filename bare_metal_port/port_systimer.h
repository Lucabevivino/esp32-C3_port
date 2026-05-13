#ifndef PORT_SYSTIMER_H
#define PORT_SYSTIMER_H

/**
 * @file port_systimer.h
 * @brief FreeRTOS Tick Timer setup for ESP32-C3 bare-metal port.
 *
 * This header declares the initialization functions for the SYSTIMER
 * peripheral when used as the system tick source in the absence of a
 * standard RISC-V mtimer.
 */

void portSetupSystimerInterrupt(void);

#endif
