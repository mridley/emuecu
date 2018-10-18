#include <util/atomic.h>
#include <string.h>
#include <stdio.h>
#include "injection.h"
#include "config.h"

// injection map in ticks (16 us) i.e. 0 - 4.096 ms
// Throttle [0 - 100%] vs RPM [0, 8192, 16384, 24576]
//uint8_t inj_map[MAP_ROWS][MAP_COLS] =
//{
//  //           0            8192           16384           24576 RPM/THROTTLE
//  {MS2TICKS(2400), MS2TICKS(1600), MS2TICKS(   0), MS2TICKS(   0) }, //  0  %
//  {MS2TICKS(2400), MS2TICKS(2400), MS2TICKS(2400), MS2TICKS(   0) }, // 33  %
//  {MS2TICKS(2600), MS2TICKS(2600), MS2TICKS(2600), MS2TICKS(1900) }, // 66  %
//  {MS2TICKS(2800), MS2TICKS(2800), MS2TICKS(2800), MS2TICKS(2000) }, // 100 %
//};


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
      int16_t inj_time_us = config.inj_open + (int16_t)(full_inj_time_us * throttle * ve) + config.inj_close;
      if (inj_time_us < 0) {
        inj_time_us = 0;
      } else if (inj_time_us > INJ_TIME_MAX) {
        inj_time_us = INJ_TIME_MAX;
      }
      config.inj_map[j][i] = US2TICKS(inj_time_us);
    }
  }
}

// mask for each rpm section of table
const uint16_t RPM_MASK = (1 << MAP_RPM_BITS_PER_COL) - 1;

// the current active row (based on throttle position)
static uint8_t _inj_row[MAP_COLS] = {0, 0, 0, 0};

void inj_map_update_row(float throttle)
{
  uint8_t tmp[MAP_COLS];
  uint8_t row = (uint8_t)(throttle * (MAP_ROWS - 1));
  if (row > (MAP_ROWS - 2U))
  {
    row = MAP_ROWS - 2U;
  }
  float wgt = (MAP_ROWS - 1U) * throttle - row;

  for (uint8_t i = 0U; i < MAP_COLS; ++i)
  {
    tmp[i] = (uint8_t)((float)config.inj_map[row][i] + wgt * (config.inj_map[row + 1][i] - config.inj_map[row][i]));
  }
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    memcpy(_inj_row, tmp, MAP_COLS);
  }
}

// fixed point linear interpolation
uint8_t inj_ticks_(uint16_t rpm)
{
  uint8_t col = (uint8_t)(rpm >> MAP_RPM_BITS_PER_COL);
  if (col >= MAP_COLS) {
    col = MAP_COLS - 1;
  }
  int16_t dt_s1 = ((int16_t)_inj_row[col+1] - (int16_t)_inj_row[col]) >> 1; // make fit in int8_t
  int16_t dt_s = (dt_s1 * ((rpm & RPM_MASK) >> (MAP_RPM_BITS_PER_COL - 8) )); // int8_t * uint8_t
  uint8_t t = _inj_row[col] + (uint8_t)(dt_s >> 7); // 7 + 1 above for 8 bits not shifted
  return t;
}

void inj_map_dump(void)
{
  for (uint8_t j = 0; j < MAP_ROWS; j++)
  {
    printf("[");
    for (uint8_t i = 0; i < MAP_COLS; i++)
    {
      printf("%04d ", (int16_t)TICKS2US(config.inj_map[j][i]));
    }
    printf("]\n");
  }
}
