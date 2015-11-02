#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/sleep.h>


#include "timers.h"
#include "inputs.h"

volatile uint32_t timer_0_1ms_ = 0;
volatile uint32_t timer_1_ovf_ = 0;
volatile uint32_t timer_2_ovf_ = 0;

uint16_t pwm_[2] = {0, 0};

void set_pwm(uint8_t c, uint16_t v)
{
  c &= 0x01;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    pwm_[c] = v;
  }
}

void schedule_pwm_();

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

ISR(TIMER0_OVF_vect)  // timer0 overflow interrupt
{
  // nothing
}

ISR(TIMER0_COMPA_vect)  // timer0 compa interrupt
{
  timer_0_1ms_++;
  // every 16ms begin a new PWM cycle
  if (!(timer_0_1ms_ & 0xf))
  {
    schedule_pwm_();
  }
}

ISR(TIMER0_COMPB_vect)  // timer0 compb interrupt
{
  // nothing
}

void schedule_pwm_()
{
  uint16_t ticks;
  uint16_t trigger;

  // PWM A
  if (pwm_[0])
  {
    PORTD |= _BV(PD5); // PWM A high
    ticks = tcnt1_();
    trigger = ticks + (pwm_[0] << 1);
    OCR1AH = (uint8_t)(trigger >> 8);
    OCR1AL = (uint8_t)(trigger & 0xf);
    // enable the OC interrupt
    TIMSK1 |= _BV(OCIE1A);
  }

  // PWM B
  if (pwm_[1])
  {
    PORTD |= _BV(PD6); // PWM B high
    ticks = tcnt1_();
    trigger = ticks + (pwm_[1] << 1);
    OCR1BH = (uint8_t)(trigger >> 8);
    OCR1BL = (uint8_t)(trigger & 0xf);
    // enable the OC interrupt
    TIMSK1 |= _BV(OCIE1B);
  }
}

void setup_timer1()
{
  // Set the Timer Mode to CTC
  TCCR1B = 0; // Normal

  // Set the ISR Overflow vect
  TIMSK1 |= _BV(TOIE1);

  // set prescaler and start the timer
  TCCR1B |= _BV(CS11); // 8 (500ns per tick)
}

ISR(TIMER1_OVF_vect)  // timer1 overflow interrupt
{
  timer_1_ovf_++;
}

ISR(TIMER1_COMPA_vect)  // timer1 compa interrupt
{
  PORTD &= ~_BV(PD5);     // PWM A low
  TIMSK1 &= ~_BV(OCIE1A); // disable until next time
}

ISR(TIMER1_COMPB_vect)  // timer1 compb interrupt
{
  PORTD &= ~_BV(PD6);     // PWM B low
  TIMSK1 &= ~_BV(OCIE1B); // disable until next time
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

ISR(TIMER2_OVF_vect)  // timer2 overflow interrupt
{
  timer_2_ovf_++;
}

ISR(TIMER2_COMPA_vect)  // timer2 compa interrupt
{
}

ISR(TIMER2_COMPB_vect)  // timer2 compb interrupt
{
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

