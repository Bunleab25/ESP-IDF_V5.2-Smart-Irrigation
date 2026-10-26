# Smart Irrigation System | ESP32 · ESP-IDF · MQTT · ThingsBoard

![Version](https://img.shields.io/badge/Version-1.0.0-2EA44F?style=flat-square)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.2.5-E67E22?style=flat-square&logo=espressif&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-ESP32-E7352C?style=flat-square&logo=espressif&logoColor=white)
![Protocol](https://img.shields.io/badge/Protocol-MQTT-660066?style=flat-square&logo=mqtt&logoColor=white)
![IoT Platform](https://img.shields.io/badge/IoT%20Platform-ThingsBoard-305680?style=flat-square)

[![LinkedIn Project Showcase](https://img.shields.io/badge/LinkedIn-Project%20Showcase-0A66C2?style=flat-square&logo=linkedin&logoColor=white)](https://lnkd.in/p/g7FB93Z2)

An **embedded IoT automation system** designed to improve irrigation management through real-time soil-moisture monitoring, environmental sensing, and automated pump control. Built on the ESP32 using the native ESP-IDF framework, the system integrates sensor acquisition, threshold-based control logic, MQTT telemetry, and remote supervision through ThingsBoard.

---

## Project Overview

Traditional irrigation methods often rely on fixed schedules or manual observation, which may result in inefficient water usage and inconsistent soil-moisture conditions.

This project explores a sensor-driven alternative by integrating embedded firmware, environmental instrumentation, and cloud-connected device management into a unified IoT workflow.

The ESP32 acquires soil-moisture measurements and environmental data from a BME280 sensor. The firmware evaluates configured irrigation conditions and controls a water pump through a relay module. Meanwhile, MQTT communication enables telemetry delivery to ThingsBoard for remote visualization and, when configured, manual actuator control.

The project emphasizes **embedded firmware engineering, hardware–software integration, IoT communication, and practical automation**.

## System Capabilities

- **Sensor-driven irrigation control** — Uses soil-moisture readings and configurable thresholds to determine irrigation requirements.
- **Environmental data acquisition** — Measures ambient temperature, relative humidity, and atmospheric pressure using the BME280.
- **Embedded firmware development** — Uses ESP-IDF v5.2.5 for native ESP32 application development.
- **Real-time task management** — Uses FreeRTOS capabilities where implemented in the firmware.
- **MQTT-based telemetry** — Transmits sensor measurements and device status to the IoT platform.
- **Remote monitoring** — Presents sensor readings and historical data through ThingsBoard dashboards.
- **Remote actuator interaction** — Supports remote pump commands through ThingsBoard RPC when configured.
- **Physical actuation** — Interfaces with a relay-controlled water pump to execute irrigation operations.

## System Architecture

```text
┌─────────────────────────────────┐
│       SENSING LAYER             │
│                                 │
│  Soil Moisture Sensor           │
│  BME280 Environmental Sensor    │
└────────────────┬────────────────┘
                 │
                 ▼
┌─────────────────────────────────┐
│       EMBEDDED CONTROL LAYER    │
│                                 │
│  ESP32 Microcontroller          │
│  ESP-IDF v5.2.5                 │
│  Sensor Acquisition             │
│  Irrigation Control Logic       │
│  Wi-Fi / MQTT Client            │
└────────────┬──────────────┬─────┘
             │              │
             ▼              ▼
┌────────────────────┐  ┌────────────────────┐
│ ACTUATION LAYER    │  │ COMMUNICATION LAYER│
│                    │  │                    │
│ Relay Module       │  │ Wi-Fi              │
│ Water Pump         │  │ MQTT               │
└────────────────────┘  └──────────┬─────────┘
                                   │
                                   ▼
                       ┌────────────────────┐
                       │ IoT PLATFORM       │
                       │                    │
                       │ ThingsBoard        │
                       │ Telemetry Dashboard│
                       │ Remote Control     │
                       └────────────────────┘
```

## Hardware Architecture

| Component | Interface | Engineering Function |
|---|---|---|
| ESP32 development board | GPIO, ADC, Wi-Fi | Embedded processing, sensor acquisition, control execution, and wireless communication |
| Soil-moisture sensor | ADC / GPIO, depending on model | Soil-moisture measurement for irrigation decisions |
| BME280 sensor | I²C / SPI | Environmental data acquisition |
| Single-channel relay module | Digital GPIO | Electrical switching interface for the pump |
| 5 V mini submersible pump | Relay-switched power circuit | Water delivery during irrigation |
| Power supply | Component-specific connections | Supplies the system according to individual electrical requirements |

> **Electrical design:** Verify operating voltage, current requirements, relay compatibility, and power-supply ratings before connecting the hardware. Do not drive the pump directly from an ESP32 GPIO.

## Software Architecture

| Layer | Technology | Purpose |
|---|---|---|
| Firmware framework | ESP-IDF v5.2.5 | Native embedded application development |
| Real-time operating system | FreeRTOS | Task scheduling and concurrent execution, where implemented |
| Peripheral interfaces | ADC, GPIO, I²C / SPI | Sensor acquisition and actuator control |
| Network connectivity | Wi-Fi | Wireless device connectivity |
| Messaging protocol | MQTT | Telemetry transport and device communication |
| IoT platform | ThingsBoard | Telemetry visualization, historical monitoring, and remote interaction |
| Physical actuation | Relay and water pump | Execution of irrigation commands |

## Control and Data Flow

**01 — Data acquisition**

The ESP32 reads soil-moisture measurements and collects environmental data from the BME280 at configured intervals.

**02 — Irrigation decision**

The firmware evaluates the soil-moisture readings against configured control thresholds to determine whether watering is required.

**03 — Actuator execution**

The ESP32 controls the relay interface to switch the pump according to the irrigation logic or an authorized remote command.

**04 — Telemetry transmission**

Sensor measurements and relevant device states are published through MQTT for delivery to the configured IoT platform.

**05 — Remote supervision**

ThingsBoard presents telemetry through dashboard widgets and historical charts. Remote pump control is available when the corresponding RPC functionality is implemented and configured.

## Technology Stack

- **Embedded systems:** ESP32, ESP-IDF v5.2.5, C/C++
- **Real-time execution:** FreeRTOS
- **Hardware interfaces:** ADC, GPIO, I²C / SPI
- **Connectivity:** Wi-Fi, MQTT
- **IoT monitoring:** ThingsBoard
- **Actuation:** Relay-controlled water pump

## Build and Deployment

### Prerequisites

- ESP-IDF v5.2.5 development environment
- ESP32 development board
- Compatible soil-moisture and BME280 sensors
- Relay module and appropriately rated pump power supply
- ThingsBoard instance and device credentials
- USB data cable and serial-port access

### 1. Clone the Repository

```bash
git clone https://github.com/<your-username>/<your-repository>.git
cd <your-repository>
```

Replace the placeholders with the actual repository URL.

### 2. Configure the ESP32 Target

Activate the ESP-IDF environment, then run:

```bash
idf.py set-target esp32
idf.py menuconfig
```

Configure the network credentials, MQTT or ThingsBoard connection parameters, and application settings supported by the project.

### 3. Compile the Firmware

```bash
idf.py build
```

### 4. Flash and Monitor

```bash
idf.py -p PORT flash monitor
```

Replace `PORT` with the serial device assigned to your ESP32, such as `/dev/ttyUSB0` on Linux or `COM3` on Windows.

Press `Ctrl+]` to exit the serial monitor.

### 5. Integrate with ThingsBoard

1. Provision a device and configure the credentials required by the firmware.
2. Configure telemetry fields and dashboard widgets.
3. Match the MQTT settings and message structure to the implementation.
4. Configure RPC methods if remote pump control is supported.
5. Validate sensor readings, irrigation decisions, actuator behavior, and telemetry delivery.

**Configuration note:** MQTT topics, telemetry keys, RPC method names, GPIO assignments, and sensor settings must match the actual source code and hardware wiring.

## Project Demonstration

Explore the project showcase for visual documentation and a demonstration of the system.

[**View Project Showcase on LinkedIn →**](https://lnkd.in/p/g7FB93Z2)

## Engineering Focus

This project demonstrates practical experience in:

- Native ESP32 firmware development with ESP-IDF
- Sensor interfacing and embedded data acquisition
- Threshold-based automation and actuator control
- Wi-Fi connectivity and MQTT communication
- Embedded hardware–software integration
- IoT telemetry visualization and remote device supervision

## Potential Enhancements

- Soil-moisture calibration and configurable control thresholds
- Maximum pump run-time limits and cooldown logic
- Sensor fault detection and safe-state handling
- More resilient Wi-Fi reconnection and MQTT error recovery
- Secure credential management and TLS-enabled communication
- Water-consumption measurement and irrigation performance evaluation

## License

Specify the project's license by adding a `LICENSE` file to the repository.
