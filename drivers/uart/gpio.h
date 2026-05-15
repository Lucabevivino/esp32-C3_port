#include <stdint.h>

/**
 * @brief IO_MUX Register Layout Structure.
 * 
 * This structure represents the IO_MUX peripheral register map for the ESP32-C3.
 * Each member corresponds to a pin configuration register, which controls 
 * function selection, pull-up/down resistors, and drive strength.
 * 
 * @note The registers are mapped consecutively in memory starting from the base address.
 *       Reference: ESP32-C3 Technical Reference Manual, IO_MUX Section.
 */
typedef struct {
    volatile uint32_t gpio_0;
    volatile uint32_t gpio_1;
    volatile uint32_t gpio_2;
    volatile uint32_t gpio_3;
    volatile uint32_t gpio_4;
    volatile uint32_t gpio_5;
    volatile uint32_t gpio_6;
    volatile uint32_t gpio_7;
    volatile uint32_t gpio_8;
    volatile uint32_t gpio_9;
    volatile uint32_t gpio_10;
    volatile uint32_t gpio_11;
    volatile uint32_t gpio_12;
    volatile uint32_t gpio_13;
    volatile uint32_t gpio_14;
    volatile uint32_t gpio_15;
    volatile uint32_t gpio_16;
    volatile uint32_t gpio_17;
    volatile uint32_t gpio_18;
    volatile uint32_t gpio_19;
    volatile uint32_t gpio_20;
    volatile uint32_t gpio_21;
} io_mux_reg_t;

/* --------------------------------------------------------------------------  
 *        BASE ADDRESS
 * -------------------------------------------------------------------------- */

#define IO_MUX_BASE_ADDRESS 0x60009000
#define IO_MUX (*(volatile io_mux_reg_t *)IO_MUX_BASE_ADDRESS)


/* --------------------------------------------------------------------------  
 *        CONFIGURATION BITS
 * -------------------------------------------------------------------------- */

/* --- IO_MUX_GPIO20_REG  ---*/
#define IO_MUX_GPIO20_MCU_SEL                        12
#define IO_MUX_GPIO20_FUN_IE                         9
#define IO_MUX_GPIO20_FUN_IE                         9

/* --- IO_MUX_GPIO21_REG ---*/
#define IO_MUX_GPIO21_MCU_SEL                        12

