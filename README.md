# ✈️ Flight Controller Rev2
<p align="center">
<img  width="39%" src="https://github.com/hiteshbhoyar03/flight-controller-v2/blob/main/gallery/flight%20controller%20rev2%20front.png">
</p>

**Flight Controller Rev2** is a dual-processor, high-performance embedded control system designed for learning and portfolio enhancement purposes. Developed independently, this project demonstrates my skills in embedded systems, PCB design, sensor integration, and flight control logic. All schematics and PCB layouts are completed using Altium Designer, while firmware development is actively progressing in STM32CubeIDE.

> Note: This project is simulation-only and has **not** been fabricated or flight-tested. It serves purely as a design and firmware engineering showcase.

The goal of this project is to simulate the development of a **complete flight control system**, suitable for use in Drones, UAVs or hobby Rockets. It covers the entire hardware-software co-design flow—from schematic and PCB layout to embedded firmware for real-time control, telemetry, and communication protocols.

---

## 🧩 System Architecture

The board integrates **two STM32H753VI** ARM Cortex-M7 microcontrollers on a single 100mm × 76mm PCB, functioning as a **Main Processor** and **Co-Processor**, connected via UART and 8 shared GPIO lines.

### 🔷 Main Processor Subsystem

| Component          | Description                                                              |
|--------------------|--------------------------------------------------------------------------|
| **MCU**            | STM32H753VI   (Cortex-M7 @ 400MHz, 2MB Flash, 1MB RAM)                   |
| **External Flash** | MT29F4G01ABAFDWB (4Gb NAND Flash for large data logs or mission storage) |
| **EEPROM**         | M95M04 (512KB SPI EEPROM for config or mission profiles)                 |
| **USB**            | Micro USB OTG FS (for firmware upload, debug)                            |
| **PWM TIMERS**     | Enables 16 PWM channels for ESCs or servos                               |
| **UART Buffer**    | SN74LVC2G34 (For safe and clean UART communication)                      |
| **Power**          | LD39200PU33 (3.3V, 2A Low Dropout Regulator)                             |
| **Interfaces**     | SPI, I2C, 2 × UART, 16 PWM                                               |

---

### 🔷 Co-Processor Subsystem

| Component         | Description                                                               |
|-------------------|---------------------------------------------------------------------------|
| **MCU**           | STM32H753VIT6 ( dedicated to sensor data acquisition and pre-processing)  |
| **EEPROM**        | M95M04-DRMN6TP                                                            |
| **Sensor Suite**  |
| • BMI088          | 6-axis high-performance IMU (gyro + accel)                                |
| • ICM-42688-P     | Ultra-low noise, low drift IMU                                            |
| • ICM-45686       | Ultra-low power IMU for efficient runtime                                 |
| • MS561101BA03    | High-resolution pressure sensor                                           |
| • ICP-20100       | MEMS barometric pressure sensor                                           |
| • MMC5983MA       | 3-axis magnetometer                                                       |
| **GNSS Module**   | NEO-M9N-00B (Multi-constellation GPS/GLONASS with external UART interface)|
| **USB**           | Micro USB OTG FS                                                          |
| **Power**         | LD39200PU33                                                               |
| **Interfaces**    | I2C, UART, GPS UART                                                       |


---

## 🔗 Processor Interconnect

- **Primary Link**: UART (for high-speed, structured data exchange)
- **Auxiliary Signals**: 8 bidirectional GPIO lines (handshake, interrupt, redundancy, sync)

---

## 💻 Firmware Architecture

Firmware development is underway using **STM32CubeIDE**, designed with a modular and layered approach:

### ✅ Completed Modules:
- **Sensor Drivers**: All onboard sensors initialized and configured (BMI088, ICM series, barometers, magnetometer)
- **GPS Parsing**: NEO-M9N GNSS interface fully functional
- **PWM Protocols**:
  - **Oneshot42**
  - **Oneshot125**
  - **Multishot**
  - **DShot (150/300/600/1200)** 
- **RC Protocols**:
  - **IBUS**
  - **SBUS**
  - **CRSF** (Crossfire telemetry protocol)
- **Sensor Bus**: SPI interface abstraction completed
- **Configuration Handler**: EEPROM read/write layer implemented for persistent config

### 🚧 In Progress:
- **Telemetry System**: Communication with GCS (Ground Control Station) via UART/USB
- **PID Control**: Stabilization and navigation loops (attitude, rate, altitude)
- **GCS Command Parsing**: MAVLink or custom command reception and execution

---
## 🔧 Tools Used

- 🛠 **Altium Designer** – Schematic and PCB Layout  
- 🔌 **STM32CubeIDE** – Embedded development
- 🔌 **STM32CubeMX** – Peripheral and clock config  
- 📦 **GitHub** – VersVersion control , documentation and project showcase
---

## 📐 Board Design Details
- **Size**: 100mm × 76mm
- **Stackup**: 4-layer PCB designed in **Altium Designer**
- **Manufacturing**: No PCB fabrication — for simulation and demonstration only

---

## 👤 Author

**Hitesh Bhoyar**  
📍 Embedded Systems | CubeSat Design | Low Power Electronics  
[GitHub →](https://github.com/hiteshbhoyar03)
> For educational use. Feel free to fork, reuse, or reach out if you are exploring embedded systems!

---

## 📜 License

Licensed under the [MIT License](../LICENSE)

---

## 🎯 Project Purpose

An academic and portfolio project to:

- Explore and apply **embedded hardware design** techniques  
- Design **custom PCBs** using **Altium Designer**  
- Demonstrate engineering workflows for Flight Controller systems  
