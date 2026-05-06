// UART Base Address for Raspberry Pi 5
#define UART_BASE 0xfe201000

// UART Register Offsets
#define UART_DR 0x00
#define UART_FR 0x18
#define UART_IBRD 0x24
#define UART_FBRD 0x28
#define UART_LCRH 0x2c
#define UART_CR 0x30

// UART Flags
#define UART_FR_TXFF (1 << 5)
#define UART_FR_RXFE (1 << 4)
#define UART_LCRH_FIFO_EN (1 << 4)
#define UART_LCRH_WLEN (0x3 << 5)
#define UART_CR_EN (1 << 0)
#define UART_CR_TXE (1 << 8)
#define UART_CR_RXE (1 << 9)

typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

void mmio_write(uint64_t reg, uint32_t data) {
    *(volatile uint32_t *)reg = data;
}

uint32_t mmio_read(uint64_t reg) {
    return *(volatile uint32_t *)reg;
}

void uart_init(void) {
    // Disable UART
    mmio_write(UART_BASE + UART_CR, 0);
    
    // Set baud rate to 115200
    // UART clock is 48MHz
    mmio_write(UART_BASE + UART_IBRD, 26);
    mmio_write(UART_BASE + UART_FBRD, 3);
    
    // Set line control (8 bits, no parity, 1 stop bit, FIFO enabled)
    mmio_write(UART_BASE + UART_LCRH, UART_LCRH_WLEN | UART_LCRH_FIFO_EN);
    
    // Enable UART
    mmio_write(UART_BASE + UART_CR, UART_CR_EN | UART_CR_TXE | UART_CR_RXE);
}

void uart_putc(char c) {
    // Wait until transmit FIFO has space
    while (mmio_read(UART_BASE + UART_FR) & UART_FR_TXFF);
    
    // Write character to UART
    mmio_write(UART_BASE + UART_DR, (uint32_t)c);
}

void uart_puts(const char *str) {
    while (*str) {
        if (*str == '\n') {
            uart_putc('\r');
        }
        uart_putc(*str++);
    }
}

void main(void) {
    uart_init();
    uart_puts("Hello, World!\n");
    
    // Infinite loop
    while (1);
}