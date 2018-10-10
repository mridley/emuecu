#ifndef CONFIG_H
#define CONFIG_H

#ifndef F_CPU
#define F_CPU 			(16000000UL)       	// Mhz
#endif

#include <stdint.h>

#define PWM_LIMIT     (2500)
#define CONFIG_VERSION (1)

typedef struct _config
{
  uint8_t  version;
  uint16_t pwm_min;
  uint16_t pwm_max;
  uint16_t rpm_limit;

  uint16_t idle_rpm;
  uint16_t dwell_time_ms;

  uint16_t a0cal[16];
  uint16_t a1cal[16];
  uint8_t  map[16][16];
  uint16_t checksum;
} emuconfig_t;

extern emuconfig_t config;

void config_defaults();
void config_load();
void config_save();
void config_dump();

#endif
