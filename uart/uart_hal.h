#include <stdint.h>

typedef enum{
    UART_0 = 0,
    UART_1
}uart_port_t;

typedef enum {
    UART_SCLK_APB = 0x01,     // 80 MHz
    UART_SCLK_XTAL =0x02,    // 40 MHz
    UART_SCLK_RTC = 0x03      // RC_FAST (circa 17.5 MHz su C3)
} uart_sclk_t;

typedef struct{
    uint32_t src_clock;
    uint32_t baud_rate;
    uint32_t   parity_check; //None,Odd ,Even
    uint32_t  port;
    uint32_t data;
    uint32_t stop;
}uart_config_t;

typedef struct{
    uint32_t integral;
    uint32_t frag;    
}divisor_t;

/**
 * @brief UART Register Structure for ESP32-C3
 * Mappatura COMPLETA e INTEGRALE (0x0000 - 0x0088)
 * Verificata su Technical Reference Manual v1.4
 */
typedef struct {
    volatile uint32_t fifo;                // 0x0000: FIFO data register
    volatile uint32_t int_raw;             // 0x0004: Raw interrupt status
    volatile uint32_t int_st;              // 0x0008: Masked interrupt status
    volatile uint32_t int_ena;             // 0x000C: Interrupt enable bits
    volatile uint32_t int_clr;             // 0x0010: Interrupt clear bits
    volatile uint32_t clkdiv;              // 0x0014: Clock divider configuration
    volatile uint32_t rx_filt;             // 0x0018: RX filter configuration
    volatile uint32_t status;              // 0x001C: UART status register
    volatile uint32_t conf0;               // 0x0020: Configuration register 0
    volatile uint32_t conf1;               // 0x0024: Configuration register 1
    volatile uint32_t lowpulse;            // 0x0028: Autobaud minimum low pulse duration
    volatile uint32_t highpulse;           // 0x002C: Autobaud minimum high pulse duration
    volatile uint32_t rxd_cnt;             // 0x0030: Autobaud edge change count
    volatile uint32_t flow_conf;           // 0x0034: Software flow control configuration
    volatile uint32_t sleep_conf;          // 0x0038: Sleep mode configuration
    volatile uint32_t swfc_conf0;          // 0x003C: Software flow control char (XOFF)
    volatile uint32_t swfc_conf1;          // 0x0040: Software flow control char (XON)
    volatile uint32_t txbrk_conf;          // 0x0044: TX break character configuration
    volatile uint32_t idle_conf;           // 0x0048: Frame end idle time configuration
    volatile uint32_t rs485_conf;          // 0x004C: RS485 mode configuration
    volatile uint32_t at_cmd_precnt;       // 0x0050: Pre-sequence timing configuration
    volatile uint32_t at_cmd_postcnt;      // 0x0054: Post-sequence timing configuration
    volatile uint32_t at_cmd_gaptout;      // 0x0058: Gap timeout configuration
    volatile uint32_t at_cmd_char;         // 0x005C: AT escape character configuration
    volatile uint32_t mem_conf;            // 0x0060: UART memory power configuration
    volatile uint32_t mem_tx_status;       // 0x0064: TX FIFO write/read pointer status
    volatile uint32_t mem_rx_status;       // 0x0068: RX FIFO write/read pointer status
    volatile uint32_t fsm_status;          // 0x006C: UART finite state machine status
    volatile uint32_t pospulse;            // 0x0070: Autobaud high pulse register
    volatile uint32_t negpulse;            // 0x0074: Autobaud low pulse register
    volatile uint32_t clk_conf;            // 0x0078: UART core clock configuration
    volatile uint32_t date;                // 0x007C: UART version date register
    volatile uint32_t uart_id;             // 0x0080: Async FIFO status register
} uart_reg_t;

#define SYSTEM_PERIP_CLK_EN0_REG (*(volatile uint32_t*)(0x600C0000 + 0x0010))
#define SYSTEM_PERIP_RST_EN0_REG (*(volatile uint32_t*)(0x600C0000 + 0x0018))
#define UART_REG (*(volatile uart_reg_t*)( 0x60000000 ))


void hal_uart_init(uart_config_t *config);
void hal_gpio_uart_setup(void);
void hal_uart_write_byte(uint8_t byte);
uint8_t hal_uart_read_byte(uint8_t *data);

