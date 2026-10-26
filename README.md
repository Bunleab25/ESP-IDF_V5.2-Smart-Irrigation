Smart Irrigation System

![Version](https://img.shields.io/badge/VERSION-v1.0.0-2ECC71?style=for-the-badge&labelColor=555555)
![Framework](https://img.shields.io/badge/FRAMEWORK-ESP--IDF_v5.2.5-E67E22?style=for-the-badge&logo=espressif&logoColor=white&labelColor=555555)
![Hardware](https://img.shields.io/badge/HARDWARE-ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white&labelColor=555555)
![Protocol](https://img.shields.io/badge/CLOUD-MQTT_%7C_THINGSBOARD-305680?style=for-the-badge&logo=mqtt&logoColor=white&labelColor=555555)
[![LinkedIn](https://img.shields.io/badge/LINKEDIN-VIEW_DEMO-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white&labelColor=555555)](https://lnkd.in/p/g7FB93Z2)   

An embedded IoT automation project built around the ESP32 using the native ESP-IDF framework (v5.2.5). The system optimizes water usage and reduces manual intervention by automating irrigation cycles from real-time soil and atmospheric telemetry, paired with remote monitoring and control through ThingsBoard.

Overview
Traditional irrigation methods often rely on fixed timers rather than actual environmental conditions, leading to water waste or inconsistent soil hydration. This project implements a closed-loop control system on the ESP32 that continuously evaluates soil dryness alongside local microclimate metrics, publishing real-time telemetry over MQTT and supporting remote manual overrides from a centralized cloud dashboard.
Key Capabilities
Closed-Loop Irrigation Control: Automatically actuates a $5\text{ V}$ water pump via a relay module whenever soil moisture falls below configured thresholds.
Environmental Telemetry: Captures real-time ambient temperature, relative humidity, and atmospheric pressure using a BME280 sensor.
Native ESP-IDF Firmware: Engineered on ESP-IDF v5.2.5 for deterministic FreeRTOS task management, low-level peripheral control, and resilient Wi-Fi networking.
MQTT Communication: Publishes structured telemetry payloads with minimal overhead for responsive cloud synchronization.
Remote Visualization & Override: Integrates with ThingsBoard for live metric visualization, historical data logging, and remote RPC pump actuation.
Hardware Specifications
Component
Interface
Functional Role
ESP32 Microcontroller
SoC / Wi-Fi
Executes ESP-IDF v5.2.5 firmware, sensor polling, control logic, and MQTT client tasks.
Soil Moisture Sensor
ADC / GPIO
Measures volumetric soil dryness to trigger or terminate watering cycles.
BME280 Environmental Sensor
$\text{I}^2\text{C}$ / SPI
Monitors ambient temperature, relative humidity, and barometric pressure.
Single-Channel Relay Module
Digital GPIO
Provides electrical isolation and switching control for the water pump circuit.
Mini $5\text{ V}$ Submersible Pump
Relay Switched
Delivers water to the soil during automated or manually triggered irrigation events.

Software & Cloud Architecture
Firmware Framework (ESP-IDF v5.2.5): Manages hardware drivers, non-volatile storage (NVS), Wi-Fi station reconnection routines, and concurrent FreeRTOS tasks.
Messaging Protocol (MQTT): Facilitates asynchronous publish/subscribe data exchange between the ESP32 node and the cloud server.
IoT Platform (ThingsBoard): Hosts the remote dashboard for real-time telemetry widgets, time-series charts, and bidirectional device control.
Operational Workflow
Sensor Acquisition: Dedicated firmware tasks sample soil moisture levels and query the BME280 sensor at defined intervals.
Threshold Evaluation: Control logic compares current soil moisture readings against target setpoints. When dry conditions are detected, the ESP32 drives the relay GPIO to engage the $5\text{ V}$ water pump.
Data Transmission: Environmental metrics and actuator status states are serialized into JSON payloads and published to the MQTT broker.
Cloud Supervision: ThingsBoard ingests incoming MQTT messages to update dashboard indicators and dispatches remote RPC commands when manual pump control is requested by the operator.
Getting Started
Prerequisites
ESP-IDF v5.2.5 installed and exported in your terminal environment.
ThingsBoard instance (Cloud or Community Edition) with a provisioned device access token.
ESP32 development board, peripheral sensors, single-channel relay, and $5\text{ V}$ pump.
Build & Flash
Clone the repository:
git clone https://github.com/your-username/smart-irrigation-esp32-idf.git
cd smart-irrigation-esp32-idf
Configure network and MQTT broker credentials:
idf.py menuconfig
Compile the firmware:
idf.py build
Flash to the ESP32 and attach the serial monitor:
idf.py -p PORT flash monitor
(Replace PORT with your serial interface, such as COM3 on Windows or /dev/ttyUSB0 on Linux).

Project Demonstration
For hardware photos, dashboard previews, and a demonstration of the system in operation, view the project showcase on LinkedIn.
