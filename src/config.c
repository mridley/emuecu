#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "injection.h"
#include "log.h"
#include "ecu.h"

extern emustatus_t status;

// defaults
#define THR_MIN       (1000)
#define THR_START     (1070)
#define THR_MAX       (2000)
#define PWM0_MIN      (1980)
#define PWM0_MAX      (1171)
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

bool config_load()
{
  eeprom_read_block(&config, CONFIG_EE_ADDR, sizeof(emuconfig_t));
  if (config.version != CONFIG_VERSION) {
    logmsgf("wrong config version");
    return false;
  }

  const uint16_t checksum = calc_chksum(&config, sizeof(config) - sizeof(config.checksum));
  if (checksum != config.checksum) {
    logmsgf("bad config checksum");
    return false;
  }
  logmsgf("config loaded ok");
  return true;
}

void config_save()
{
  config.checksum = calc_chksum(&config, sizeof(config) - sizeof(config.checksum));
  eeprom_update_block(&config, CONFIG_EE_ADDR, sizeof(emuconfig_t));
}

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

enum config_type {
    CTYPE_UINT8,
    CTYPE_UINT16,
    CTYPE_INT16,
    CTYPE_INT16_ARRAY,
    CTYPE_UINT16_2D_ARRAY,
};

static const struct ctable {
    const char name[14];
    enum config_type type;
    void *ptr;
    uint8_t tsize1, tsize2;
    bool read_only;
} config_table[] PROGMEM = {
    { "version", CTYPE_UINT8, &config.version, 0, 0, true },
    { "thr_min", CTYPE_UINT16, &config.thr_min },
    { "thr_start", CTYPE_UINT16, &config.thr_start },
    { "thr_max", CTYPE_UINT16, &config.thr_max },
    { "pwm0_min", CTYPE_UINT16, &config.pwm0_min },
    { "pwm0_max", CTYPE_UINT16, &config.pwm0_max },
    { "pwm1_min", CTYPE_UINT16, &config.pwm1_min },
    { "pwm1_max", CTYPE_UINT16, &config.pwm1_max },
    { "auto_start", CTYPE_UINT8, &config.auto_start },
    { "rpm_limit", CTYPE_UINT16, &config.rpm_limit },
    { "capacity", CTYPE_UINT8, &config.capacity },
    { "inj_open", CTYPE_UINT16, &config.inj_open },
    { "inj_close", CTYPE_UINT16, &config.inj_close },
    { "inj_flow", CTYPE_UINT16, &config.inj_flow },
    { "idle_rpm", CTYPE_UINT16, &config.idle_rpm },
    { "dwell_time_ms", CTYPE_UINT16, &config.dwell_time_ms },
    { "start_time_ms", CTYPE_UINT16, &config.start_time_ms },
    { "checksum", CTYPE_UINT16, &config.checksum },
    { "a0cal", CTYPE_INT16_ARRAY, &config.a0cal[0], ARRAY_LEN(config.a0cal) },
    { "a1cal", CTYPE_INT16_ARRAY, &config.a1cal[0], ARRAY_LEN(config.a1cal) },
    { "ign_adv", CTYPE_INT16_ARRAY, &config.ign_adv[0], ARRAY_LEN(config.ign_adv) },
    { "inj_map", CTYPE_UINT16_2D_ARRAY, &config.inj_map[0][0], ARRAY_LEN(config.inj_map), ARRAY_LEN(config.inj_map[0]) },
    { "thr_over", CTYPE_INT16, &status.throttle_override },
};

#define CONFIG_TABLE_LEN ARRAY_LEN(config_table)

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

/*
  parse array reference xxx[idx1][idx2]
  if an array reference is found then name is truncated at the first [
  NOTE: this modifies the input string
*/
static void parse_array_name(char *name, int8_t *idx1, int8_t *idx2)
{
    char *p = strchr(name,'[');
    if (p == NULL) {
        return;
    }
    *p = 0;
    int8_t i = strtol(p+1, &p, 10);
    if (!p || *p != ']') {
        return;
    }
    *idx1 = i;
    if (p[1] == '[') {
        i = strtol(p+2, &p, 10);
        if (!p || *p != ']') {
            return;
        }
        *idx2 = i;
    }
}


