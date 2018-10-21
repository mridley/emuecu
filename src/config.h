#ifndef CONFIG_H
#define CONFIG_H

#ifndef F_CPU
#define F_CPU 			(16000000UL)       	// Mhz
#endif

#include <stdint.h>

#define PWM_LIMIT      (2500)
#define RPM_LIMIT      (15000)
#define CONFIG_VERSION (1)

#define MAP_ROWS       (10)
#define MAP_COLS       (10)

#define A_TAB_IDX_BITS (4)
#define A_TAB_SIZE     ((1<<A_TAB_IDX_BITS) + 1)

typedef struct _config
{
  uint8_t  version;   // int
  uint16_t pwm0_min;  // us throttle
  uint16_t pwm0_max;  // us
  uint16_t pwm1_min;  // us starter?
  uint16_t pwm1_max;  // us
  uint8_t  auto_start; // attempts
  uint16_t rpm_limit; // rpm

  uint8_t  capacity;  // cc
  uint16_t inj_open;  // us
  uint16_t inj_close; // us
  uint16_t inj_flow;  // g/min

  uint16_t idle_rpm;      // rpm
  uint16_t dwell_time_ms; // ms
  uint16_t start_time_ms; // ms

  // tables and calibration
  int16_t  a0cal[A_TAB_SIZE]; // 100*degrees
  int16_t  a1cal[A_TAB_SIZE]; // 100*degrees
  uint8_t  inj_map[MAP_ROWS][MAP_COLS];   // [throttle  ][rpm] ticks (16us)
  int8_t   ign_adv[MAP_COLS];

  uint16_t checksum;
} emuconfig_t;

extern emuconfig_t config;

void config_defaults();
void config_load();
void config_save();
void config_dump();

#endif
