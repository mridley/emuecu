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

emustatus_t status = {0};

void engine_crank(bool crank)
{
  if (crank)
  {
    status.pwm1_out = config.pwm1_max;
    set_pwm(1, status.pwm1_out);
  }
  else
  {
    status.pwm1_out = config.pwm1_min;
    set_pwm(1, status.pwm1_out);
  }
}

void engine_stop()
{
  status.engine_stop_ms = ticks_ms();
  ignition_disable();
  pump_disable();
  engine_crank(false);
}

float throttle()
{
  return status.throttle_in;
}

void default_state()
{
  status.state = INIT;
  status.baro = BARO_MSLP_PA;
  status.pwm0_out = config.pwm0_min;
  status.pwm1_out = config.pwm1_min;
}

int main(void)
{
  uart0_init();

  //config_load();
  config_defaults();
  config_save();
  config_dump();
  //config_save();

  default_state();
  setup_timers(status.pwm0_out, status.pwm1_out);

  setup_inputs();
 
  sei(); // Enable Global Interrupt
  logmsgf("EMU ECU");

  bme_read_calib_data();
  bme_start_conversion();
  start_adc();
  sleep(10);

  status.engine_stop_ms = ticks_ms();
  status.engine_start_ms = ticks_ms();
  uint16_t loop_ms = ticks_ms() - 1000;
  while (1)
  {
    uint16_t ms = ticks_ms();
    status.rpm = rpm();
    status.thr_in = pwm_input();
    uint16_t thr_clamped = clamp_pwm(status.thr_in, config.thr_min, config.thr_max);
    const float t_scale = 1.0 / (float)(config.thr_max - config.thr_min);
    status.throttle_in = (float)(thr_clamped - config.thr_min) * t_scale;
    status.throttle_out = throttle();

    status.pwm0_out = clamp_pwm(config.pwm0_min + (uint16_t)(status.throttle_out*(config.pwm0_max-config.pwm0_min)),
                                config.pwm0_min, config.pwm0_max);
    set_pwm(0, status.pwm0_out);

    status.pt_c = inj_corrections(status.baro, status.iat, status.cht,
                                  status.state == START);

    inj_map_update_row(status.throttle_out, status.pt_c);

    // keep the start/stop times sliding to prevent wraps
    if ((ms - status.engine_stop_ms) > 2*config.dwell_time_ms)
    {
      status.engine_stop_ms = ms - config.dwell_time_ms;
    }
    if ((ms - status.engine_start_ms) > 2*config.dwell_time_ms)
    {
      status.engine_start_ms = ms - config.dwell_time_ms;
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
      }

      int32_t tval = max6675_read();
      if (tval >= 0) {
        status.egt = tval;
      } else {
        logmsgf("max6675 error: %d", tval);
      }

      printf("{\"status\":{\"thr_in\":%d,\"throttle_in\":%d,\"throttle_out\":%d,\"rpm\":%u,\"cht\":%d,\"iat\":%d}}\n",
             status.thr_in, (int)(100*status.throttle_in), (int)(100*status.throttle_out), status.rpm,
             status.cht, status.iat);
      printf("{\"status\":{\"baro\":%lu,\"ecut\":%d,\"humidity\":%u,\"egt\":%lu}}\n",
             status.baro, status.ecut, status.humidity, status.egt);
      printf("{\"status\":{\"pt_c\":%f,\"starts\":%u}}\n",
             status.pt_c, status.starts);
      printf("{\"status\":{\"pwm0_out\":%d,\"pwm1_out\":%d,\"inj_ticks\":%u}}\n",
             status.pwm0_out, status.pwm1_out, inj_ticks_(rpm()));

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
        status.engine_prime_ms = ticks_ms();
        pump_enable();
        logmsgf("engine prime");
        status.state = PRIME;
      }
      break;
    case PRIME:
      if ((ms - status.engine_prime_ms) > 1000) {
        engine_stop();
        logmsgf("engine stopped");
        status.state = STOPPED;
      }
      break;
    case STOPPED:
      if ((ms - status.engine_stop_ms) > config.dwell_time_ms) {
        if (config.auto_start &&
            (status.thr_in < config.thr_start)) {
          status.starts = 0;
        }
        if ((status.rpm > 0) &&
            (0.0f < status.throttle_in)) {
          status.engine_start_ms = ticks_ms();
          ignition_enable();
          pump_enable();
          logmsgf("engine start");
          status.state = START;
        } else if (config.auto_start &&
                   (status.thr_in > config.thr_start) &&
                   (status.starts < config.auto_start)) {
          status.starts++;
          status.engine_start_ms = ticks_ms();
          engine_crank(true);
          pump_enable();
          logmsgf("engine crank");
          status.state = CRANK;
        }
      }
      break;
    case CRANK:
      if ((ms - status.engine_start_ms) > config.start_time_ms) {
        engine_stop();
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
          ((ms - status.engine_start_ms) > config.start_time_ms)) {
        engine_crank(false);
        logmsgf("cranked");
      }
      if ((status.rpm > 0) &&
          ((ms - status.engine_start_ms) > config.dwell_time_ms)) {
        status.starts = 0;
        logmsgf("engine running");
        status.state = RUNNING;
      }
      // fall through
    case RUNNING:
      if (status.rpm > config.rpm_limit) {
        engine_stop();
        logmsgf("overrev - engine stopped");
        status.state = STOPPED;
      } else if (status.throttle_in <= 0.0f ) {
        engine_stop();
        logmsgf("throttle - engine stopped");
        status.state = STOPPED;
      } else if (!status.rpm) {
        engine_stop();
        logmsgf("engine stopped");
        status.state = STOPPED;
      }
      break;
    default:
      break;
    }
  }
}
