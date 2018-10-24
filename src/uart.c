#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdio.h>

#include "uart.h"
#include "timers.h"

#ifndef __AVR_ATmega328P__
#error
#endif

static uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
static uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];

static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;

static uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;

static volatile uint16_t rx_lost = 0;
static volatile uint16_t rx_overrun = 0;

int uart_putchar(char c, FILE *stream) {

  if (c == '\n') uart_putchar('\r', stream);

  while (((tx_head + 1) & UART_TX_INDEX_MASK) == tx_tail) {
    // full
  }

  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    uart_tx_buffer[tx_head] = c;
    tx_head++;
    tx_head &= UART_TX_INDEX_MASK;
    // make sure interrupt enabled
    UCSR0B |= (1 << UDRIE0);
  }

  return 0;
}

int uart_getchar(FILE *stream) {
  int c;
  uint16_t timeout_tick_ms = ticks_ms() + UART_TIMEOUT_MS;

  while (rx_head == rx_tail) {
    if (ticks_ms() > timeout_tick_ms) {
      break;
    }
  }

  if (rx_head != rx_tail) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
      c = uart_rx_buffer[rx_tail];
      rx_tail++;
      rx_tail &= UART_RX_INDEX_MASK;
    }
  } else {
    c = _FDEV_EOF;
  }

  return c;
}

void uart0_init(void) {
#include <util/setbaud.h>
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A = (1 << U2X0);
#else
  UCSR0A = 0
#endif
  UCSR0C = (1 << USBS0) | (3 << UCSZ00); // 8 data bits, 1 stop bits, no parity
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

  (void)fdevopen(uart_putchar, uart_getchar);
}

ISR(USART_RX_vect)
{
  if ( bit_is_set(UCSR0B, DOR0) ) {
    rx_overrun++;
  }
  uart_rx_buffer[rx_head] = UDR0;
  rx_head++;
  rx_head &= UART_RX_INDEX_MASK;
  // overwrite oldest byte
  if (rx_head == rx_tail) {
    rx_tail++;
    rx_tail &= UART_RX_INDEX_MASK;
    rx_lost++;
  }
}

/*
ISR(USART_TX_vect)
{

}
*/

ISR(USART_UDRE_vect)
{
  if (tx_head != tx_tail) {
    UDR0 = uart_tx_buffer[tx_tail];
    tx_tail++;
    tx_tail &= UART_TX_INDEX_MASK;
  } else {
    // buffer empty - disable interrupt
    UCSR0B &= ~(1 << UDRIE0);
  }
}
