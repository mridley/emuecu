#ifndef _INPUTS_H_
#define _INPUTS_H_

#include <stdint.h>
#include "config.h"

int16_t analogue(uint8_t ch);
void start_adc();
/* perform table lookup with interpolation */
int16_t interp_a_tab(const int16_t tab[A_TAB_SIZE], uint16_t v);

uint16_t pwm_input();
uint32_t crank_ticks_us();

void setup_inputs();

// igition pass through
uint16_t rpm();
uint8_t ignition_enabled();
void ignition_enable();
void ignition_disable();


#endif
