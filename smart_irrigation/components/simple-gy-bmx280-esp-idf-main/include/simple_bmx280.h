#ifndef SIMPLE_BMX280_H
#define SIMPLE_BMX280_H

#include <stdint.h>
#include "esp_err.h"

typedef struct {
    int i2c_port;    // I2C port number (e.g., I2C_NUM_0)
    int sda_pin;     // GPIO pin for SDA
    int scl_pin;     // GPIO pin for SCL
    uint8_t addr;    // I2C address (e.g., 0x76 or 0x77)
} bmp280_config_t;

esp_err_t bmp280_init(const bmp280_config_t *config);
esp_err_t bmp280_read_data(float *temp, float *press);

#endif // SIMPLE_BMX280_H