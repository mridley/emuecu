#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

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

inline uint16_t clamp_pwm(uint16_t v, uint16_t pwm_min, uint16_t pwm_max)
{
  // for no input scenario
  if (v > PWM_LIMIT)
  {
    v = 0U;
  }
  CLAMP(v, pwm_min, pwm_max);
  return v;
}


int main(void)
{
  emustatus_t status = {0};
  uart0_init();

  config_load();
  config_dump();
  //config_save();

  status.pwm0_out = config.pwm0_min;
  set_pwm(0, status.pwm0_out);
  status.pwm1_out = config.pwm1_min;
  set_pwm(1, status.pwm1_out);
  setup_timers();

  setup_inputs();
 
  sei(); // Enable Global Interrupt
  printf("EMU ECU\n");

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
  uint16_t engine_start_ms = ticks_ms();
  uint16_t engine_prime_ms;
  uint16_t loop_ms = ticks_ms() - 1000;
  while (1)
  {
    uint16_t ms = ticks_ms();
    status.rpm = rpm();
    status.pwm0_in = pwm_input();
    status.pwm0_out = clamp_pwm(status.pwm0_in, config.pwm0_min, config.pwm0_max);
    set_pwm(0, status.pwm0_out);

    const float t_scale = 1.0 / (float)(config.pwm0_max - config.pwm0_min);
    status.throttle = (float)(status.pwm0_out - config.pwm0_min) * t_scale;

    inj_map_update_row(status.throttle, status.pt_c);

    // keep the start/stop times sliding to prevent wraps
    if ((ms - engine_stop_ms) > 2*config.dwell_time_ms)
    {
      engine_stop_ms = ms - config.dwell_time_ms;
    }
    if ((ms - engine_start_ms) > 2*config.dwell_time_ms)
    {
      engine_start_ms = ms - config.dwell_time_ms;
    }
    // 1 second tasks
    if ((ms - loop_ms) >= 1000)
    {
      loop_ms += 1000;

      status.cht = interp_a_tab(config.a0cal, analogue(0));
      status.iat = interp_a_tab(config.a1cal, analogue(1));

      if (0 == bme_read_data()) {
        status.baro = bme_baro();
        status.ecut = bme_temp();
        status.humidity = bme_humidity();
        status.pt_c = inj_pt_correction(status.baro, status.iat);
      } else {
        status.pt_c = inj_pt_correction(BARO_MSLP_PA, status.iat);
      }

      status.egt = max6675_read();

      printf("pwm0_in=%d throttle=%d rpm=%u cht=%d iat=%d p=%lu, t=%d, h=%u egt=%ld\n",
             status.pwm0_in, (int)(100*status.throttle), status.rpm,
             status.cht, status.iat,
             status.baro, status.ecut, status.humidity,
             status.egt);

      printf("pwm0=%d pwm1=%d pt_c=%f ticks=%u\n",
             status.pwm0_out, status.pwm1_out, status.pt_c, inj_ticks_(rpm()) );

      // start next conversion
      bme_start_conversion();
      start_adc();

      //int c = getchar();
      //printf("c=%d\n", c);
    }
    switch (status.state)
    {
    case INIT:
      if (status.pt_c > 0.0) {
        engine_prime_ms = ticks_ms();
        pump_enable();
        printf("engine prime\n");
        status.state = PRIME;
      }
      break;
    case PRIME:
      if ((ms - engine_prime_ms) > 1000) {
        engine_stop_ms = ticks_ms();
        pump_disable();
        printf("engine stopped\n");
        status.state = STOPPED;
      }
      break;
    case STOPPED:
      if ( (ms - engine_stop_ms) > config.dwell_time_ms) {
        if (config.auto_start) {
          engine_start_ms = ticks_ms();
          status.pwm1_out = config.pwm1_max;
          set_pwm(1, status.pwm1_out);
          pump_enable();
          printf("engine crank\n");
          status.state = CRANK;
        }
        else if (status.rpm > 0) {
          engine_start_ms = ticks_ms();
          ignition_enable();
          pump_enable();
          printf("engine start\n");
          status.state = START;
        }
      }
      break;
    case CRANK:
      if ((ms - engine_start_ms) > config.start_time_ms) {
        engine_stop_ms = ticks_ms();
        status.pwm1_out = config.pwm1_min;
        set_pwm(1, status.pwm1_out);
        pump_disable();
        printf("crank failure - stopped\n");
        status.state = STOPPED;
      } else if (status.rpm > 0) {
        ignition_enable();
        printf("engine start\n");
        status.state = START;
      }
      break;
    case START:
      if (config.auto_start &&
          (status.pwm1_out == config.pwm1_max) &&
          (ms - engine_start_ms) > config.start_time_ms) {
        status.pwm1_out = config.pwm1_min;
        set_pwm(1, status.pwm1_out);
        printf("cranked\n");
      }
      if (status.rpm > 0 && (ms - engine_start_ms) > config.dwell_time_ms) {
        printf("engine running\n");
        status.state = RUNNING;
      }
      // fall through
    case RUNNING:
      if (status.rpm > config.rpm_limit)
      {
        engine_stop_ms = ticks_ms();
        ignition_disable();
        pump_disable();
        printf("overrev - forced engine stop\n");
        status.state = STOPPED;
      }
      if (!status.rpm)
      {
        engine_stop_ms = ticks_ms();
        ignition_disable();
        pump_disable();
        printf("engine has stopped\n");
        status.state = STOPPED;
      }
      break;
    default:
      break;
    }
  }
}

    
