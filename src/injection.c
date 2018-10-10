#include <util/atomic.h>
#include <string.h>

#include "injection.h"
#include "config.h"

// injection map in multiples of 16 us i.e. 0 - 4.096 ms
// Throttle [0 - 100%] vs RPM [0, 8192, 16384, 24576]
uint8_t inj_map[4][4] =
{
  //     0     8192    16384    24576
  {2400>>4, 1600>>4,    0>>4,    0>>4 }, //  0  %
  {2400>>4, 2400>>4, 2400>>4,    0>>4 }, // 33  %
  {2600>>4, 2600>>4, 2600>>4, 1900>>4 }, // 66  %
  {2800>>4, 2800>>4, 2800>>4, 2000>>4 }, // 100 %

//  {2000>>4, 2000>>4,    0>>4,    0>>4 }, //  0  %
//  {2400>>4, 2400>>4, 2400>>4,    0>>4 }, // 33  %
//  {2800>>4, 2800>>4, 2800>>4, 2800>>4 }, // 66  %
//  {3200>>4, 3200>>4, 3200>>4, 3200>>4 }, // 100 %
};

uint8_t test_map[16][16] = {{0}};

// mask for each rpm section of table
const uint16_t RPM_MASK = (1 << 13) - 1;

// the current active row (based on throttle position)
uint8_t _inj_row[4] = {0, 0, 0, 0};

void update_inj_row(float throttle)
{
  uint8_t tmp[4];
  uint8_t row = (uint8_t)(throttle * 2.99);
  float wgt = 3.0 * throttle - row;

  for (uint8_t i = 0; i < 4; ++i)
  {
    tmp[i] = (uint8_t)((float)inj_map[row][i] + wgt * (inj_map[row + 1][i] - inj_map[row][i]));
  }
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    memcpy(_inj_row, tmp, 4);
  }
}

// fixed point linear interpolation
uint8_t inj_ticks_(uint16_t rpm)
{
  uint8_t col = (uint8_t)(rpm >> 13);
  int16_t dt = (_inj_row[col+1] >> 1) - (_inj_row[col] >> 1);
  int16_t dt_s = (dt * ((rpm & RPM_MASK) >> 5 )); // int8_t * uint8_t
  uint8_t t = _inj_row[col] + (uint8_t)(dt_s >> 7);
  return t;
}
