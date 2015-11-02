#ifndef _INPUTS_H_
#define _INPUTS_H_

#include <stdint.h>

int16_t analogue(uint8_t ch);
void start_adc();

uint16_t pwm_input();
uint32_t crank_ticks_us();

void setup_inputs();

// igition pass through
uint16_t rpm();
uint8_t ignition_enabled();
void ignition_enable();
void ignition_disable();


#endif
