#include "config.h"

#ifndef F_CPU
#error F_CPU must be defined earlier
#endif

#define BAUD		    (115200L)
#define BAUD_TOL            (3)

// 100 character timeout
#define UART_TIMEOUT_MS     ((100L*1000L)/(BAUD/10L))

#define UART_TX_BUFFER_SIZE (1<<4)
#define UART_TX_INDEX_MASK (UART_TX_BUFFER_SIZE-1)
#define UART_RX_BUFFER_SIZE (1<<5)
#define UART_RX_INDEX_MASK (UART_RX_BUFFER_SIZE-1)

void uart0_init(void);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);


