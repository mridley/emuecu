#include <util/atomic.h>
#include <string.h>
#include <stdio.h>
#include "injection.h"
#include "config.h"

#if ((3*MAP_MAX_RPM/4)>RPM_LIMIT)
#error RPM limit deep inside table
#endif

#if (MAP_MAX_RPM<RPM_LIMIT)
#error RPM limit outside table
#endif


void inj_map_default(void)
{
  //printf("capacity=%f", (float)config.capacity);
  float full_delivery_g = ((AIR_DENSITY_15C_SL*(float)config.capacity)/AFR);
  float full_inj_time_us = (full_delivery_g/((float)config.inj_flow/60.0f))*1e6;
  //printf("full_delivery_g=%f\n", full_delivery_g );
  //printf("full_inj_time=%f\n", full_inj_time_us );
  for (uint8_t j = 0; j < MAP_ROWS; j++)
  {
    float throttle = (float)j/(float)(MAP_ROWS-1);
    for (uint8_t i = 0; i < MAP_COLS; i++)
    {
      // guess volumetric efficiency at low rpm, ve nom at mid rpm
      float ve = VE_NOM + (VE_MAX - VE_NOM)*(float)((int8_t)(MAP_COLS - 1)/2 - i)/(float)((MAP_COLS - 1)/2U);

      int16_t inj_time_us = (int16_t)(FUDGE_MULT * full_inj_time_us * throttle * ve) + FUDGE_ADD;
      if (inj_time_us < 0) {
        inj_time_us = 0;
      } else if (inj_time_us > INJ_TIME_MAX) {
        inj_time_us = INJ_TIME_MAX;
      }
      config.inj_map[j][i] = (inj_ticks_t)US2TICKS(inj_time_us);
    }
  }
}

// mask for each rpm section of table
const uint16_t RPM_MASK = (1 << MAP_RPM_BITS_PER_COL) - 1;

// the current active row (based on throttle position)
static inj_ticks_t _inj_row[MAP_COLS] = {0};

float inj_corrections(uint32_t baro, int16_t iat, int16_t cht, uint16_t run_time_ms)
{
  // baro*temp is bigger than INT_MAX cast to uint32_t
  float pt_c = (float)(baro*(uint32_t)(ZEROC_KELVIN+TEMP_REF_CDEGC)/(float)(BARO_MSLP_PA*(uint32_t)(ZEROC_KELVIN+iat)));
  if (cht < CHT_COLD_ENRICH_T) {
    float cht_c = 1.0f + CHT_ENRICH_RATE*0.01*(float)(CHT_COLD_ENRICH_T - cht);
    if (cht_c > CHT_ENRICH_MAXFACTOR) {
      cht_c = CHT_ENRICH_MAXFACTOR;
    }
    pt_c *= cht_c;
  }
  if (cht > CHT_HOT_ENRICH_T) {
    float cht_c = 1.0f + CHT_ENRICH_RATE*0.01*(float)(cht - CHT_HOT_ENRICH_T);
    if (cht_c > CHT_ENRICH_MAXFACTOR) {
      cht_c = CHT_ENRICH_MAXFACTOR;
    }
    pt_c *= cht_c;
  }
  if (run_time_ms < config.dwell_time_ms) {
    pt_c *= START_ENRICH_FACTOR;
  } else if (run_time_ms < 2*config.dwell_time_ms) {
    pt_c *= START_ENRICH_FACTOR *((float)(2*config.dwell_time_ms-run_time_ms)/(float)config.dwell_time_ms);
  }
  return pt_c;
}

void inj_map_update_row(float throttle, float pt_c)
{
  inj_ticks_t tmp[MAP_COLS];
  uint8_t row = (uint8_t)(throttle * (MAP_ROWS - 1));
  if (row > (MAP_ROWS - 2U))
  {
    row = MAP_ROWS - 2U;
  }
  float wgt = (MAP_ROWS - 1U) * throttle - row;
  for (uint8_t i = 0U; i < MAP_COLS; ++i)
  {
    float v_uc = (float)config.inj_map[row][i] + wgt * (config.inj_map[row + 1][i] - config.inj_map[row][i]);
    float val = (v_uc*pt_c);

    if (val < 0.0f) {
      tmp[i] = 0U;
    } else if (val < INJ_TICKS_MAX) {
      tmp[i] = (inj_ticks_t)val;
    } else {
      tmp[i] = INJ_TICKS_MAX;
    }
  }
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    memcpy(_inj_row, tmp, MAP_COLS*sizeof(inj_ticks_t));
  }
}

// faster fixed point linear interpolation
// as this is done inside the ISR
inj_ticks_t inj_ticks_(uint16_t rpm)
{
  uint8_t col = (uint8_t)(rpm >> MAP_RPM_BITS_PER_COL);
  if (col >= (MAP_COLS-1)) {
    return _inj_row[MAP_COLS-1];
  }
  int16_t dt = ((int16_t)_inj_row[col+1] - (int16_t)_inj_row[col]); // int16_t
  int16_t dt_s = (int16_t)(((int32_t)dt * (rpm & RPM_MASK)) >> MAP_RPM_BITS_PER_COL); // int16_t * uint16_t
  int16_t t_ticks = (int16_t)_inj_row[col] + dt_s; //

  // Q: how much of the close time delivers fuel ? Attribute 0 for now.
  uint16_t t_us = config.inj_open + TICKS2US(t_ticks);
  if (t_us > INJ_TIME_MAX) {
    t_us = INJ_TIME_MAX;
  }

  return (inj_ticks_t)US2TICKS(t_us);
}

void inj_map_dump(void)
{
  printf("\"map\":[");
  for (uint8_t j = 0; j < MAP_ROWS;) {
    printf("[");
    for (uint8_t i = 0; i < MAP_COLS;) {
      printf("%d", TICKS2US(config.inj_map[j][i]));
      if (++i < MAP_COLS) {
        printf(",");
      }
    }
    printf("]");
    if (++j < MAP_ROWS) {
      printf(",");
    }
  }
  printf("]");
}
