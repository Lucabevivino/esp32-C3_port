#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configcpu_clock_hz            ( 160000000ul )   /* o il clock reale che impostate */
#define configTICK_RATE_HZ            ( ( TickType_t ) 1000 )
#define configMTIME_BASE_ADDRESS      0
#define configMTIMECMP_BASE_ADDRESS   0
#define configMINIMAL_STACK_SIZE      100
#define configTOTAL_HEAP_SIZE                 ( ( size_t ) ( 40 * 1024 ) )
#define configMAX_PRIORITIES          5
#define configUSE_IDLE_HOOK           0
#define configUSE_TICK_HOOK           0
#define INCLUDE_vTaskDelay            1

#define configUSE_PREEMPTION          1
#define configUSE_TIME_SLICING        1
#define configUSE_16_BIT_TICKS        0
#define configCHECK_FOR_STACK_OVERFLOW 0 // method 2 it's slower but does catch every stack overflow (not as method 1)
#define configUSE_MALLOC_FAILED_HOOK  0 // enables a callback function for failed mallocs (usefull in DEBUG)

#endif
