#include <stdint.h>
#include "bme280_defs.h"
#include "i2cmaster.h"

struct bme280_calib_data cd;
struct bme280_uncomp_data ud;
struct bme280_data pth_data;

int32_t BME280_compensate_T_int32(int32_t adc_T);
uint32_t BME280_compensate_P_int32(int32_t adc_P);
uint16_t bme280_compensate_H_int32(int32_t adc_H);

uint8_t bme_probe()
{
   uint8_t rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
   if (0 == rc) {
     rc = i2c_write(BME280_CHIP_ID_ADDR);
     if (0 == rc) {
       rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_READ);
       if (0 == rc) {
         uint8_t id = i2c_readNak();
         i2c_stop();
         return id;
       }
     }
   }
   return 0;
}

uint8_t bme_read_calib_data()
{
  uint8_t rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
  if (0 == rc) {
    rc = i2c_write(BME280_TEMP_CALIB_DATA_ADDR);
    if (0 == rc) {
      rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_READ);
      if (0 == rc) {
        cd.dig_T1 = (uint16_t)i2c_readAck();
        cd.dig_T1 |= (uint16_t)i2c_readAck() << 8;
        cd.dig_T2 = (uint16_t)i2c_readAck();
        cd.dig_T2 |= (int16_t)i2c_readAck() << 8;
        cd.dig_T3 = (uint16_t)i2c_readAck();
        cd.dig_T3 |= (int16_t)i2c_readNak() << 8;
        i2c_stop();
      }
    }
  }
  if (0 == rc) {
    rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
    if (0 == rc) {
      rc = i2c_write(BME280_PRESS_CALIB_DATA_ADDR);
      if (0 == rc) {
        rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_READ);
        if (0 == rc) {
          cd.dig_P1 = (uint16_t)i2c_readAck();
          cd.dig_P1 |= (uint16_t)i2c_readAck() << 8;
          cd.dig_P2 = (int16_t)i2c_readAck();
          cd.dig_P2 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P3 = (int16_t)i2c_readAck();
          cd.dig_P3 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P4 = (int16_t)i2c_readAck();
          cd.dig_P4 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P5 = (int16_t)i2c_readAck();
          cd.dig_P5 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P6 = (int16_t)i2c_readAck();
          cd.dig_P6 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P7 = (int16_t)i2c_readAck();
          cd.dig_P7 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P8 = (int16_t)i2c_readAck();
          cd.dig_P8 |= (int16_t)i2c_readAck() << 8;
          cd.dig_P9 = (int16_t)i2c_readAck();
          cd.dig_P9 |= (int16_t)i2c_readAck() << 8;
          cd.dig_H1 = (uint8_t)i2c_readNak();
          i2c_stop();
        }
      }
    }
  }
  if (0 == rc) {
    uint8_t rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
    if (0 == rc) {
      rc = i2c_write(BME280_HUMIDITY_CALIB_DATA_ADDR);
      if (0 == rc) {
        rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_READ);
        if (0 == rc) {
          cd.dig_H2 = (int16_t)i2c_readAck();       // E1
          cd.dig_H2 |= (int16_t)i2c_readAck() << 8; // E2
          cd.dig_H3 = (uint8_t)i2c_readAck();       // E3
          uint8_t E4 = i2c_readAck();
          uint8_t E5 = i2c_readAck();
          uint8_t E6 = i2c_readAck();
          cd.dig_H4 = (int16_t)(int8_t)E4 << 4 | ((int16_t)E5 & 0x0f);
          cd.dig_H5 = (int16_t)(int8_t)E6 << 4 | ((int16_t)E5 >> 4);
          cd.dig_H6 = (int8_t)i2c_readNak();        // E7
          i2c_stop();
        }
      }
    }
  }
  return rc;
}

uint8_t bme_start_conversion()
{
  uint8_t rc = 0;
  if (0 == rc) {
    rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
    if (0 == rc) {
      rc = i2c_write(BME280_CTRL_HUM_ADDR);
      if (0 == rc) {
        rc = i2c_write(1); // 1x
        i2c_stop();
      }
    }
  }
  if (0 == rc) {
    i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
    if (0 == rc) {
      rc = i2c_write(BME280_PWR_CTRL_ADDR);
      if (0 == rc) {
        rc = i2c_write(1<<5|1<<2|BME280_FORCED_MODE); // 1x
        i2c_stop();
      }
    }
  }
  return rc;
}

