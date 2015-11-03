#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdio.h>


#include "inputs.h"
#include "timers.h"

volatile uint16_t pwm_val_ = 0;
volatile uint16_t pwm_period_ = 0;

// we only use 1 channel
const uint8_t     adc_num_chan_ = 1;
volatile uint8_t  adc_complete_ = 0;
volatile uint16_t adc[8];

// RPM calcs
volatile uint32_t ticks_per_rev_us_ = 0;
volatile uint32_t crank_ticks_us_ = 0;
volatile uint8_t  rpm_uptodate_ = 0;
int16_t           current_rpm_ = 0;
const uint32_t    CRANK_TIMEOUT_USECS = 100000UL;

// Ignition passthrough
uint8_t           ignition_enabled_ = 0;

void start_adc();

int16_t analogue(uint8_t ch)
{
  int16_t v = -1;
  while(!adc_complete_){}
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    if ( ch < adc_num_chan_ )
    {
      v = adc[ch&0x7];
    }
  }
  return v;
}

uint32_t crank_ticks_us()
{
  uint32_t ct;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    ct = crank_ticks_us_;
  }
  return ct;
}

uint16_t rpm()
{
  uint32_t tpr;
  while (!rpm_uptodate_)
  {
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      tpr = ticks_per_rev_us_;
      rpm_uptodate_ = 1;
    }
    if (tpr > 0)
    {
      current_rpm_ = (uint16_t)(60/(1e-6*tpr));
    }
  }
  uint32_t ct = crank_ticks_us();
  uint32_t ticks = ticks_us();
  int32_t dt = ticks - ct;
  if (dt > CRANK_TIMEOUT_USECS)
  {
    current_rpm_ = 0;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      ticks_per_rev_us_ = 0;
    }
  }
  return current_rpm_;
}

uint8_t ignition_enabled()
{
  return ignition_enabled_;
}

void ignition_enable()
{
  ignition_enabled_ = 1;
}

void ignition_disable()
{
  ignition_enabled_ = 0;
  PORTD |= _BV(PD7); // set high
}

// crank interrupt
ISR(INT0_vect)
{
  static uint8_t fall_ = 0;

  uint32_t now_us = tcnt1_us_();

  if (!(PIND & _BV(PD2)))
  {
    if (ignition_enabled_)
    {
      PORTD &= ~_BV(PD7); // follow low
    }
    if (fall_)
    {
       ticks_per_rev_us_ = (now_us - crank_ticks_us_);
    }
    fall_ = 1;
    crank_ticks_us_ = now_us;
    rpm_uptodate_ = 0;
  }
  else if (ignition_enabled_)
  {
    PORTD |= _BV(PD7); // follow high
  }
}

// pwm input
ISR(INT1_vect)
{
  static uint8_t rise_ = 0;
  static uint16_t rise_t_ = 0;

  uint16_t now_1us = (uint16_t)tcnt1_us_();

  if (PIND & _BV(PD3))
  {
    if (rise_)
    {
      pwm_period_ = (now_1us - rise_t_);
    }
    rise_ = 1;
    rise_t_ = now_1us;
  }
  else if (rise_)
  {
    pwm_val_ = (now_1us - rise_t_);
  }
}

ISR(ADC_vect)
{
  uint8_t ch = ADMUX & 0x07;
  adc[ch] = ADCL; // read low byte first
  adc[ch] |= ((int16_t)ADCH << 8);

  if (ch < adc_num_chan_ - 1)
  {
    ADMUX = ADMUX++;
    ADCSRA |= _BV(ADSC); // start next conversion
  }
  else
  {
    adc_complete_ = 1;
  }
}

void setup_int0()
{
  DDRD  &= ~_BV(PD2);  // Set PD2 as input (Using for interupt INT0)
  //PORTD |= _BV(PD2);  // enable internal pullup

  EIMSK |= _BV(INT0); // Enable INT0

  EICRA |= 0<<ISC01 | 1<<ISC00;	// Trigger INT0 on any edge

  DDRD |= _BV(PD7); // SET PD7 as ignition output
}

void setup_int1()
{
  DDRD  &= ~_BV(PD3); // Set PD3 as input (Using for interupt INT1)
  //PORTD |= _BV(PD3); // enable internal pullup

  EIMSK |= _BV(INT1); // Enable INT1

  EICRA |= 0<<ISC11 | 1<<ISC10;	// Trigger INT1 on any edge
}

void setup_adc()
{
  // set PC5 as input, for ADC5
  DDRC = 0x00;  // set all to input
  DIDR0 = 0x3f; // disable all digital inputs

  // select AVcc Refrence
  ADMUX |= _BV(REFS0);

  // enable adc, enable interrupt , 128 prescalar
  ADCSRA |= _BV(ADEN) | _BV(ADIE) | 0x07;
  ADCSRB = 0;

  adc_complete_ = 0;
}

void start_adc()
{
  // clear bottom of mux
  ADMUX = (ADMUX  & 0xF0); // select ADC0
  adc_complete_ = 0;
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
  setup_adc();
}
