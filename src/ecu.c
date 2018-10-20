#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#include "config.h"
#include "ecu.h"
#include "uart.h"
#include "timers.h"
#include "inputs.h"
#include "injection.h"
#include "bme280.h"
#include "max6675.h"


#define CLAMP(v, min, max)\
  v = (v > max) ? max : ((v < min) ? min : v );

inline uint16_t clamp_pwm(uint16_t v)
{
  if (v > PWM_LIMIT)
  {
    v = 0U;
  }
  CLAMP(v, config.pwm_min, config.pwm_max);
  return v;
}


int main(void)
{
  uart0_init();

  config_load();
  config_dump();
  //config_save();

  setup_timers();

  setup_inputs();
  printf("EMU ECU\n");
 
  sei(); // Enable Global Interrupt

  printf("bme id (0x%02x)\n", bme_probe());
  printf("bme cd (0x%02x)\n", bme_read_calib_data());
  //printf("T[%u,%d,%d]\n", cd.dig_T1, cd.dig_T2, cd.dig_T3);
  //printf("P[%u,%d,%d,%d,%d,%d,%d,%d,%d]\n", cd.dig_P1, cd.dig_P2, cd.dig_P3, cd.dig_P4, cd.dig_P5, cd.dig_P6, cd.dig_P7, cd.dig_P8, cd.dig_P9 );
  //printf("H[%u,%d,%u,%d,%d,%d]\n", cd.dig_H1, cd.dig_H2, cd.dig_H3, cd.dig_H4, cd.dig_H5, cd.dig_H6 );
  printf("bme start (0x%02x)\n", bme_start_conversion());
  sleep(10);
  printf("bme read (0x%02x)\n", bme_read_data());
  //printf("p=%lu, t=%lu, h=%u\n", ud.pressure, ud.temperature, ud.humidity);
  start_adc();

  uint16_t engine_stop_ms = ticks_ms();
  ecu_status status = {0};
  uint16_t loop_ms = 0;
  while (1)
  {
    status.rpm = rpm();
    if (!status.rpm)
    {
      // keep the stop time sliding for when we attempt to start again
      if ((ticks_ms() - engine_stop_ms) > config.dwell_time_ms)
      {
        engine_stop_ms = ticks_ms() - config.dwell_time_ms;
      }
    }
    if (!ignition_enabled())
    {
      if (status.rpm > 0 && (ticks_ms() - engine_stop_ms) > config.dwell_time_ms)
      {
        ignition_enable();
        pump_enable();
        printf("engine start\n");
      }
    }
    else
    {
      if (status.rpm > config.rpm_limit)
      {
        printf("overrev - forced engine stop\n");
        ignition_disable();
        pump_disable();
        engine_stop_ms = ticks_ms();
      }
      if (!status.rpm)
      {
        printf("engine has stopped\n");
        ignition_disable();
        pump_disable();
      }
    }

    status.pwm0_in = pwm_input();
    status.pwm0_out = clamp_pwm(status.pwm0_in);
    set_pwm(0, status.pwm0_out);

    const float t_scale = 1.0 / (float)(config.pwm_max - config.pwm_min);
    status.throttle = (float)(status.pwm0_out - config.pwm_min) * t_scale;

    inj_map_update_row(status.throttle);

    uint16_t ms = ticks_ms();
    //uint32_t us = ticks_us();

    if ((ms - loop_ms) >= 1000)
    {
      loop_ms += 1000;

      status.cht = interp_a_tab(config.a0cal, analogue(0));
      status.iat = interp_a_tab(config.a1cal, analogue(1));

      if (0 == bme_read_data()) {
        status.baro = bme_baro();
        status.ecut = bme_temp();
        status.humidity = bme_humidity();
      }
      status.egt = max6675_read();
      //printf("pwm_in=%u pwm_out=%u us=%lu a0=%d\n", pwm_in, pwm_out, us, a);

      printf("pwm=%d throttle=%d rpm=%u ticks=%u cht=%d iat=%d p=%lu, t=%d, h=%u egt=%ld\n",
             status.pwm0_in, (int)(100*status.throttle), status.rpm, inj_ticks_(rpm()),
             status.cht, status.iat,
             status.baro, status.ecut, status.humidity,
             status.egt);
      // start next conversion
      bme_start_conversion();
      start_adc();
    }
    //sleep(1000);
    //_delay_ms(1000);
  }  
}

    
