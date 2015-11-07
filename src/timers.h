#ifndef _TIMERS_H_
#define _TIMERS_H_

#include <stdint.h>

extern volatile uint32_t timer_0_1ms_;
extern volatile uint32_t timer_1_ovf_;
extern volatile uint32_t timer_2_ovf_;

inline uint32_t tcnt2_us_()
{
  uint8_t ovf;
  uint16_t tcnt2;

  do
  {
    //loop in case TOV2 changes while reading TCNT2
    ovf = TIFR2 & _BV(TOV2);
    tcnt2 = TCNT2;
  }while (ovf  != (TIFR2 & _BV(TOV2)) );

  return (uint32_t)(tcnt2 << 1) | ((timer_2_ovf_ + ovf) << 9);
}

void setup_timers();
void set_pwm(uint8_t c, uint16_t v);
void set_injection_us(uint16_t dur);
void do_injection();
void pump_enable();
void pump_disable();

uint32_t ticks_us();
uint16_t ticks_ms();

void sleep(int ms);

#endif
