#include <avr/io.h>
#include "max6675.h"
#include "timers.h"

void max6675_init()
{
  // Outputs for CS and SCK
  DDRB |= _BV(MAX6675_CS) | _BV(MAX6675_SCK);
  DDRB &= ~_BV(MAX6675_SO); // input for SO
  PORTB |= _BV(MAX6675_SO); // pullup SO
  PORTB |= _BV(MAX6675_CS); // CS high
  PORTB &= ~_BV(MAX6675_SCK); // SCK idle low
}
/*
 * @brief returns temperature in hundreths of a degree i.e. 25.43 deg C returns 2543
 */
int32_t max6675_read()
{
  uint16_t data = 0;
  PORTB &= ~_BV(MAX6675_CS); // CS asserted
  microsleep(100);

  for (uint8_t i =0; i < 16; ++i) {
    PORTB |= _BV(MAX6675_SCK); // SCK high
    microsleep(100);
    data <<= 1;
    data |= (uint16_t)((PINB & _BV(MAX6675_SO)) ? 1 : 0);
    PORTB &= ~_BV(MAX6675_SCK); // SCK low
    microsleep(100);
  }

  PORTB |= _BV(MAX6675_CS); // CS high
  if (data & MAX6675_D15) {
    return MAX6675_ERR_MISSING; // pulled high.. so missing
  }
  if (data & MAX6675_D1) {
    return MAX6675_ERR_ID;  // should be 0
  }
  if (!(data & (MAX6675_D0)) ) {
    return MAX6675_ERR_BUS; // should be tri state and pulled high
  }
  if (data & MAX6675_D2) {
    return MAX6675_ERR_SENSOR;
  }
  //convert to deci degrees
  //return ((data & 0x7fff) >> 3)
  return (int32_t)(25*(uint32_t)((data & 0x7fff) >> 3));

}
