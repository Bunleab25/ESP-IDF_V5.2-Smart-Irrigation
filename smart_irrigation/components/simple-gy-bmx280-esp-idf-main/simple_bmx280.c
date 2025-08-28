#include "simple_bmx280.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"

// Register addresses
#define BMP280_REG_ID 0xD0
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_STATUS 0xF3
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_DATA 0xF7
#define BMP280_REG_CALIB 0x88

static const char *TAG = "BMP280";

// Calibration data structure
typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
} bmp280_calib_t;

static bmp280_calib_t calib_data;
static int32_t t_fine; // Global variable for temperature compensation

static uint8_t bmp280_read_reg(uint8_t reg, i2c_port_t i2c_port, uint8_t addr) {
    uint8_t value;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &value, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read register 0x%02X: %s", reg, esp_err_to_name(ret));
        return 0;
    }
    return value;
}

static void bmp280_write_reg(uint8_t reg, uint8_t value, i2c_port_t i2c_port, uint8_t addr) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, value, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write register 0x%02X: %s", reg, esp_err_to_name(ret));
    }
}

static void bmp280_read_calib(i2c_port_t i2c_port, uint8_t addr) {
    uint8_t calib[24];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, BMP280_REG_CALIB, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, calib, 24, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read calibration data: %s", esp_err_to_name(ret));
        return;
    }

    // Parse calibration data
    calib_data.dig_T1 = (calib[1] << 8) | calib[0];
    calib_data.dig_T2 = (calib[3] << 8) | calib[2];
    calib_data.dig_T3 = (calib[5] << 8) | calib[4];
    calib_data.dig_P1 = (calib[7] << 8) | calib[6];
    calib_data.dig_P2 = (calib[9] << 8) | calib[8];
    calib_data.dig_P3 = (calib[11] << 8) | calib[10];
    calib_data.dig_P4 = (calib[13] << 8) | calib[12];
    calib_data.dig_P5 = (calib[15] << 8) | calib[14];
    calib_data.dig_P6 = (calib[17] << 8) | calib[16];
    calib_data.dig_P7 = (calib[19] << 8) | calib[18];
    calib_data.dig_P8 = (calib[21] << 8) | calib[20];
    calib_data.dig_P9 = (calib[23] << 8) | calib[22];
}

esp_err_t bmp280_init(const bmp280_config_t *config) {
    if (config == NULL) {
        ESP_LOGE(TAG, "Null config pointer passed to bmp280_init");
        return ESP_ERR_INVALID_ARG;
    }

    // Initialize I2C
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = config->sda_pin,
        .scl_io_num = config->scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    esp_err_t ret = i2c_param_config(config->i2c_port, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C config failed: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = i2c_driver_install(config->i2c_port, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(ret));
        return ret;
    }

    // Verify chip ID (should be 0x58 for BMP280)
    uint8_t id = bmp280_read_reg(BMP280_REG_ID, config->i2c_port, config->addr);
    ESP_LOGI(TAG, "Chip ID: 0x%02X", id);
    if (id != 0x58) {
        ESP_LOGE(TAG, "Not a BMP280! Check your sensor.");
        return ESP_ERR_NOT_FOUND;
    }

    // Soft reset
    bmp280_write_reg(BMP280_REG_RESET, 0xB6, config->i2c_port, config->addr);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Read calibration data
    bmp280_read_calib(config->i2c_port, config->addr);
    ESP_LOGI(TAG, "Calibration data loaded");

    return ESP_OK;
}

esp_err_t bmp280_read_data(float *temp, float *press) {
    if (temp == NULL || press == NULL) {
        ESP_LOGE(TAG, "Null pointer passed to bmp280_read_data");
        return ESP_ERR_INVALID_ARG;
    }

    // Use the last initialized config (assuming init was called with valid config)
    static bmp280_config_t last_config = {I2C_NUM_0, 21, 22, 0x76}; // Default values
    uint8_t data[6];
    
    // Trigger measurement
    bmp280_write_reg(BMP280_REG_CTRL_MEAS, 0x25, last_config.i2c_port, last_config.addr);  // Temp/Press oversampling x1, forced mode
    
    // Wait for measurement to complete
    while (bmp280_read_reg(BMP280_REG_STATUS, last_config.i2c_port, last_config.addr) & 0x08);
    
    // Read all data
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (last_config.addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, BMP280_REG_DATA, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (last_config.addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 6, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(last_config.i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read data: %s", esp_err_to_name(ret));
        return ret;
    }
    
    // Combine raw data
    int32_t adc_p = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t adc_t = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);

    // Temperature compensation
    int32_t var1, var2;
    var1 = ((((adc_t >> 3) - ((int32_t)calib_data.dig_T1 << 1))) * ((int32_t)calib_data.dig_T2)) >> 11;
    var2 = (((((adc_t >> 4) - ((int32_t)calib_data.dig_T1)) * ((adc_t >> 4) - ((int32_t)calib_data.dig_T1))) >> 12) * ((int32_t)calib_data.dig_T3)) >> 14;
    t_fine = var1 + var2;
    *temp = ((t_fine * 5 + 128) >> 8) / 100.0f;

    // Pressure compensation
    int64_t var1_p, var2_p, p;
    var1_p = ((int64_t)t_fine) - 128000;
    var2_p = var1_p * var1_p * (int64_t)calib_data.dig_P6;
    var2_p = var2_p + ((var1_p * (int64_t)calib_data.dig_P5) << 17);
    var2_p = var2_p + (((int64_t)calib_data.dig_P4) << 35);
    var1_p = ((var1_p * var1_p * (int64_t)calib_data.dig_P3) >> 8) + ((var1_p * (int64_t)calib_data.dig_P2) << 12);
    var1_p = (((((int64_t)1) << 47) + var1_p)) * ((int64_t)calib_data.dig_P1) >> 33;
    if (var1_p == 0) {
        *press = 0; // Avoid division by zero
        return ESP_ERR_INVALID_STATE;
    }
    p = 1048576 - adc_p;
    p = (((p << 31) - var2_p) * 3125) / var1_p;
    var1_p = (((int64_t)calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2_p = (((int64_t)calib_data.dig_P8) * p) >> 19;
    p = ((p + var1_p + var2_p) >> 8) + (((int64_t)calib_data.dig_P7) << 4);
    *press = (float)p / 256.0f / 100.0f; // Convert to hPa

    return ESP_OK;
}