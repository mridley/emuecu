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
#include "log.h"

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
  logmsgf("EMU ECU");

  bme_read_calib_data();
  bme_start_conversion();
  start_adc();
  sleep(10);

  uint16_t engine_stop_ms = ticks_ms();
  uint16_t engine_start_ms = ticks_ms();
  uint16_t engine_prime_ms;
  uint16_t loop_ms = ticks_ms() - 1000;
  while (1)
  {
    uint16_t ms = ticks_ms();
    status.rpm = rpm();
    status.thr_in = pwm_input();
    uint16_t thr_clamped = clamp_pwm(pwm_input(), config.thr_min, config.thr_max);
    const float t_scale = 1.0 / (float)(config.thr_max - config.thr_min);
    status.throttle = (float)(thr_clamped - config.thr_min) * t_scale;

    status.pwm0_out = clamp_pwm(config.pwm0_min + (uint16_t)(status.throttle*(config.pwm0_max-config.pwm0_min)),
                                config.pwm0_min, config.pwm0_max);
    set_pwm(0, status.pwm0_out);

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

      printf("{\"status\":{\"thr_in\":%d,\"throttle\":%d,\"rpm\":%u,\"cht\":%d,\"iat\":%d}}\n",
             status.thr_in, (int)(100*status.throttle), status.rpm,
             status.cht, status.iat);
      printf("{\"status\":{\"baro\":%lu,\"ecut\":%d,\"humidity\":%u,\"egt\":%lu}}\n",
             status.baro, status.ecut, status.humidity, status.egt);
      printf("{\"status\":{\"pt_c\":%f,\"start_attempts\":%u}}\n",
             status.pt_c, status.start_attempts);
      printf("{\"status\":{\"pwm0_out\":%d,\"pwm1_out\":%d,\"inj_ticks\"=%u}}\n",
             status.pwm0_out, status.pwm1_out, inj_ticks_(rpm()) );

      // start next conversion
      bme_start_conversion();
      start_adc();

      //int c = getchar();
      //logmsgf("c=%d", c);
    }
    switch (status.state)
    {
    case INIT:
      if (status.pt_c > 0.0) {
        engine_prime_ms = ticks_ms();
        pump_enable();
        logmsgf("engine prime");
        status.state = PRIME;
      }
      break;
    case PRIME:
      if ((ms - engine_prime_ms) > 1000) {
        engine_stop_ms = ticks_ms();
        pump_disable();
        logmsgf("engine stopped");
        status.state = STOPPED;
      }
      break;
    case STOPPED:
      if ( (ms - engine_stop_ms) > config.dwell_time_ms) {
        if (config.auto_start &&
            (status.thr_in < config.thr_start)) {
          status.start_attempts = 0;
        }
        if (config.auto_start &&
            (status.thr_in > config.thr_start) &&
            (status.start_attempts < config.auto_start)) {
          status.start_attempts++;
          engine_start_ms = ticks_ms();
          status.pwm1_out = config.pwm1_max;
          set_pwm(1, status.pwm1_out);
          pump_enable();
          logmsgf("engine crank");
          status.state = CRANK;
        }
        else if (status.rpm > 0) {
          engine_start_ms = ticks_ms();
          ignition_enable();
          pump_enable();
          logmsgf("engine start");
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
        logmsgf("crank failure - engine stopped");
        status.state = STOPPED;
      } else if (status.rpm > 0) {
        ignition_enable();
        logmsgf("engine start");
        status.state = START;
      }
      break;
    case START:
      if (config.auto_start &&
          (status.pwm1_out == config.pwm1_max) &&
          (ms - engine_start_ms) > config.start_time_ms) {
        status.pwm1_out = config.pwm1_min;
        set_pwm(1, status.pwm1_out);
        logmsgf("cranked");
      }
      if (status.rpm > 0 && (ms - engine_start_ms) > config.dwell_time_ms) {
        status.start_attempts = 0;
        logmsgf("engine running");
        status.state = RUNNING;
      }
      // fall through
    case RUNNING:
      if (status.rpm > config.rpm_limit)
      {
        engine_stop_ms = ticks_ms();
        ignition_disable();
        pump_disable();
        logmsgf("overrev - engine stopped");
        status.state = STOPPED;
      }
      if (!status.rpm)
      {
        engine_stop_ms = ticks_ms();
        ignition_disable();
        pump_disable();
        logmsgf("engine stopped");
        status.state = STOPPED;
      }
      break;
    default:
      break;
    }
  }
}

    
