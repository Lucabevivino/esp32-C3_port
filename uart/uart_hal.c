#include <uart_hal.h>
#include <gpio.h>
#include <uart_pins.h>

uint32_t get_clk_freq(uart_sclk_t sclk) {
    switch (sclk) {
        case UART_SCLK_APB:  return 80000000;
        case UART_SCLK_XTAL: return 40000000;
        case UART_SCLK_RTC:  return 17500000; // Valore indicativo
        default:             return 80000000;
    }
}

divisor_t get_clk_div(uint32_t freq, uint32_t baudrate){ 
    uint32_t div_val = (uint32_t)((((uint64_t)freq << 4) + (baudrate / 2)) / baudrate);
    divisor_t divisor;
    divisor.integral = div_val >> 4;
    divisor.frag = div_val & 0xF;
    return divisor;
}

void hal_uart_init(uart_config_t *config){
    
    // INITIALISATION
    SYSTEM_PERIP_CLK_EN0_REG |= (1 << 24);
    
    if(config->port){
        SYSTEM_PERIP_CLK_EN0_REG |= (1 << 5);
        SYSTEM_PERIP_RST_EN0_REG &= ~(1 << 5);
    }else{
        SYSTEM_PERIP_CLK_EN0_REG |= (1 << 2);
        SYSTEM_PERIP_RST_EN0_REG &= ~(1 << 2);
    }
    
    UART_REG.clk_conf |= (1 << 22);
    UART_REG.clk_conf |= (1 << 23);

    if(config->port){
        SYSTEM_PERIP_RST_EN0_REG |= (1 << 5);
        SYSTEM_PERIP_RST_EN0_REG &= ~(1 << 5);
    }else{
        SYSTEM_PERIP_RST_EN0_REG |= (1 << 2);
        SYSTEM_PERIP_RST_EN0_REG &= ~(1 << 2);
    }
    
    UART_REG.clk_conf &= ~(1 << 23);
    
    // CONFIGURATION
    // Azzera i bit 20 e 21, poi imposta la sorgente shiftata di 20
    UART_REG.clk_conf = (UART_REG.clk_conf & ~(0x3 << 20))|((config->src_clock & 0x3) << 20); 
    
    uint32_t freq = get_clk_freq(config->src_clock);
    
    divisor_t divisor = get_clk_div(freq, config->baud_rate);
    
    UART_REG.clkdiv |= ((divisor.integral & 0xFFF) | ((divisor.frag & 0xF) << 20));
    UART_REG.conf0 |= (0x3 << 2);
    

    // 6. RESET FIFO (Indispensabile per iniziare puliti)
    UART_REG.conf0 |= (1 << 17) | (1 << 18); // TX_RST e RX_RST
    UART_REG.conf0 &= ~((1 << 17) | (1 << 18));
    
    UART_REG.conf0 |= (1 << 1) | (1 <<  0);

    UART_REG.uart_id |= (1 << 31);

    // TX-RX ENABLING
    UART_REG.conf1 |= ((10 & 0x7F) << 8);
    UART_REG.conf1 |= ((10 & 0x7F) << 0);
    
    // 2. Trigger update
    UART_REG.uart_id &= ~(1 << 30);
    UART_REG.uart_id |= (1 << 31);

    // 3. Wait for completion
    while (UART_REG.uart_id & (1 << 31)) {} 
 
    return;    
}

void hal_gpio_uart_setup() {
    //Enable GPIO clock
     SYSTEM_PERIP_CLK_EN0_REG |= (1 << 1); // GPIO clock

    // 2. Configure TX (GPIO 21)
    GPIO_FUNC_OUT_SEL_21_REG = U0TXD_OUT_IDX;

    // 3. Configure RX (GPIO 20)
    // U0RXD_IN_SEL_REG (0x0018) -> Connect GPIO 20
    GPIO_FUNC_IN_SEL_U0RX_REG = (UART0_RX_PIN & 0x3F);

    //GPIO config in IO_MUX
    IO_MUX.gpio[21] |= (1 << 12); // Function 1
    IO_MUX.gpio[20] |= (1 << 12) | (1 << 9); // Function 1 + Input Enable
}

void hal_uart_write_byte(uint8_t byte){
    
    while (((UART_REG.status >> 16) & 0xFF) >= 128) {}

    UART_REG.fifo = byte;    
}

uint8_t hal_uart_read_byte(uint8_t *data){
    
    uint8_t rx_count = (UART_REG.status & 0xFF);

    if(rx_count > 0){
        *data = (uint8_t)(UART_REG.fifo & 0xFF);
        return 1;
    }
    return 0;
}

/* void hal_uart_write_string(uint8_t *byte_string){
    uint8_t *curr_byte = byte_string;
    while((((UART_REG.status >> 16) & 0xFF) <= 128)||curr_byte != NULL){
        UART_REG.fifo = *curr_byte;
        curr_byte = byte_string + sizeof(uint8_t);
    }
}*/


