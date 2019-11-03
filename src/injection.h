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

// default map fudge factors
// fudge factor to account for anything else
#define FUDGE_MULT (1.0f)
// to account to air leakage for zero throttle (us)
#define FUDGE_ADD (500)

// CHT cold enrichment threshold 50 deg
#define CHT_COLD_ENRICH_T (5000)
// CHT hot enrichment threshold 100 deg
#define CHT_HOT_ENRICH_T (10000)

// enrichment per deg
#define CHT_ENRICH_RATE (0.005f)
#define CHT_ENRICH_MAXFACTOR (1.5f)
#define CRANKING_ENRICH_FACTOR (1.5f)

#define US2TICKS(a)    ((a) >> 4)
#define TICKS2US(a)    ((a) << 4)

#define INJ_TICKS_MAX  (8*255U)
#define INJ_TIME_MAX   TICKS2US(INJ_TICKS_MAX)

#define MAP_RPM_BITS_PER_COL   (11)

#define MAP_MAX_RPM ((MAP_COLS-1)<<MAP_RPM_BITS_PER_COL)

float inj_corrections(uint32_t baro, int16_t iat, int16_t cht, bool cranking);
void inj_map_update_row(float throttle, float pt_c);
void inj_map_default(void);
void inj_map_dump(void);

// called from isr
// returns injection time on 16us ticks
inj_ticks_t inj_ticks_(uint16_t rpm);

#endif
