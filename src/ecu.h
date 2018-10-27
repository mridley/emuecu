#ifndef _ECU_H_
#define _ECU_H_

#include <stdint.h>

typedef enum  {
  INIT = 0,
  PRIME,
  STOPPED,
  CRANK,
  START,
  RUNNING,
} state_t;

typedef struct _ecu_status
{
  // inputs
  state_t  state;
  float throttle;
  uint16_t rpm;
  int16_t cht;   // -? - ~100
  int16_t iat;   // -? - 80
  uint32_t egt;  // 0 - 1024
  int16_t ecut;  // -? -
  uint32_t baro; // 0 - ~101300pa
  uint16_t humidity;
  // internal vars
  float pt_c;
  uint8_t starts;
  // outputs
  uint8_t inj_ticks;
  uint16_t thr_in;
  uint16_t pwm0_out;
  uint16_t pwm1_out;
} emustatus_t;

#endif
