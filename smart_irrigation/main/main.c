// #include <stdio.h>
// #include "esp_log.h"
// #include "wifi_simple.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_event.h"
// #include "mqtt_client.h"
// #include "string.h"
// #include "cJSON.h"
// #include "simple_bmx280.h"
// #include "driver/i2c.h"
// #include "driver/adc.h"
// #include "driver/gpio.h"

// static const char *TAG = "esp32_demo_mqtt";
// static const char *TAG_MQTT = "MQTT";
// #define RELAY_GPIO GPIO_NUM_12

// uint8_t MQTT_CONNECTED = 0;
// uint8_t QoS = 1;
// char topic[64] = "learn_iot_khmer/testing_sending_data";
// esp_mqtt_client_handle_t client;

// void mqtt_event_handler(void* event_handler_arg,
//                         esp_event_base_t event_base,
//                         int32_t event_id,
//                         void* event_data) {
//     esp_mqtt_event_handle_t event = event_data;

//     switch ((esp_mqtt_event_id_t)event_id) {
//         case MQTT_EVENT_CONNECTED:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
//             MQTT_CONNECTED = 1;
//             break;

//         case MQTT_EVENT_DISCONNECTED:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
//             MQTT_CONNECTED = 0;
//             break;

//         case MQTT_EVENT_SUBSCRIBED:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED");
//             break;

//         case MQTT_EVENT_UNSUBSCRIBED:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED");
//             break;

//         case MQTT_EVENT_PUBLISHED:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED");
//             break;

//         case MQTT_EVENT_DATA:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
//             printf("Topic: %.*s\n", event->topic_len, event->topic);
//             printf("Message: %.*s\n", event->data_len, event->data);
//             break; // 🛠️ this was missing

//         case MQTT_EVENT_ERROR:
//             ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
//             break;

//         default:
//             break;
//     }
// }

// int mqtt_send(const char *topic, const char *payload) {
//     return esp_mqtt_client_publish(client, topic, payload, strlen(payload), QoS, 0);
// }

// int map(int x, int in_min, int in_max, int out_min, int out_max) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }


// void publish_task(void *param) {
//     bmp280_config_t config = {
//         .i2c_port = I2C_NUM_0,
//         .sda_pin = 22,
//         .scl_pin = 21,
//         .addr = 0x76
//     };

//     if (bmp280_init(&config) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to initialize BMP280");
//         vTaskDelete(NULL);
//         return;
//     }

//     // ADC & Relay Config
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // GPIO36

//     gpio_reset_pin(RELAY_GPIO);
//     gpio_set_direction(RELAY_GPIO, GPIO_MODE_INPUT_OUTPUT);
//     gpio_set_level(RELAY_GPIO, 0);  // Initial pump state OFF

//     while (1) {
//         if (MQTT_CONNECTED) {
//             float temp;
//             float press;
//             int adc_val = adc1_get_raw(ADC1_CHANNEL_0);
//             int soil_value;
//             soil_value = map(adc_val,0,4095, 100, 0);
//             // printf(" ADC: %d, Soil: %d\n", adc_val, soil_value);

//             if (bmp280_read_data(&temp, &press) == ESP_OK) {
//                 ESP_LOGI(TAG, "Temp: %.2f°C, Pressure: %.2f hPa, Soil: %d", temp, press, soil_value);

//                 // Pump logic
//                 if (soil_value < 35) {
//                     gpio_set_level(RELAY_GPIO, 1);
//                     ESP_LOGI(TAG, "Soil dry → Pump ON");
//                 } else {
//                     gpio_set_level(RELAY_GPIO, 0);
//                     ESP_LOGI(TAG, "Soil moist → Pump OFF");
//                 }

//                 // Prepare and send payload
//                 char payload[200];
//                 snprintf(payload, sizeof(payload),
//                          "{\"temperature\":%.2f,\"pressure\":%.2f,\"soil\":%d,\"pump\":%d}",
//                          temp, press, soil_value, gpio_get_level(RELAY_GPIO));

//                 mqtt_send(topic, payload);
//             }

//             vTaskDelay(pdMS_TO_TICKS(3000));
//         } else {
//             ESP_LOGI(TAG, "Waiting for MQTT...");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//     }
// }

// void app_main(void) {
//     // Connect to WiFi
//     wifi_init_simple(WIFI_MODE_STA, "bunleab", "12345678");
//     wifi_connect();

