#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "timers.h"
#include "inputs.h"

//#define PWM_MIN    800
#define PWM_MIN    955
//#define PWM_MAX    2400
#define PWM_MAX    1740
#define RPM_LIMIT  17000
#define DWELL_TIME_MS 2000

#define INJ_TIME_US 2500


#define CLAMP(v, min, max)\
  v = (v > max) ? max : ((v < min) ? min : v );

inline uint16_t clamp_pwm(uint16_t v)
{
  CLAMP(v, PWM_MIN, PWM_MAX);
  return v;
}


int main(void)
{
  uart0_init();

  setup_timers();

  setup_inputs();
  
  printf("EMU ECU\n");
 
  sei(); // Enable Global Interrupt

  uint16_t engine_stop_ms = ticks_ms();

  uint16_t loop_ms = 0;
  while (1)
  {
    uint16_t curr_rpm = rpm();
    if (!curr_rpm)
    {
      // keep the stop time sliding for when we attempt to start again
      if ((ticks_ms() - engine_stop_ms) > DWELL_TIME_MS)
      {
        engine_stop_ms = ticks_ms() - DWELL_TIME_MS;
      }
    }
    if (!ignition_enabled())
    {
      if (curr_rpm > 0 && (ticks_ms() - engine_stop_ms) > DWELL_TIME_MS)
      {
        ignition_enable();
        pump_enable();
        set_injection_us(INJ_TIME_US);
        printf("engine start\n");
      }
    }
    else
    {
      if (curr_rpm > RPM_LIMIT)
      {
        printf("overrev - forced engine stop\n");
        ignition_disable();
        pump_disable();
        set_injection_us(0);
        engine_stop_ms = ticks_ms();
      }
      if (!curr_rpm)
      {
        printf("engine has stopped\n");
        ignition_disable();
        pump_disable();
        set_injection_us(0);
      }
    }

    uint16_t pwm_in = pwm_input();
    uint16_t pwm_out = clamp_pwm(pwm_in);
    set_pwm(0, pwm_out);

    uint16_t ms = ticks_ms();
    uint32_t us = ticks_us();

    if ((ms - loop_ms) >= 1000)
    {
      loop_ms += 1000;
      start_adc();
      int16_t a = analogue(0);

      printf("pwm_in=%u pwm_out=%u us=%lu rpm=%u a0=%d\n", pwm_in, pwm_out, us, rpm(), a);
    }
    //sleep(1000);
    //_delay_ms(1000);
  }  
}

    
