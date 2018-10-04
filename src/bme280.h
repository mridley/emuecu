#ifndef BME280_H
#define BME280_H

#include <stdint.h>
#include "bme280_defs.h"

uint8_t bme_probe();
uint8_t bme_read_calib_data();
uint8_t bme_start_conversion();
uint8_t bme_read_data();
void bme_comp_data();

struct bme280_calib_data cd;
struct bme280_uncomp_data ud;
struct bme280_data pth_data;

//uint16_t bme_baro();
//uint16_t bme_humidity();
//uint16_t bme_temp();

#endif // BME280_H
