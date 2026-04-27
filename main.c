#include "FreeRTOS.h"
#include "task.h"
#include "uart_hal.h"

void vUART_Echo_Task(void *pvParameters) {
    uint8_t rx_data;

    // Messaggio di avvio
    const char *welcome = "\r\n--- ESP32-C3 Bare Metal Echo Ready ---\r\n";
    while(*welcome) hal_uart_write_byte(*welcome++);

    for (;;) {
        // 1. Prova a leggere un byte
        if (hal_uart_read_byte(&rx_data)) {
            
            // 2. Opzionale: Logica di manipolazione (es. trasforma in maiuscolo)
            // if(rx_data >= 'a' && rx_data <= 'z') rx_data -= 32;

            // 3. Trasmetti il byte ricevuto (Echo)
            hal_uart_write_byte(rx_data);
            
            // 4. Se ricevi un 'invio' (\r), aggiungi un 'newline' (\n)
            if (rx_data == '\r') {
                hal_uart_write_byte('\n');
            }
        } else {
            // 5. IMPORTANTE: Se non c'è nulla, dormi per 1 tick.
            // Questo permette ad altri task di girare e impedisce al 
            // Watchdog di resettare il microcontrollore.
            vTaskDelay(1); 
        }
    }
}


int main(void) {
    // 1. Inizializzazione Hardware (Bare-Metal HAL)
    hal_gpio_uart_setup(); //
    
    uart_config_t my_config = {
        .src_clock = UART_SCLK_APB,
        .baud_rate = 115200,    // Nota: verifica lo spelling nel tuo .h
    };

    uart_port_t port = UART_0;
    
    // Inizializza la UART0 usando il puntatore alla struct mappata
    hal_uart_init(&port, &my_config);
    //TEST
    volatile uint32_t *uart_fifo = (volatile uint32_t *)0x60000000;
    *uart_fifo = 'O';
    *uart_fifo = 'K';
    *uart_fifo = '\n';

    // 2. Creazione del Task
    // Usiamo una priorità media (es. 2)
    xTaskCreate(
        vUART_Echo_Task,    // Funzione del task
        "UART_Echo",        // Nome descrittivo
        2048,               // Stack size (parole, non byte in RISC-V)
        NULL,               // Parametri
        2,                  // Priorità
        NULL                // Handle (non necessario ora)
    );

    // 3. Avvio dello Scheduler
    // Da qui in poi il controllo passa a FreeRTOS e il main "sparisce"
    vTaskStartScheduler();

    // Non dovrebbe mai arrivare qui
    for(;;);
    return 0;
}
