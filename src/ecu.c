#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#include "uart.h"

volatile uint16_t timer_1ms_;
volatile uint16_t pwm_val_;
//volatile uint16_t pwm_period_;

volatile uint8_t adc_complete;
volatile uint16_t adc[8];

void start_adc();

ISR(INT0_vect)
{
  // crank interrupt
  start_adc();
}

ISR(INT1_vect)
{
  static uint8_t rise = 0;
  static uint16_t rise_t_ = 0;
  
  uint16_t now_4us = TCNT0;
  uint16_t now_1us = (uint16_t)((uint32_t)timer_1ms_*1000 + (now_4us << 2));
  
  if (PIND & _BV(PD3))
  {
    //if (rise)
    //{
    //  pwm_period_ = (now_1us - rise_t_); 
    //} 
    rise = 1;
    rise_t_ = now_1us;
  }
  else if (rise)
  {
    pwm_val_ = (now_1us - rise_t_);
  }
}

ISR(TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
  // event to be executed every 1ms here
  timer_1ms_++;
}

ISR(ADC_vect)
{
  uint8_t ch = ADMUX & 0x0f;
  adc[ch] = (ADCH << 8) | ADCL;

  if (ch > 5)
  {
    ADMUX = ADMUX--;
    ADCSRA |= _BV(ADSC); // start conversion
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

void setup_timer0()
{
  // Set the Timer Mode to CTC
  TCCR0A |= _BV(WGM01);

  // Set the value that you want to count to
  OCR0A = 0xFA;

  //Set the ISR COMPA vect
  TIMSK0 |= _BV(OCIE0A);

  // set prescaler and start the timer
  TCCR0B |= _BV(CS01) | _BV(CS00); //64
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
  ADMUX = (ADMUX  & 0xF0) | 0x07; // select ADC7
  adc_complete = 0;
  ADCSRA |= _BV(ADSC); // start conversion
}

uint16_t ticks_ms()
{
  uint16_t ta = timer_1ms_;
  uint16_t tb;
  while((tb = timer_1ms_) != ta)
  {
    ta = tb;
  }
  return ta;
}

uint32_t ticks_us()
{
  uint16_t ta = timer_1ms_;
  uint16_t tc;
  while (1)
  {
    tc = TCNT0;
    uint16_t tb = timer_1ms_;
    if (tb != ta)
    {
      ta = tb;
      continue;
    }
    break;
  }
  return (uint32_t)ta*1000 + (tc << 2);
}

void sleep(int ms)
{
  uint16_t t0 = ticks_ms();
  while((ticks_ms() - t0) <  ms)
  {
    sleep_cpu();
  }
}

int main(void)
{
  uart0_init();
  setup_timer0();
  setup_int0();
  setup_int1();
  
  printf("EMU ECU\n");
 
  sei(); // Enable Global Interrupt

  while (1) {
  
    uint32_t ticks = ticks_us();
    //sleep(500);
    _delay_ms(1000);
    printf("pwm_val=%u ticks=%lu\n", pwm_val_, ticks);
  }  
}

    
