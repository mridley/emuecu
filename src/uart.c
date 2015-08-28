#include <avr/io.h>
#include <stdio.h>

#include "uart.h"

#ifndef __AVR_ATmega328P__
#error
#endif

int uart_putchar(char c, FILE *stream) {

  if (c == '\n') uart_putchar('\r', stream);
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;

  return 0;
}


void uart0_init(void) {
  unsigned char baudrateDiv;

  baudrateDiv = (unsigned char)((F_CPU+(UART_BAUD_RATE*8L))/(UART_BAUD_RATE*16L)-1);
  	
  UBRR0H = baudrateDiv >> 8;	
  UBRR0L = baudrateDiv;
	
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
 
  fdevopen(uart_putchar, NULL);
  //printf("\n\nuart0_init();\n");
}

