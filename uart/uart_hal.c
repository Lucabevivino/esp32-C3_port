#include <uart_hal.h>


uint32_t get_clk_freq(uart_sclk_t sclk) {
    switch (sclk) {
        case UART_SCLK_APB:  return 80000000;
        case UART_SCLK_XTAL: return 40000000;
        case UART_SCLK_RTC:  return 17500000; // Valore indicativo
        default:             return 80000000;
    }
}

divisor_t get_clk_div(uint32_t freq, uint32_t bauderate){ 
    uint32_t div_val = (uint32_t)((((uint64_t)freq << 4) + (baudrate / 2)) / baudrate);
    divisor_t divisor;
    divisor->integral = div_val >> 4;
    divisor->frag = div_val & 0xF;
    return divisor;
}

void hal_uart_init(uart_port_t *port, uart_config_t *config){
    
    // INITIALISATION
    SYSTEM_PERIP_CLK_EN0_REG = (1 << 24);
    
    if(config->port){
        SYSTEM_PERIP_CLK_EN0_REG = (1 << 5);
        UART_PERIP_RST_EN0_REG = (0 << 5);
    }
    
    else{
        SYSTEM_PERIP_CLK_EN0_REG = (1 << 2);
        SYSTEM_PERIP_RST_EN0_REG = (0 << 2);
    }

    UART_REG.clk_conf = (1 << 23);

    if(config->port){
        SYSTEM_PERIP_RST_EN0_REG = (1 << 5);
        SYSTEM_PERIP_RST_EN0_REG = (0 << 5);
    }
 
    else{
        SYSTEM_PERIP_RST_EN0_REG = (1 << 2);
        SYSTEM_PERIP_RST_EN0_REG = (0 << 2);
    }
    
    UART_REG.clk_conf |= (1 << 23);

    UART_REG.uart_id |= (0 << 30);
    
    // CONFIGURATION
    UART_REG.uart_id |= (1 << 31);
    
    while(UART_REG.uart_id & (1 << 31)){}

    UART0.clk_conf |= (config->src_clock & 0x3);
    
    uint32_t freq = get_clk_freq(config->src_clock);
    
    divisor_t divisor = get_clk_div(freq, config->baudrate);
    
    UART_REG.clkdiv |= ((divisor.integral & 0xFFF) | ((divisor.frag & 0xF) << 20));
    UART_REG.conf0 |= (0x3 << 2);
    
    // 6. RESET FIFO (Indispensabile per iniziare puliti)
    port->conf0 |= (1 << 17) | (1 << 18); // TX_RST e RX_RST
    port->conf0 &= ~((1 << 17) | (1 << 18));
    
    UART_REG.conf0 |= (1 << 1) | (1 <<  0);

    UART_REG.uart_id |= (1 << 31);

    // TX-RX ENABLING

    UART_REG.conf1 |= ((10 & 0x7F) << 8);
    UART_REG.conf1 |= ((10 & 0x7F) << 0);
    
    return;    
}

void hal_uart_write_byte(uint8_t byte){
    
    while (((UART_REG.status >> 16) & 0xFF) >= 128) {}

    UART_REG.fifo = data;    
}

unsigned int hal_uart_read_byte(uint8_t *data){
    
    uint8_t rx_count = (hw->status & 0xFF);

    if(rx_count > 0){
        *data_out = (uint8_t)(hw->fifo & 0xFF);
        return 1;
    }
    return 0;
}
