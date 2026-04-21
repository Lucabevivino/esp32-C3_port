#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configcpu_clock_hz            ( 160000000ul )   /* o il clock reale che impostate */
#define configTICK_RATE_HZ            ( ( TickType_t ) 1000 )
#define configMTIME_BASE_ADDRESS      0
#define configMTIMECMP_BASE_ADDRESS   0
#define configISR_STACK_SIZE_WORDS    256
#define configUSE_PREEMPTION          1
#define configUSE_TIME_SLICING        1
#define configUSE_16_BIT_TICKS        0
#define configCHECK_FOR_STACK_OVERFLOW 2 // method 2 it's slower but does catch every stack overflow (not as method 1)
#define configUSE_MALLOC_FAILED_HOOK  1 // enables a callback function for failed mallocs (usefull in DEBUG)

#endif
