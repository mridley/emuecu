#ifndef _INJECTION_H_
#define _INJECTION_H_

#include <stdint.h>

void update_inj_row(float throttle);

// called from isr
// returns injection time on 16us ticks
uint8_t inj_ticks_(uint16_t rpm);

#endif
