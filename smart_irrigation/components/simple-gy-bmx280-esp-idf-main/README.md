# Simple GY-BMx280 ESP-IDF Component

This repository contains an ESP-IDF component for interfacing a GY-BMx280 sensor with an ESP32 microcontroller to measure temperature and atmospheric pressure.

## How to Use

### Prerequisites
- ESP-IDF framework installed (version 4.x or later).
- ESP32 development board (e.g., ESP32-WROOM-32).
- GY-BMx280 sensor module

### Installation
1. Clone or copy the `GY-BMx280` directory into the `components` folder of your ESP-IDF project:
   ```
   mkdir -p components/GY-BMx280
   cp -r GY-BMx280/* components/GY-BMx280/
   ```
2. Ensure your project's `CMakeLists.txt` includes the `components` directory (default behavior in ESP-IDF projects).
3. Update your project's `main/CMakeLists.txt` to require the `GY-BMx280` component:
   ```cmake
   idf_component_register(SRCS "main.c"
                          INCLUDE_DIRS "."
                          REQUIRES GY-BMx280)
   ```

### Configuration
- The GY-BMx280 component is configurable via a `GY-BMx280_config_t` structure, allowing you to specify the I2C port, SDA pin, SCL pin, and I2C address.
- Default values are provided, but you can customize them in your code.

### Example Usage
Add the following code to your `main.c` to initialize and read data from the GY-BMx280 sensor:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "GY-BMx280.h"

static const char *TAG = "APP";

void app_main(void) {
    ESP_LOGI(TAG, "Starting GY-BMx280 application");

    // Configure GY-BMx280 settings
    GY-BMx280_config_t config = {
        .i2c_port = I2C_NUM_0,  // I2C port (e.g., I2C_NUM_0)
        .sda_pin = 22,          // SDA pin (e.g., GPIO 22)
        .scl_pin = 21,          // SCL pin (e.g., GPIO 21)
        .addr = 0x76            // I2C address (0x76 or 0x77, depending on SDO pin)
    };

    // Initialize GY-BMx280
    esp_err_t ret = GY-BMx280_init(&config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize GY-BMx280: %s", esp_err_to_name(ret));
        return;
    }

    // Main loop to read data
    while (1) {
        float temp, press;
        ret = GY-BMx280_read_data(&temp, &press);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Temp: %.1f C, Press: %.1f hPa", temp, press);
        } else {
            ESP_LOGE(TAG, "Failed to read data: %s", esp_err_to_name(ret));
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Read every 2 seconds
    }
}
```

### Building and Flashing
1. Build the project:
   ```
   idf.py build
   ```
2. Flash the firmware to the ESP32:
   ```
   idf.py flash
   ```
3. Monitor the output:
   ```
   idf.py monitor
   ```
   Expected output will show temperature (e.g., ~25°C) and pressure (e.g., ~1013 hPa) readings every 2 seconds.

## Pin Connections
Connect the GY-BMx280 sensor module to the ESP32 as follows:
- **VCC**: Connect to 3.3V on the ESP32 (ensure your module supports 3.3V; some may require 5V).
- **GND**: Connect to GND on the ESP32.
- **SCL**: Connect to the configured SCL pin (default GPIO 21).
- **SDA**: Connect to the configured SDA pin (default GPIO 22).
- **SDO**: Connect to GND for I2C address 0x76, or to VCC for 0x77 (verify with an I2C scanner if unsure).
- **CSB**: Leave unconnected or tie to VCC for I2C mode (not used in SPI mode).

### Notes
- Ensure pull-up resistors (e.g., 4.7kΩ) are present on SDA and SCL lines if not already included on the GY-BMx280 module.
- Adjust the `sda_pin`, `scl_pin`, and `addr` values in the `GY-BMx280_config_t` structure to match your hardware setup if different from the defaults.