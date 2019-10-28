#ifndef _INJECTION_H_
#define _INJECTION_H_

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

// Air fuel ratio
//#define AFR (14.7f)
#define AFR (12.0f)

// Air density at 15 deg C at sea level (g/cc)
#define AIR_DENSITY_15C_SL (0.001225f)

// Mean sea level pressure (Pa)
#define BARO_MSLP_PA (101325UL)

// Reference temp against which we adjust (centi degrees)
#define TEMP_REF_CDEGC (1500L)

#define ZEROC_KELVIN (27315UL)

// Volumetric efficiency (nominal)
#define VE_NOM (0.5f)
// Volumetric efficiency (max at low rpm ?)
#define VE_MAX (1.0f)
// CHT enrichment threshold 50 deg
#define CHT_ENRICH_THRESH (5000)
#define CHT_ENRICH_FACTOR (2.5f)
#define CRANKING_ENRICH_FACTOR (2.0f)

// fudge factor to account for anything else
#define FUDGE_MULT (1.0f)
#define FUDGE_ADD (500)

#define US2TICKS(a)    ((a) >> 4)
#define TICKS2US(a)    ((a) << 4)
#define INJ_TICKS_MAX  (255)
#define INJ_TIME_MAX   (INJ_TICKS_MAX << 4)

#define MAP_RPM_BITS_PER_COL   (11)

#define MAP_MAX_RPM ((MAP_COLS-1)<<MAP_RPM_BITS_PER_COL)

float inj_pt_correction(uint32_t baro, int16_t iat, int16_t cht, bool cranking);
void inj_map_update_row(float throttle, float pt_c);
void inj_map_default(void);
void inj_map_dump(void);

// called from isr
// returns injection time on 16us ticks
uint8_t inj_ticks_(uint16_t rpm);

#endif
