#ifndef _INJECTION_H_
#define _INJECTION_H_

#include <stdint.h>
#include "config.h"

// Air density at 15 deg C at sea level
#define AIR_DENSITY_15C_SL (0.001225f)

// Air fuel ratio
#define AFR (14.7f)

// Volumetric efficiency (nominal)
#define VE_NOM (0.55f)
// Volumetric efficiency (max at low rpm ?)
#define VE_MAX (0.80f)

#define US2TICKS(a)    ((a) >> 4)
#define TICKS2US(a)    ((a) << 4)
#define INJ_TIME_MAX   (255 << 4)

#define MAP_RPM_BITS_PER_COL   (11)

#define MAP_MAX_RPM ((MAP_COLS-1)<<MAP_RPM_BITS_PER_COL)

void inj_map_update_row(float throttle);
void inj_map_default(void);
void inj_map_dump(void);

// called from isr
// returns injection time on 16us ticks
uint8_t inj_ticks_(uint16_t rpm);

#endif
