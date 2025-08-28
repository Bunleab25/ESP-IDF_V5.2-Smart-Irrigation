#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "simple_bmx280.h"
#include "driver/i2c.h"

static const char *TAG = "APP";

void app_main(void) {
    ESP_LOGI(TAG, "Starting BMP280 application");

    // Configure BMP280 settings
    bmp280_config_t config = {
        .i2c_port = I2C_NUM_0,
        .sda_pin = 22,  // Changed to match your original setup
        .scl_pin = 21,  // Changed to match your original setup
        .addr = 0x76    // Confirmed by your scan
    };

    // Initialize BMP280
    esp_err_t ret = bmp280_init(&config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize BMP280: %s", esp_err_to_name(ret));
        return;
    }

    // Main loop to read data
    while (1) {
        float temp, press;
        ret = bmp280_read_data(&temp, &press);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Temp: %.1f C, Press: %.1f hPa", temp, press);
        } else {
            ESP_LOGE(TAG, "Failed to read data: %s", esp_err_to_name(ret));
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}