uint8_t bme_read_data()
{
  uint8_t rc = 0;
  if (0 == rc) {
    rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_WRITE);
    if (0 == rc) {
      rc = i2c_write(BME280_DATA_ADDR);
      if (0 == rc) {
        rc = i2c_start((BME280_I2C_ADDR_PRIM << 1) + I2C_READ);
        if (0 == rc) {
          ud.pressure = (uint32_t)i2c_readAck() << 12;
          ud.pressure |= (uint32_t)i2c_readAck() << 4;
          ud.pressure |= (uint32_t)i2c_readAck() >> 4;
          ud.temperature = (uint32_t)i2c_readAck() << 12;
          ud.temperature |= (uint32_t)i2c_readAck() << 4;
          ud.temperature |= (uint32_t)i2c_readAck() >> 4;
          ud.humidity = (uint16_t)i2c_readAck() << 8;
          ud.humidity |= (uint16_t)i2c_readNak();
          i2c_stop();
        }
      }
    }
  }
  return rc;
}

uint32_t bme_baro()
{
  return BME280_compensate_P_int32(ud.pressure);
}

uint16_t bme_humidity()
{
  return (uint16_t)bme280_compensate_H_int32(ud.humidity);
}

uint16_t bme_temp()
{
  return (uint16_t)BME280_compensate_T_int32(ud.temperature);
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of '5123' equals 51.23 DegC.
// t_fine carries fine temperature as global value
int32_t BME280_compensate_T_int32(int32_t adc_T)
{
  int32_t var1, var2, T;
  var1 = ((((adc_T>>3) - ((int32_t)cd.dig_T1<<1))) * ((int32_t)cd.dig_T2)) >> 11;
  var2 = (((((adc_T>>4) - ((int32_t)cd.dig_T1)) * ((adc_T>>4) - ((int32_t)cd.dig_T1))) >> 12) *
          ((int32_t)cd.dig_T3)) >> 14;

  cd.t_fine = var1 + var2;
  T = (cd.t_fine * 5 + 128) >> 8;
  return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of '96386' equals 96386 Pa = 963.86 hPa
uint32_t BME280_compensate_P_int32(int32_t adc_P)
{
  int32_t var1, var2;
  uint32_t p;
  var1 = (((int32_t)cd.t_fine)>>1) - (int32_t)64000;
  var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)cd.dig_P6);
  var2 = var2 + ((var1*((int32_t)cd.dig_P5))<<1);
  var2 = (var2>>2)+(((int32_t)cd.dig_P4)<<16);
  var1 = (((cd.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)cd.dig_P2) * var1)>>1))>>18;
  var1 =((((32768+var1))*((int32_t)cd.dig_P1))>>15);
  if (var1 == 0)
  {
    return 0; // avoid exception caused by division by zero
  }
  p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
  if (p < 0x80000000)
  {
    p = (p << 1) / ((uint32_t)var1);
  }
  else
  {
    p = (p / (uint32_t)var1) * 2;
  }
  var1 = (((int32_t)cd.dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
  var2 = (((int32_t)(p>>2)) * ((int32_t)cd.dig_P8))>>13;
  p = (uint32_t)((int32_t)p + ((var1 + var2 + cd.dig_P7) >> 4));
  return p;
}

// Returns humidity in %RH as unsigned 32 bit integer with resolution 0.01 %
// Output value of 4744 represents 47.44 %RH
uint16_t bme280_compensate_H_int32(int32_t adc_H)
{
  int32_t v_x1_u32r;
  v_x1_u32r = (cd.t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)cd.dig_H4) << 20) - (((int32_t)cd.dig_H5) * v_x1_u32r)) +
                 ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)cd.dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)cd.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
                                                   ((int32_t)cd.dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)cd.dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  return (uint16_t)(((v_x1_u32r>>12)*100)>>10);
}
