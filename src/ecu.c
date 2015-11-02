#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "timers.h"
#include "inputs.h"

#define PWM_MIN    800
#define PWM_MAX    2400
#define RPM_LIMIT  15000

#define CLAMP(v, min, max)\
  v = (v > max) ? max : ((v < min) ? min : v );

int main(void)
{
  uart0_init();

  setup_timers();

  setup_inputs();
  
  printf("EMU ECU\n");
 
  sei(); // Enable Global Interrupt

  uint16_t pwm_in = 0;
  uint16_t engine_stop_ms = ticks_ms();

  while (1)
  {
    uint16_t curr_rpm = rpm();
    if (!curr_rpm)
    {
      engine_stop_ms = ticks_ms();
    }
    if (!ignition_enabled())
    {
      if (curr_rpm > 0 && (ticks_ms() - engine_stop_ms) > 2048 )
      {
        ignition_enable();
        printf("engine start\n");
      }
    }
    else
    {
      if (curr_rpm > RPM_LIMIT)
      {
        printf("overrev - forced engine stop\n");
        ignition_disable();
        engine_stop_ms = ticks_ms();
      }
      if (!curr_rpm)
      {
        printf("engine stop\n");
        ignition_disable();
      }
    }

    uint16_t p = pwm_input();
    CLAMP(p, PWM_MIN, PWM_MAX);
    if (pwm_in != p)
    {
      pwm_in = p;
      set_pwm(0, pwm_in);
    }


    if (!(ticks_ms() & 0x3ff))
    {
      start_adc();
      int16_t a = analogue(0);

      printf("pwm_in=%u ticks_ms=%u rpm=%u a0=%d\n", pwm_in, ticks_ms(), rpm(), a);
    }
    //sleep(1000);
    //_delay_ms(1000);
  }  
}

    
