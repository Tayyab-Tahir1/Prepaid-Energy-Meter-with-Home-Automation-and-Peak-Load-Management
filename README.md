# Prepaid Energy Meter with Home Automation and Peak Load Management

## Overview

This repository presents the implementation of a **Prepaid Energy Meter** equipped with **Home Automation** and **Peak Load Management** capabilities. The system leverages the **Internet of Things (IoT)**, an **Android Application**, and hardware components like **ESP32** and **Arduino UNO** to provide an efficient, user-friendly, and cost-effective energy management solution. 

The smart meter integrates real-time power monitoring, prepaid billing, and load control functionalities, making it a practical solution for residential energy management. Additionally, it supports peak load management to mitigate grid stress during high-demand periods and enables theft detection to enhance system security.

---

## Features

- **IoT-Based Prepaid Billing**:
  - Monitors energy consumption in real time.
  - Facilitates prepaid billing via an Android application.
- **Home Automation**:
  - Provides control of home appliances through the Android app.
  - Supports secure login and remote load management.
- **Peak Load Management**:
  - Automatically disconnects power loads during peak hours while maintaining light loads.
  - Implements theft detection and alerts during unauthorized load bypassing.
- **Android Application**:
  - Real-time data visualization of energy usage and billing.
  - Biometric-secured login and account management.
- **Comprehensive Hardware Integration**:
  - Utilizes ESP32 for data handling and communication.
  - Employs current and potential transformers for accurate power measurements.

---

## Methodology

### 1. Hardware Components
- **Voltage Measurement**:
  - Potential transformer (250/12V) measures line voltage.
  - Signal conditioning ensures compatibility with ESP32.
- **Current Measurement**:
  - Current transformer (5A rating) measures load current.
  - Signal conditioning converts analog signals to compatible pulses.
- **Power Factor Calculation**:
  - Operational amplifier generates pulsating signals to calculate power factor.
- **Circuit Design**:
  - Power supply circuit supports multiple voltage levels (3.3V, 5V, 12V).
  - OLED display visualizes real-time power and billing data.

### 2. IoT and Database Integration
- **Cloud Database**:
  - Utilizes Firebase for seamless communication between the smart meter and the Android application.
- **Unit Measurement**:
  - Optocoupler detects LED pulses from the energy meter.
  - Arduino calculates consumed and credited units based on the entered amount.

### 3. Android Application
- Developed using Kodular Creator, the app provides:
  - Secure login with biometric authentication.
  - Real-time monitoring of power consumption and billing status.
  - Load control options for home automation.

### 4. Peak Load Management
- Automatically disconnects power loads during predefined peak hours.
- Light loads remain connected to reduce the strain on UPS batteries and grid infrastructure.
- Alerts for unauthorized load bypassing with theft detection mechanisms.

---

## Results

### Key Observations
- **Energy Monitoring**:
  - Real-time tracking of voltage, current, power factor, and energy consumption.
- **Prepaid Billing**:
  - Automatic disconnection of loads when credit is exhausted.
  - Recharging via the Android app efficiently updates meter credits.
- **Peak Load Management**:
  - Successfully reduces grid stress during high-demand periods.
  - Prevents unauthorized load connection through theft detection.

### Hardware Prototype
- A fully functional prototype was developed and tested, demonstrating the practical application of the system.

---

## Repository Content

```
📂 Root Directory
├── Arduino/           # Code for Arduino UNO for energy meter control
├── ESP32/             # ESP32 firmware for IoT functionalities
├── Simulation/    # Circuit design and simulations in Proteus
├── Android_Application/            # Source files, APK and other data for Android application
├── LICENSE                 # License information
├── README.md               # Project documentation
```

---

## Prerequisites

- **Hardware**:
  - ESP32, Arduino UNO, OLED display.
  - Current Transformer, Potential Transformer, Relays.
- **Software**:
  - Proteus for simulations.
  - Kodular Creator for Android app development.
- **Python** (optional for data analysis and visualization).

---

## Setup and Usage

### 1. Hardware Setup
- Assemble the components as per the provided circuit diagram in the `Proteus_Simulations/` folder.
- Load the respective codes for Arduino and ESP32 from the `Arduino_Code/` and `ESP32_Code/` folders.

### 2. Android App
- Install the APK from the `Android_App/` folder.
- Login with biometric authentication and manage loads or recharge the meter.

### 3. Simulation
- Open `.dsn` files in Proteus to visualize the circuit design and validate the hardware setup.

---

## Applications

1. **Residential Energy Management**:
   - Prepaid billing with real-time monitoring.
   - Reduces energy costs through peak load control.
2. **Grid Efficiency**:
   - Reduces strain on the grid during peak hours.
   - Improves load distribution with automated energy management.
3. **Sustainability**:
   - Encourages efficient energy usage and reduces wastage.

---

## License

This project is licensed under the Creative Commons Attribution (CC BY) license. See the [LICENSE](LICENSE) file for details.

---

## References

- Tahir, Z.M., Tahir, T., et al. *Design and Implementation of Prepaid Energy Meter with Home Automation and Peak Load Management Using Android Application*. Eng. Proc. 2023, 46, 23. DOI: [10.3390/engproc2023046023](https://doi.org/10.3390/engproc2023046023)
