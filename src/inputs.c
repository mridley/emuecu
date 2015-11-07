#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdio.h>


#include "inputs.h"
#include "timers.h"

// pwm
volatile uint32_t pwm_ticks_us_ = 0;
volatile uint16_t pwm_val_ = 0;
volatile uint16_t pwm_filtered_val_ = 0;
volatile uint16_t pwm_period_ = 0;
volatile uint8_t  pwm_change_ = 0;

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
const uint32_t    PWM_TIMEOUT_USECS = 100000UL;

// Ignition passthrough
uint8_t           ignition_enabled_ = 0;
#define           IGN_OUT_PORTD PD4
#define           IGN_IN_PORTD  PD2
#define           PWM_IN_PORTD  PD3

// ----- CRANK -----

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
  if (current_rpm_)
  {
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
  PORTD |= _BV(IGN_OUT_PORTD); // set high
}

// crank interrupt
ISR(INT0_vect)
{
  static uint8_t fall_ = 0;

  uint32_t now_us = tcnt2_us_();

  if (!(PIND & _BV(IGN_IN_PORTD)))
  {
    if (ignition_enabled_)
    {
      PORTD &= ~_BV(IGN_OUT_PORTD); // follow low
      do_injection(); // turns on injecter, set up interrupt to turn it off
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
    PORTD |= _BV(IGN_OUT_PORTD); // follow high
  }
}

void setup_int0()
{
  DDRD  &= ~_BV(IGN_IN_PORTD);  // Set PD2 as input (Using for interupt INT0)
  //PORTD |= _BV(IGN_IN_PORTD);  // not required, external pullup

  EIMSK |= _BV(INT0); // Enable INT0

  EICRA |= 0<<ISC01 | 1<<ISC00;	// Trigger INT0 on any edge

  DDRD |= _BV(IGN_OUT_PORTD); // SET IGNITION_PORT as ignition output
}

// ----- PWM -----

inline uint16_t filter_pwm(uint16_t curr, uint16_t sample)
{
  // assumes < 16384
  return (curr + curr + curr + sample) >> 2;
}

// pwm input
ISR(INT1_vect)
{
  static uint8_t  rise_ = 0;
  static uint16_t rise_t_ = 0;

  uint32_t now_1us = tcnt2_us_();

  if (PIND & _BV(PWM_IN_PORTD))
  {
    if (rise_)
    {
      pwm_period_ = ((uint16_t)now_1us - rise_t_);
    }
    rise_ = 1;
    rise_t_ = now_1us;
  }
  else if (rise_)
  {
    pwm_ticks_us_ = now_1us;
    pwm_val_ = ((uint16_t)now_1us - rise_t_);
    pwm_change_ = 1;
    if (pwm_filtered_val_)
    {
      pwm_filtered_val_ = filter_pwm(pwm_filtered_val_, pwm_val_);
    }
    else
    {
      pwm_filtered_val_ = pwm_val_; // first iteration
    }
  }
}

uint32_t pwm_ticks_us()
{
  uint32_t ct;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    ct = pwm_ticks_us_;
  }
  return ct;
}

uint16_t pwm_input()
{
  static uint16_t pwm_val = 0;

  if (pwm_change_)
  {
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      pwm_val = pwm_filtered_val_;
      pwm_change_ = 0;
    }
  }
  else if (pwm_val)
  {
    uint32_t ct = pwm_ticks_us();
    uint32_t ticks = ticks_us();
    int32_t dt = ticks - ct;
    if (dt > PWM_TIMEOUT_USECS)
    {
      pwm_val = pwm_val_ = pwm_filtered_val_ = 0;
    }
  }

  return pwm_val;
}

void setup_int1()
{
  DDRD  &= ~_BV(PWM_IN_PORTD); // Set PD3 as input (Using for interupt INT1)
  PORTD |= _BV(PWM_IN_PORTD); // enable internal pullup (no external pullup)

  EIMSK |= _BV(INT1); // Enable INT1

  EICRA |= 0<<ISC11 | 1<<ISC10;	// Trigger INT1 on any edge
}

// ----- ADC -----

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

void setup_inputs()
{
  setup_int0();
  setup_int1();
  setup_adc();
}
