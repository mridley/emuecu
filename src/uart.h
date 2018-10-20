#include "config.h"

#ifndef F_CPU
#error F_CPU must be defined earlier
#endif

#define UART_BAUD_RATE		115200      	// 38400 baud

void uart0_init(void);
int uart_putchar(char c, FILE *stream);

