
//IO_MUX BASE ADDRESS
#define DR_REG_IO_MUX_BASE 0x60009000

typedef struct {
    volatile uint32_t gpio[22]; // L'ESP32-C3 ha 22 GPIO (0-21)
} io_mux_dev_t;

//Pointer
#define IO_MUX (*(volatile io_mux_dev_t *)DR_REG_IO_MUX_BASE)

//GPIO BASE ADDRESS
#define DR_REG_GPIO_BASE 0x60004000

// Offset 0x0010 + (n_gpio * 4)
#define GPIO_FUNC_OUT_SEL_21_REG (*(volatile uint32_t*)(DR_REG_GPIO_BASE + 0x0010 + 21 * 4))
// Offset 0x0054 + (index_sig * 4)
#define GPIO_FUNC_IN_SEL_U0RX_REG (*(volatile uint32_t*)(DR_REG_GPIO_BASE + 0x0054 + 6 * 4))


