#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "injection.h"
#include "log.h"

// defaults
#define THR_MIN       (1000)
#define THR_START     (1050)
#define THR_MAX       (2000)
#define PWM0_MIN      (1000)
#define PWM0_MAX      (2080)
#define PWM1_MIN      (1000)
#define PWM1_MAX      (2000)

#define DWELL_TIME_MS (2000)
#define START_TIME_MS (1000)
#define IDLE_RPM      (1200)

#define CONFIG_EE_ADDR (0)

emuconfig_t config;

const int16_t atab0[A_TAB_SIZE] PROGMEM = {
  -1008,   358,  1532,  2541,
  3411,  4168, 4840, 5452,
  6031,  6604,  7197,  7837,
  8550, 9364, 10304, 11397, 12670
};

const int16_t atab1[A_TAB_SIZE] PROGMEM = {
  -2775, -1848, -1058,  -385,
  192,   698,  1154, 1580,
  1999,  2434,  2905,  3434,
  4045,  4758,  5595,  6578,  7730
};

void config_defaults()
{
  config.version = CONFIG_VERSION;
  config.dwell_time_ms = DWELL_TIME_MS;
  config.start_time_ms = START_TIME_MS;
  config.auto_start = 5;
  config.idle_rpm = IDLE_RPM;
  config.thr_min = THR_MIN;
  config.thr_start = THR_START;
  config.thr_max = THR_MAX;
  config.pwm0_max = PWM0_MAX;
  config.pwm0_min = PWM0_MIN;
  config.pwm1_max = PWM1_MAX;
  config.pwm1_min = PWM1_MIN;
  config.rpm_limit = RPM_LIMIT; // set to table boundary
  config.capacity = 35;
  config.inj_open = 900;
  config.inj_close = 650;
  config.inj_flow = 38;

  memcpy_P(config.a0cal, atab0, sizeof(config.a0cal));
  memcpy_P(config.a1cal, atab1, sizeof(config.a1cal));
  inj_map_default();
  memset(config.ign_adv, 0, sizeof(config.ign_adv));
  config.checksum = 0;
}

uint16_t calc_chksum(const void* const data, size_t n)
{
  uint16_t chksum = 0;
  const uint8_t* end_p = (const uint8_t*)data + n;
  for (const uint8_t* p = (const uint8_t*)data; p < end_p ; ++p)
  {
    chksum += *p;
  }
  return chksum;
}

void config_load()
{
  eeprom_read_block(&config, CONFIG_EE_ADDR, sizeof(emuconfig_t));
  uint16_t checksum = calc_chksum(&config, sizeof(config) - sizeof(config.checksum));
  if (checksum != config.checksum) {
    logmsgf("invalid config: using defaults");
    config_defaults();
  }
}

void config_save()
{
  config.checksum = calc_chksum(&config, sizeof(config) - sizeof(config.checksum));
  eeprom_update_block(&config, CONFIG_EE_ADDR, sizeof(emuconfig_t));
}

void config_dump()
{
  printf("{\"config\":{");

  printf("\"version\":%u,", (uint16_t)config.version);
  printf("\"thr_min\":%u,", config.thr_min);
  printf("\"thr_start\":%u,", config.thr_start);
  printf("\"thr_max\":%u,", config.thr_max);
  printf("\"pwm0_min\":%u,", config.pwm0_min);
  printf("\"pwm0_max\":%u,", config.pwm0_max);
  printf("\"pwm1_min\":%u,", config.pwm1_min);
  printf("\"pwm1_max\":%u,", config.pwm1_max);
  printf("\"auto_start\":%u,", (uint16_t)config.auto_start);
  printf("\"rpm_limit\":%u,", config.rpm_limit);

  printf("\"capacity\":%u,", (uint16_t)config.capacity);
  printf("\"inj_open\":%u,", config.inj_open);
  printf("\"inj_close\":%u,", config.inj_close);
  printf("\"inj_flow\":%u,", config.inj_flow);

  printf("\"idle_rpm\":%u,", config.idle_rpm);
  printf("\"dwell_time_ms\":%u,", config.dwell_time_ms);
  printf("\"start_time_ms\":%u,", config.start_time_ms);

  dump_array_int16_t("a0cal", config.a0cal, sizeof(config.a0cal)/sizeof(config.a0cal[0]));
  printf(",");
  dump_array_int16_t("a1cal", config.a1cal, sizeof(config.a1cal)/sizeof(config.a1cal[0]));
  printf(",");
  inj_map_dump();
  printf(",");
  dump_array_int16_t("ign_adv", config.ign_adv, sizeof(config.ign_adv)/sizeof(config.ign_adv[0]));
  printf(",");

  printf("\"checksum\":\"0x%04x\"", config.checksum);

  printf("}}\n");
}
