#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

void vPortSetupTimerInterrupt( void )
{   
    //enable clock for SYSTIMER module
    SYSTEM_PERIP_CLK_EN0_REG |= SYSTEM_SYSTIMER_CLK_EN;
   
    //set SYSTIMER_TIMER_UNIT0_UPDATE to update the current count value
    SYSTIMER_UNIT0_OP_REG |= SYSTIMER_TIMER_UNIT0_UPDATE;
    //set EN bits for SYSTIMER_CONF_REG
    SYSTIMER_CONF_REG |= SYSTIMER_TIMER_UNIT0_WORK_EN |SYSTIMER_TIMER_UNIT0_CORE0_STALL_EN | SYSTIMER_TARGET0_WORK_EN;

    //set periodic timer 
    SYSTIMER_TARGET0_CONF_REG |= SYSTIMER_TARGET0_PERIOD_MODE;
    uint32_t period = 16000000/configTICK_RATE_HZ;
    SYSTIMER_TIMER_TARGET0_LO_REG = period;
    SYSTIMER_TIMER_TARGET0_HI_REG = 0;

    //COMP0 synch
    SYSTIMER_COMP0_LOAD_REG = 1;
    
    //interrupt enable 
    SYSTIMER_INT_ENA_REG = 1;
}

