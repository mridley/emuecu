#ifndef _RPM_H_
#define _RPM_H_

#include <stdint.h>

#define CRANK_TIMEOUT_USECS  100000UL

uint16_t rpm_from_us(uint32_t ticks_us);

#endif
