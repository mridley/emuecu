#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/sleep.h>


#include "timers.h"

volatile uint32_t timer_0_1ms_ = 0;
volatile uint32_t timer_1_ovf_ = 0;
volatile uint32_t timer_2_ovf_ = 0;


ISR(TIMER0_OVF_vect)  // timer0 overflow interrupt
{
  // nothing
}

ISR(TIMER0_COMPA_vect)  // timer0 compa interrupt
{
  timer_0_1ms_++;
}

ISR(TIMER0_COMPB_vect)  // timer0 compb interrupt
{
  // nothing
}

ISR(TIMER1_OVF_vect)  // timer1 overflow interrupt
{
  timer_1_ovf_++;
}

ISR(TIMER1_COMPA_vect)  // timer1 compa interrupt
{
}

ISR(TIMER1_COMPB_vect)  // timer1 compa interrupt
{
}

ISR(TIMER2_OVF_vect)  // timer1 overflow interrupt
{
  timer_2_ovf_++;
}

ISR(TIMER2_COMPA_vect)  // timer1 compa interrupt
{
}

ISR(TIMER2_COMPB_vect)  // timer1 compa interrupt
{
}

void setup_timer0()
{
  // Set the Timer Mode to CTC
  TCCR0A |= _BV(WGM01);

  // Set the value that you want to count to
  OCR0A = 0xFA;

  //Set the ISR COMPA vect
  TIMSK0 |= _BV(OCIE0A);

  // set prescaler and start the timer
  TCCR0B |= _BV(CS01) | _BV(CS00); // 64 (4us per tick)
}

void setup_timer1()
{
  // Set the Timer Mode to CTC
  TCCR1B = 0; // Normal

  // Set the ISR COMPA vect
  TIMSK1 |= _BV(TOIE1) | _BV(OCIE1A) | _BV(OCIE1B);

  // set prescaler and start the timer
  TCCR1B |= _BV(CS11); // 8 (500ns per tick)
}

void setup_timer2()
{
  // Set the Timer Mode to normal
  TCCR2A = 0;

  // enable all 3 ISR
  TIMSK2 |= _BV(TOIE2) | _BV(OCIE2A) | _BV(OCIE2B);

  // set prescaler and start the timer
  TCCR2B |= _BV(CS22); // 64 (4us per tick)
}

void setup_timers()
{
  setup_timer0();
  setup_timer1();
  setup_timer2();
}

uint16_t ticks_ms()
{
  uint16_t ta;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    ta = (uint16_t)timer_0_1ms_;
  }
  return ta;
}

uint32_t ticks_us()
{
  uint32_t ticks;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    ticks = tcnt1_us_();
  }
  return ticks;
}

void sleep(int ms)
{
  uint16_t t0 = ticks_ms();
  while((ticks_ms() - t0) <  ms)
  {
    sleep_cpu();
  }
}

