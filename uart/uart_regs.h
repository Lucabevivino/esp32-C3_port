#ifndef UART_REGS_H
#define UART_REGS_H
#endif

/*
 * ------------------------------------------
 *              uart registers
 * ------------------------------------------ 
 */

// uart configure register
#define UART_CLK_CONF_REG 
// timer divider
#define UART_CLKDIV_REG
// uart access register
#define UART_FIFO_REG
// uart status register -> TX FIFO and RX status
#define UART_STATUS_REGISTER

#define SYSTEM_PERIP_CLK_EN0_REG 
// registers config
#define UART_ID_REG 

#define UART_CONF0_REG

#define UART_CONF1_REG

#define UART_INT_CLR_REG
/*
 * ------------------------------------------
 *            uart setup masks
 * ------------------------------------------ 
 */

//UART_CLK_CONF_REG -> timer select 
#define  UART_SCLK_SEL
//UART_CLKDIV_REG -> Divider for timer frequency: integral part
#define UART_CLKDIV
//UART_CLKDIV_REG -> Divider for timer frequency: fractional part
#define UART_CLKDIV_FRAG
//UART_CLK_CONF_REG -> TX clock enable
#define UART_TX_SCLK_EN 
//UART_CLK_CONF_REG -> RX clock enable
#define UART_RX_SCLK_EN


/*
 * ------------------------------------------
 *          uart init/reset masks
 * ------------------------------------------ 
 */
//SYSTEM_PERIP_CLK_EN0_REG -> enable the clock for UART RAM by setting 
#define SYSTEM_UART_MEM_CLK_EN to 1;
//SYSTEM_PERIP_CLK_EN0_REG -> enable APB_CLK for UARTn  
#define SYSTEM_UART_CLK_EN to 1
//SYSTEM_PERIP_RST_EN0_REG -> clear reset bit 
#define SYSTEM_UART_RST to 0;
//UART_CLK_CONF_REG -> write (and then clear) to reset RX and TX 
#define UART_RST_CORE to 1;
//SYSTEM_PERIP_RST_EN0_REG -> write (and the clear) to reset from system perip control register
#define SYSTEM_UART_RST to 1;
//SYSTEM_PERIP_RST_EN0_REG -> clear 
#define SYSTEM_UART_RST to 0;
//UART_CLK_CONF_REG -> clear 
#define UART_RST_CORE to 0;
//UART_ID_REG -> register synch
#define UART_UPDATE_CTRL to 1

/*
 * ------------------------------------------
 *       uart communication config masks
 * ------------------------------------------
 */
//UART_ID_REG -> Set to 1 by software, synchronizes the registers to UART Core's clock domain
#define UART_REG_UPDATE 
//UART_CLK_CONF_REG -> configure divisor of the divider
#define UART_SCLK_DIV_NUM
//UART_CLK_CONF_REG -> configure divisor of the divider
#define  UART_SCLK_DIV_A
//UART_CLK_CONF_REG -> configure divisor of the divider
#define  UART_SCLK_DIV_B

//UART_CLKDIV_REG -> confifure baude rate
//#define UART_CLKDIV 
//UART_CLKDIV_REG -> confifure baude rate
//#define UART_CLKDIV_FRAG

//UART_CONF0_REG ->  set the length of data
#define UART_BIT_NUM
//UART_CONF0_REG -> configure parity check mode
#define UART_PARITY 
//UART_CONF0_REG -> enables UART parity check
#define UART_PARITY_EN

/*
 * ------------------------------------------
 *             uart-n  enable masks
 * ------------------------------------------
 */

//UART_CONF1_REG -> configure TX FIFO’s empty threshold via
#define UART_TXFIFO_EMPTY_THRHD
//UART_INT_ENA_REG -> disable UART_TXFIFO_EMPTY_INT interrupt by clearing 
#define UART_TXFIFO_EMPTY_INT_ENA
//UART_FIFO_REG -> write data to be sent to 
#define UART_RXFIFO_RD_BYTE
//UART_INT_CLR_REG -> UART_TXFIFO_EMPTY_INT interrupt by setting 
#define UART_TXFIFO_EMPTY_INT_CLR

//enable UART_TXFIFO_EMPTY_INT interrupt by setting 

//detect UART_TXFIFO_EMPTY_INT and wait for the completion of data transmission.

//UART_CONF1_REG -> configure RX FIFO’s full threshold via 
#define UART_RXFIFO_FULL_THRHD
//UART_INT_ENA_REG -> enable UART_RXFIFO_FULL_INT interrupt by setting 
#define UART_RXFIFO_FULL_INT_ENA

//detect UART_TXFIFO_FULL_INT and wait until the RX FIFO is full;

//UART_STATUS_REG -> read data from RX FIFO via UART_RXFIFO_RD_BYTE, and obtain the number of bytes received in RX FIFO
#define UART_RXFIFO_CNT
/*
 * ------------------------------------------
 *              FIFO management masks
 * ------------------------------------------
 */

//UART_CONF0_REG -> reset TX FIFO
#define UART_TXFIFO_RST to 0;
//UART_CONF0_REG -> reset RX FIFO
#define UART_RXFIFO_RST to 0;