/*
  show a config variable
 */
void config_show(char *name)
{
    int8_t idx1=-1, idx2=-1;
    parse_array_name(name, &idx1, &idx2);
    for (uint8_t i=0; i<CONFIG_TABLE_LEN; i++) {
        struct ctable c;
        memcpy_P(&c, &config_table[i], sizeof(c));
        if (strcmp(name, c.name) == 0) {
            switch (c.type) {
            case CTYPE_UINT8:
                printf("{\"var\":{\"%s\":%u}}\n", name, *(uint8_t *)c.ptr);
                break;
            case CTYPE_UINT16:
                printf("{\"var\":{\"%s\":%u}}\n", name, *(uint16_t *)c.ptr);
                break;
            case CTYPE_INT16:
                printf("{\"var\":{\"%s\":%d}}\n", name, *(int16_t *)c.ptr);
                break;
            case CTYPE_INT16_ARRAY:
                if (idx1 == -1) {
                    for (uint8_t j=0; j<c.tsize1; j++) {
                        printf("{\"var\":{\"%s[%u]\":%d}}\n", name, j, ((int16_t *)c.ptr)[j]);
                    }
                } else if (idx1 >= 0 && idx1 < c.tsize1) {
                    printf("{\"var\":{\"%s[%u]\":%d}}\n", name, idx1, ((int16_t *)c.ptr)[idx1]);
                }
                break;
            case CTYPE_UINT16_2D_ARRAY:
                if (idx1 >= 0 && idx1 < c.tsize1 && idx2 == -1) {
                    // show row
                    for (uint8_t j=0; j<c.tsize2; j++) {
                        printf("{\"var\":{\"%s[%u][%u]\":%u}}\n", name, idx1, j, ((uint16_t *)c.ptr)[idx1*c.tsize1+j]);
                    }
                } else if (idx1 >= 0 && idx1 < c.tsize1 && idx2 >= 0 && idx2 < c.tsize2) {
                    printf("{\"var\":{\"%s[%u][%u]\":%u}}\n", name, idx1, idx2, ((uint16_t *)c.ptr)[idx1*c.tsize1+idx2]);
                }
                break;
            }
            break;
        }
    }
}

/*
  set a config variable
 */
void config_set(char *name, const char *value)
{
    int8_t idx1=-1, idx2=-1;
    char name0[32];
    if (strlen(name) > sizeof(name0)-1) {
        return;
    }
    strncpy(name0, name, sizeof(name0)-1);
    parse_array_name(name0, &idx1, &idx2);
    for (uint8_t i=0; i<CONFIG_TABLE_LEN; i++) {
        struct ctable c;
        memcpy_P(&c, &config_table[i], sizeof(c));
        if (strcmp(name0, c.name) == 0) {
            if (c.read_only) {
                // don't allow set of read-only config values
                return;
            }
            switch (c.type) {
            case CTYPE_UINT8:
                *(uint8_t *)c.ptr = strtoul(value, NULL, 10);
                config_show(name);
                return;
            case CTYPE_UINT16:
                *(uint16_t *)c.ptr = strtoul(value, NULL, 10);
                config_show(name);
                return;
            case CTYPE_INT16:
                *(int16_t *)c.ptr = strtol(value, NULL, 10);
                config_show(name);
                return;
            case CTYPE_INT16_ARRAY:
                if (idx1 >= 0 && idx1 < c.tsize1) {
                    ((int16_t *)c.ptr)[idx1] = strtol(value, NULL, 10);
                    config_show(name);
                    return;
                }
                break;
            case CTYPE_UINT16_2D_ARRAY:
                if (idx1 >= 0 && idx1 < c.tsize1 && idx2 >= 0 && idx2 < c.tsize2) {
                    ((uint16_t *)c.ptr)[idx1*c.tsize1+idx2] = strtoul(value, NULL, 10);
                    config_show(name);
                    return;
                }
                break;
            }
        }
    }
}
