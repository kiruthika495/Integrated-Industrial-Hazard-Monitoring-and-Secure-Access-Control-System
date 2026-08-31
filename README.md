# Integrated Industrial Hazard Monitoring and Secure Access Control System

## 1. Project Overview

The **Integrated Industrial Hazard Monitoring and Secure Access Control System** is an embedded safety and security system designed for industrial environments. It combines continuous monitoring of hazardous conditions with RFID-and-password-based access control.

The system uses an **LPC21xx ARM7 microcontroller** as the central controller. Environmental parameters such as temperature, gas concentration, light intensity, flame, and object/intrusion conditions are monitored. When a hazardous condition is detected, the system provides a local warning and can send an SMS alert through a GSM module.

For secure entry, an authorized user must first scan an RFID card and then enter the correct password using a keypad. Access is verified only when both authentication steps are successful.

## 2. Objectives

- Monitor important industrial hazards continuously.
- Detect abnormal temperature, gas, flame, light, and intrusion conditions.
- Provide immediate local indication through LCD.
- Send hazard notifications through GSM.
- Restrict entry using RFID authentication and password verification.
- Record important events with RTC-based date/time information.
- Integrate sensing, communication, monitoring, and security in one embedded system.

## 3. Major Hardware Components

| Component | Purpose |
|---|---|
| LPC21xx ARM7 | Main controller and decision-making unit |
| LM35 | Temperature measurement and overheating detection |
| Gas Sensor | Detection of harmful/combustible gas level |
| LDR | Monitoring light intensity |
| Flame Sensor | Fire/flame detection |
| IR Sensor | Object/intrusion detection |
| MCP3204 ADC | Converts analog sensor signals for the controller |
| RFID Reader | Identifies an authorized RFID card |
| 4x4 Keypad | Password entry |
| 16x2 LCD | Displays sensor values, status, and access messages |
| GSM Module | Sends SMS alerts |
| RTC | Provides date/time for event monitoring/logging |
| I2C EEPROM | Stores required data/events |
| Power Supply | Provides regulated power to the system |

## 4. System Block Diagram

The system is divided into two main sections:

### Hazard Monitoring
Sensors → Signal Conditioning/ADC → LPC21xx → LCD / GSM / RTC

### Secure Access Control
RFID Reader → LPC2129 → Password Keypad → Access Verification.

The LPC2129 coordinates both sections and makes the final monitoring and access-control decisions.

## 5. Working Principle

### 5.1 Hazard Monitoring

1. The microcontroller initializes the LCD, ADC, RTC, communication interfaces, and other peripherals.
2. Analog sensors are read through the **MCP3204 ADC using SPI**.
3. Digital sensors such as the flame and IR sensors are read through GPIO.
4. The controller compares sensor readings with programmed threshold values.
5. If a hazardous condition is detected:
   - The hazard status is shown on the LCD.
   - A buzzer/LED can provide a local warning.
   - An SMS alert can be sent through the GSM module.
   - The event can be associated with RTC date/time information.
6. Normal conditions continue to be monitored in a loop.

### 5.2 Secure Access Control

1. The LCD prompts the user to scan an RFID card.
2. The RFID reader sends the card UID to the controller through UART.
3. The controller compares the received UID with the authorized UID.
4. If the RFID card is valid, the system asks for the password.
5. The password is entered using the 4x4 keypad.
6. The entered password is compared with the stored password.
7. If both RFID and password are correct, the LCD displays **Access Verified** and the door-lock/relay can be activated.
8. If either authentication step fails, the LCD displays **Access Denied** and the door remains locked.

## 6. Communication Interfaces

| Interface | Used For |
|---|---|
| SPI | LPC21xx ↔ MCP3204 ADC |
| UART | RFID/GSM communication |
| I2C | RTC and EEPROM |
| GPIO | Flame sensor, IR sensor, keypad |
| LCD Interface | Status and measurement display |

## 7. Sensor Functions

### Temperature Sensor – LM35
The LM35 produces an analog voltage proportional to temperature. The controller reads this voltage through the ADC and converts it into temperature.

Typical relationship:

**Temperature (°C) = Sensor Voltage (V) × 100**

### Gas Sensor
The gas sensor detects an increase in the concentration of selected gases. Its analog output can be converted using the MCP3204 and compared against a programmed threshold.

### LDR
The LDR changes its resistance according to light intensity. The controller monitors its voltage level to determine whether the environment is sufficiently bright or dark.

### Flame Sensor
The flame sensor provides a digital indication when flame/fire is detected. A detected flame causes the controller to enter the hazard-alert condition.

### IR Sensor
The IR sensor detects an object or intrusion in its sensing range. It can be used to monitor restricted areas or detect unexpected movement near an access point.

## 8. Example Thresholds

The following values are examples for demonstration and must be calibrated for the actual sensors and industrial environment.

| Parameter | Example Condition |
|---|---|
| Temperature | Above 50 °C |
| Gas | Above calibrated gas threshold |
| LDR | Below calibrated dark-level threshold |
| Flame | Flame detected |
| IR | Object/intrusion detected |

**Important:** Actual sensor thresholds should be determined experimentally using the specific sensor modules, ADC reference voltage, installation conditions, and required safety limits.

## 9. Software

- Programming Language: **Embedded C**
- Microcontroller: **LPC21xx ARM7**
- Development Environment: **Keil µVision**
- ADC: **MCP3204**
- Communication: **SPI, UART, I2C**
- Display: **16x2 LCD**

Typical software modules include:

```text
Project/
├── main.c              # Main application
├── lcd.h       # LCD functions
├──  delay.h   # Delay routines
├── uart.h     # UART functions
├── spi.h       # SPI and MCP3204 functions
├── i2c.h       # I2C functions
├── eeprom.c / eeprom.h # EEPROM functions
└── keypad.c / keypad.h # Keypad functions
```
