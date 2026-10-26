# Smart Irrigation System

![Version](https://img.shields.io/badge/VERSION-v1.0.0-2ECC71?style=for-the-badge&labelColor=555555)
![Framework](https://img.shields.io/badge/ESP--IDF-v5.2.5-E67E22?style=for-the-badge&logo=espressif&logoColor=white&labelColor=555555)
![Hardware](https://img.shields.io/badge/HARDWARE-ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white&labelColor=555555)
![Protocol](https://img.shields.io/badge/COMMUNICATION-MQTT-305680?style=for-the-badge&logo=mqtt&logoColor=white&labelColor=555555)
![Platform](https://img.shields.io/badge/IoT_PLATFORM-ThingsBoard-1D8FCE?style=for-the-badge&labelColor=555555)

[![LinkedIn Demo](https://img.shields.io/badge/LINKEDIN-VIEW_DEMO-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white&labelColor=555555)](https://lnkd.in/p/g7FB93Z2)

An embedded Internet of Things (IoT) project that automates plant irrigation using an **ESP32** programmed with the native **ESP-IDF v5.2.5** framework. The system monitors soil moisture and environmental conditions, controls a water pump based on configured moisture thresholds, and supports remote monitoring and manual control through MQTT and ThingsBoard.

## Overview

Conventional irrigation systems often depend on fixed schedules rather than actual soil conditions, potentially causing unnecessary water consumption or inconsistent watering.

This project implements a sensor-driven irrigation system on the ESP32. Soil moisture readings inform the pump-control logic, while a BME280 sensor measures ambient temperature, relative humidity, and atmospheric pressure. The device publishes telemetry through MQTT, enabling remote monitoring and pump control through a ThingsBoard dashboard.

## Key Features

- **Automated irrigation:** Controls a water pump through a relay according to configured soil-moisture thresholds.
- **Environmental monitoring:** Measures temperature, relative humidity, and atmospheric pressure using a BME280 sensor.
- **Native embedded firmware:** Developed using ESP-IDF v5.2.5 and FreeRTOS.
- **MQTT communication:** Publishes sensor readings and device status to the IoT platform.
- **Remote monitoring:** Supports real-time visualization and historical telemetry through ThingsBoard.
- **Remote actuator control:** Supports manual pump control through ThingsBoard RPC when configured in the firmware.
- **Wi-Fi connectivity:** Connects the ESP32 to the configured wireless network for cloud communication.

## System Architecture

```text
┌──────────────────────────────┐
│         Sensors              │
│ Soil Moisture + BME280       │
└──────────────┬───────────────┘
               │ Sensor Data
               ▼
┌──────────────────────────────┐
│            ESP32             │
│       ESP-IDF v5.2.5          │
│   FreeRTOS + Control Logic   │
└──────────┬───────────┬───────┘
           │           │
           │           ▼
           │    ┌──────────────┐
           │    │ Relay + Pump │
           │    │  Irrigation  │
           │    └──────────────┘
           │
           │ Wi-Fi / MQTT
           ▼
┌──────────────────────────────┐
│         ThingsBoard          │
│ Dashboard + Telemetry        │
│ Remote Control (RPC)         │
└──────────────────────────────┘
```

## Hardware Components

| Component | Interface | Function |
|---|---|---|
| ESP32 development board | GPIO, ADC, Wi-Fi | Executes firmware, reads sensors, and manages system communication. |
| Soil moisture sensor | ADC / GPIO | Measures soil moisture for irrigation decisions. |
| BME280 sensor | I²C / SPI | Measures temperature, relative humidity, and atmospheric pressure. |
| Single-channel relay module | Digital GPIO | Switches the pump's power circuit. |
| 5 V mini submersible pump | Relay-switched power | Delivers water during irrigation. |
| Appropriate power supply | Power connections | Supplies the components according to their electrical requirements. |

> **Hardware safety:** Verify the pump's voltage and current requirements and the relay module's electrical compatibility. Do not power the pump directly from an ESP32 GPIO. Keep electrical connections protected from water.

## Software and IoT Stack

| Layer | Technology | Responsibility |
|---|---|---|
| Firmware framework | ESP-IDF v5.2.5 | Embedded application development and hardware integration. |
| Real-time operating system | FreeRTOS | Task management and concurrent application execution. |
| Sensor interfaces | ADC, GPIO, I²C / SPI | Sensor acquisition and hardware control. |
| Wireless connectivity | Wi-Fi | Network connectivity. |
| Messaging protocol | MQTT | Telemetry transmission and device communication. |
| IoT platform | ThingsBoard | Dashboard visualization, telemetry storage, and remote interaction. |
| Actuation | Relay + water pump | Physical irrigation control. |

## Operational Workflow

1. **Sensor acquisition:** The ESP32 reads soil moisture and retrieves environmental measurements from the BME280.
2. **Threshold evaluation:** The firmware compares soil moisture readings against a configured threshold.
3. **Pump actuation:** When the irrigation conditions are satisfied, the ESP32 controls the relay to switch the pump.
4. **Telemetry transmission:** Sensor measurements and device status are packaged into messages and published through MQTT.
5. **Cloud visualization:** ThingsBoard receives telemetry and displays current readings and historical trends.
6. **Remote control:** When RPC is implemented and configured, an operator can issue pump-control commands through ThingsBoard.

**Control note:** Soil-moisture thresholds, sensor calibration, sampling intervals, and pump safety limits should be configured for the specific hardware and growing conditions.

## Getting Started

### Prerequisites

- ESP-IDF v5.2.5 installed and configured.
- ESP32 development board.
- Soil moisture sensor and BME280 sensor.
- Relay module and compatible water pump.
- Appropriate power supply and USB data cable.
- ThingsBoard Cloud or a self-hosted ThingsBoard instance.
- Provisioned ThingsBoard device credentials.

### 1. Clone the Repository

Replace the placeholders with your actual GitHub username and repository name.

```bash
git clone https://github.com/<your-username>/<your-repository>.git
cd <your-repository>
```

### 2. Configure the Project

Activate your ESP-IDF environment, then configure the project:

```bash
idf.py set-target esp32
idf.py menuconfig
```

Configure Wi-Fi credentials, MQTT or ThingsBoard connection settings, device credentials, and other project-specific options supported by your firmware.

**Security:** Never commit Wi-Fi passwords, access tokens, private keys, or other secrets to a public repository. Use local configuration and provide example values in documentation.

### 3. Build the Firmware

```bash
idf.py build
```

### 4. Flash and Monitor

Replace `PORT` with your ESP32 serial port.

```bash
idf.py -p PORT flash monitor
```

Example serial ports:

- Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`
- Windows: `COM3`

Press `Ctrl+]` to exit the serial monitor.

### 5. Configure ThingsBoard

1. Create or select a device in ThingsBoard.
2. Configure the device credentials required by your firmware.
3. Create dashboard widgets for soil moisture, temperature, humidity, pressure, and pump status.
4. Configure MQTT telemetry and RPC settings to match the firmware.
5. Verify telemetry reception and test remote pump control.

The MQTT topics, JSON fields, and RPC method names must match your actual firmware implementation.

## Project Demonstration

Explore the project showcase on LinkedIn for demonstration content and project visuals.

[**View Smart Irrigation System Demo on LinkedIn**](https://lnkd.in/p/g7FB93Z2)

## Project Scope

This project demonstrates an embedded IoT workflow integrating:

- ESP32 firmware development using ESP-IDF
- Sensor acquisition and threshold-based control
- Relay-driven actuator management
- Wi-Fi connectivity and MQTT communication
- ThingsBoard telemetry visualization and remote device interaction

## Future Improvements

- Implement soil-moisture calibration and configurable thresholds.
- Add maximum pump run-time limits and cooldown intervals.
- Introduce sensor-failure detection and safe fallback behavior.
- Improve Wi-Fi reconnection and MQTT error handling.
- Strengthen credential management and encrypted communication.
- Evaluate irrigation performance using soil-moisture trends and measured water consumption.

## License

Add a `LICENSE` file and specify the license here if you intend to distribute the project publicly.