//     vTaskDelay(pdMS_TO_TICKS(3000));
//     if (wifi_is_connected()) {
//         printf("Successfully connected to WiFi!\n");
//     } else {
//         printf("Failed to connect to WiFi\n");
//     }

//     // MQTT client configuration
//     esp_mqtt_client_config_t mqtt_cfg = {
//         .broker.address.uri = "mqtt://test.mosquitto.org:1883",
//     };

//     client = esp_mqtt_client_init(&mqtt_cfg);
//     esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
//     esp_mqtt_client_start(client);

//     // Start telemetry task
//     xTaskCreate(publish_task, "publish_task", 1024 * 5, NULL, 5, NULL);
// }


                //project_smart irrigation system using  IOT


#include <stdio.h>
#include "esp_log.h"
#include "wifi_simple.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "string.h"
#include "cJSON.h"
#include "simple_bmx280.h"
#include "driver/i2c.h"
#include "driver/adc.h"
#include "driver/gpio.h"

static const char *TAG = "esp32_demo_mqtt";
static const char *TAG_MQTT = "MQTT";
#define RELAY_GPIO GPIO_NUM_12

uint8_t MQTT_CONNECTED = 0;
uint8_t QoS = 1;
char topic[64] = "v1/devices/me/telemetry";
esp_mqtt_client_handle_t client;

void mqtt_event_handler(void* event_handler_arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data) {
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
            MQTT_CONNECTED = 1;
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
            MQTT_CONNECTED = 0;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED");
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED");
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED");
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
            printf("Topic: %.*s\n", event->topic_len, event->topic);
            printf("Message: %.*s\n", event->data_len, event->data);
            break; // 🛠 this was missing

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
            break;

        default:
            break;
    }
}

int mqtt_send(const char *topic, const char *payload) {
    return esp_mqtt_client_publish(client, topic, payload, strlen(payload), QoS, 0);
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void publish_task(void *param) {
    bmp280_config_t config = {
        .i2c_port = I2C_NUM_0,
        .sda_pin = 22,
        .scl_pin = 21,
        .addr = 0x76
    };

    if (bmp280_init(&config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize BMP280");
        vTaskDelete(NULL);
        return;
    }

    // ADC & Relay Config
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // GPIO36

    gpio_reset_pin(RELAY_GPIO);
    gpio_set_direction(RELAY_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(RELAY_GPIO, 0);  // Initial pump state OFF

    while (1) {
        if (MQTT_CONNECTED) {
            float temp;
            float press;
            int adc_val = adc1_get_raw(ADC1_CHANNEL_0);
            int soil_value;
            soil_value = map(adc_val,0,4095, 100, 0);
            // printf(" ADC: %d, Soil: %d\n", adc_val, soil_value);

            if (bmp280_read_data(&temp, &press) == ESP_OK) {
                ESP_LOGI(TAG, "Temp: %.2f°C, Pressure: %.2f hPa, Soil: %d", temp, press, soil_value);

                // Pump logic
                if (soil_value < 35) {
                    gpio_set_level(RELAY_GPIO, 1);
                    ESP_LOGI(TAG, "Soil dry → Pump ON");
                } else {
                    gpio_set_level(RELAY_GPIO, 0);
                    ESP_LOGI(TAG, "Soil moist → Pump OFF");
                }

                // Prepare and send payload
                char payload[200];
                snprintf(payload, sizeof(payload),
                         "{\"temperature\":%.2f,\"pressure\":%.2f,\"soil\":%d,\"pump\":%d}",
                         temp, press, soil_value, gpio_get_level(RELAY_GPIO));

                mqtt_send(topic, payload);
            }
            vTaskDelay(pdMS_TO_TICKS(3000));
        } else {
            ESP_LOGI(TAG, "Waiting for MQTT...");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void app_main(void) {
    // Connect to WiFi
    wifi_init_simple(WIFI_MODE_STA, "bunleab", "12345678");
    wifi_connect();

    vTaskDelay(pdMS_TO_TICKS(3000));
    if (wifi_is_connected()) {
        printf("Successfully connected to WiFi!\n");
    } else {
        printf("Failed to connect to WiFi\n");
    }

    // MQTT client configuration
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://6J1W2a94OcngSP0WDIip:NULL@demo.thingsboard.io:1883",
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    // Start telemetry task
    xTaskCreate(publish_task, "publish_task", 1024 * 5, NULL, 5, NULL);
}


