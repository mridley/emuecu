#ifndef _MAX6675_H_
#define _MAX6675_H_

#include <stdint.h>
#include "config.h"

#define MAX6675_SCK PB4
#define MAX6675_CS  PB3
#define MAX6675_SO  PB5

#define MAX6675_D2  (0x0004)
#define MAX6675_D15 (0x8000)
#define MAX6675_D1  (0x0002)
#define MAX6675_D0  (0x0001)

#define MAX6675_ERR_MISSING (-1)
#define MAX6675_ERR_ID      (-2)
#define MAX6675_ERR_BUS     (-3)
#define MAX6675_ERR_SENSOR  (-4)

void max6675_init();
int32_t max6675_read();

#endif
