#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "timers.h"
#include "inputs.h"

int main(void)
{
  uart0_init();

  setup_timers();

  setup_inputs();
  
  printf("EMU ECU\n");
 
  sei(); // Enable Global Interrupt

  while (1) {
  
    sleep(1000);
    //_delay_ms(1000);
    printf("pwm_val=%u ticks=%lu ticks_ms=%u\n", pwm_input(), ticks_us(), ticks_ms());
  }  
}

    
