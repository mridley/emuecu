#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>

#include "inputs.h"
#include "timers.h"

volatile uint16_t pwm_val_ = 0;
volatile uint16_t pwm_period_ = 0;

volatile uint8_t adc_complete = 0;
volatile uint16_t adc[8];
// we only use 1 channel
const uint8_t num_adc_chan = 1;

void start_adc();

int16_t analogue(uint8_t ch)
{
  if (!adc_complete)
    return -1;
  if (ch >= num_adc_chan)
    return -1;
  return adc[ch&0x7];
}

ISR(INT0_vect)
{
  // crank interrupt
  start_adc();
}

ISR(INT1_vect)
{
  static uint8_t rise = 0;
  static uint16_t rise_t_ = 0;

  uint16_t now_1us = (uint16_t)tcnt1_us_();

  if (PIND & _BV(PD3))
  {
    if (rise)
    {
      pwm_period_ = (now_1us - rise_t_);
    }
    rise = 1;
    rise_t_ = now_1us;
  }
  else if (rise)
  {
    pwm_val_ = (now_1us - rise_t_);
  }
}

ISR(ADC_vect)
{
  uint8_t ch = ADMUX & 0x07;
  adc[ch] = (ADCH << 8) | ADCL;

  if (ch < num_adc_chan - 1)
  {
    ADMUX = ADMUX++;
    ADCSRA |= _BV(ADSC); // start next conversion
  }
  else
  {
    adc_complete = 1;
  }
}

void setup_int0()
{
  DDRD  |= _BV(PD2);  // Set PD2 as input (Using for interupt INT0)
  PORTD |= _BV(PD2);  // enable internal pullup

  EIMSK |= _BV(INT1); // Enable INT0

  EICRA |= 0<<ISC01 | 1<<ISC00;	// Trigger INT0 on any edge
}

void setup_int1()
{
  DDRD  |= _BV(PD3); // Set PD3 as input (Using for interupt INT1)
  PORTD |= _BV(PD3); // enable internal pullup

  EIMSK |= _BV(INT1); // Enable INT1

  EICRA |= 0<<ISC11 | 1<<ISC10;	// Trigger INT1 on any edge
}

void setup_adc()
{
  // set PC5 as input, for ADC5
  DDRC |= _BV(PC5);
  DIDR0 |= _BV(ADC5D);

  // select AVcc Refrence
  ADMUX |= _BV(REFS0);

  // enable adc, enable interrupt , 128 prescalar
  ADCSRA |= _BV(ADEN) | _BV(ADIE) | 0x07;

  adc_complete = 0;
}

void start_adc()
{
  // clear bottom of mux
  ADMUX = (ADMUX  & 0xF0); // select ADC0
  adc_complete = 0;
  ADCSRA |= _BV(ADSC); // start conversion
}

uint16_t pwm_input()
{
  uint16_t pwm_val;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    pwm_val = pwm_val_;
  }
  return pwm_val;
}

void setup_inputs()
{
  setup_int0();
  setup_int1();
  //setup_adc();
}
