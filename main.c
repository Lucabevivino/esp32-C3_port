#include "FreeRTOS.h"
#include "task.h"
#include "uart_hal.h"

void uart_task(void *arg)
{
    uart_config_t *cfg = (uart_config_t *)arg;

    hal_gpio_uart_setup();
    hal_uart_init(cfg);

    while (1)
    {
        hal_uart_write_byte('A');
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{

    uart_config_t cfg;
    cfg.src_clock = UART_SCLK_APB;
    cfg.baud_rate = 115200;
    cfg.parity_check = 0;
    cfg.port = UART_0;
    cfg.data = 0;
    cfg.stop = 0;
   
    ((void (*)(int)) 0x40000588)(160);

    xTaskCreate(
        uart_task,     // function
        "uart_task",   // name
        2048,          // stack size (IMPORTANT!)
        &cfg,          // argument
        1,             // priority
        NULL           // handle
    );

    vTaskStartScheduler();

    // Should NEVER reach here
    while (1) {}
}
