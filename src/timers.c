#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/sleep.h>


#include "timers.h"
#include "inputs.h"

volatile uint32_t timer_1_ovf_ = 0;
volatile uint32_t timer_2_500us_ = 0;
volatile uint32_t timer_2_ovf_ = 0;

uint16_t pwm_[2] = {1500, 1500};
uint8_t inj_ticks_ = 0;
uint8_t pump_enabled_ = 0;

void set_pwm(uint8_t c, uint16_t v)
{
  c &= 0x01;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    pwm_[c] = v;
  }
}

void setup_timer0()
{
  // Set the Timer Mode to Normal
  TCCR0A = 0;

  // Set the value that you want to count to
  OCR0A = 0; //
  OCR0B = 0; //

  //Set the ISR COMPB vect
  TIMSK0 |= _BV(OCIE0B);

  // enable the output of the two OC0x ports PD5 PD6
  DDRD |= (_BV(PD6) | _BV(PD5));

  // set prescaler and start the timer
  TCCR0B |= _BV(CS02); // 256 (16us per tick)
}

ISR(TIMER0_OVF_vect)  // timer0 overflow interrupt
{
  // nothing
}

void set_injection_us(uint16_t dur)
{
  inj_ticks_ = (uint8_t)(dur >> 4);
}

void do_injection()
{
  if (inj_ticks_)
  {
    PORTD |= _BV(PD6); // Injector on
    OCR0A = TCNT0 + inj_ticks_; // set delay
    // enable the OC interrupt
    TIMSK0 |= _BV(OCIE0A);
    TIFR0 |= _BV(OCF0A); // clear interrupt flag
  }
}

ISR(TIMER0_COMPA_vect)  // timer0 compa interrupt
{
  PORTD &= ~_BV(PD6);     // Injector off
  TIMSK0 &= ~_BV(OCIE0A); // disable until next time
}

void pump_enable()
{
  pump_enabled_ = 1;
}

void pump_disable()
{
  pump_enabled_ = 0;
}

ISR(TIMER0_COMPB_vect)  // timer0 compb interrupt
{
  if (PORTD & _BV(PD5))
  {
    PORTD &= ~_BV(PD5);     // Pump off
    OCR0B = TCNT0 + 2;
  }
  else if (pump_enabled_)
  {
    PORTD |= _BV(PD5);
    OCR0B = TCNT0 + 3;
  }
}

void setup_timer1()
{
  // Enable OC1x Non inverting mode
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  // Set the Timer Mode to Fast PWM using ICR1 as TOP
  TCCR1B = _BV(WGM13) | _BV(WGM12);

  // Set the TOP value to 29999 ( 15ms @ 2MHz)
  ICR1H = 0x75;
  ICR1L = 0x2f;

  // Set the ISR Overflow vect
  TIMSK1 |= _BV(TOIE1);

  // enable the output of the two OC1x ports PB1 PB2
  DDRB |= _BV(PB1) | _BV(PB2);

  // set prescaler and start the timer
  TCCR1B |= _BV(CS11); // 8 (500ns per tick)
}

ISR(TIMER1_OVF_vect)  // timer1 overflow interrupt
{
  timer_1_ovf_++;

  uint16_t pwm;
  pwm = (pwm_[0] << 1);
  OCR1AH = (uint8_t)(pwm >> 8);
  OCR1AL = (uint8_t)(pwm & 0xff);

  pwm = (pwm_[1] << 1);
  OCR1BH = (uint8_t)(pwm >> 8);
  OCR1BL = (uint8_t)(pwm & 0xff);
}

void setup_timer2()
{
  // Set the Timer Mode to normal
  TCCR2A = 0;

  // Set the value that you want to count to
  OCR2A = 0xF9; // 249

  // enable ISR
  TIMSK2 |= _BV(TOIE2) | _BV(OCIE2A);

  // set prescaler and start the timer
  TCCR2B |= _BV(CS21) | _BV(CS20); // 32 (2us per tick)
}

ISR(TIMER2_OVF_vect)  // timer2 overflow interrupt
{
  timer_2_ovf_++;
}

ISR(TIMER2_COMPA_vect)  // timer2 compa interrupt
{
  timer_2_500us_++;
  OCR2A += 0xFA; // ++ 250
}

void setup_timers()
{
  setup_timer0();
  setup_timer1();
  setup_timer2();
}

uint16_t ticks_ms()
{
  uint32_t ta;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    ta = timer_2_500us_;
  }
  return (uint16_t)(ta >> 1);
}

uint32_t ticks_us()
{
  uint32_t ticks;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    ticks = tcnt2_us_();
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

