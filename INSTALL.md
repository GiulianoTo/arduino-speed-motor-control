# Installation Guide

## Requirements

- Arduino IDE 2.0 or later
- Required Libraries:
  - U8g2lib
  - PID_v1
  - Wire (included in Arduino IDE)
  - EEPROM (included in Arduino IDE)

## Hardware Setup

1. Connect all components according to the pin configuration in `pins.h`
2. Verify power supply connections
3. Configure the motor driver according to its specifications

## Software Installation

### Library Versions
- U8g2lib: 2.32.15 or later
- PID_v1: 1.2.0 or later

1. Install Arduino IDE
2. Install required libraries through Library Manager
3. Open MotorSpeedControlProject.ino
4. Select "Arduino Nano Every" as the board
5. Select the appropriate COM port
6. Compile and upload

### Installation Verification

After uploading, verify:
1. OLED display shows splash screen
2. RGB LED shows blue (IDLE state)
3. All LED bar segments light up briefly during initialization
4. Buttons respond to presses (enter menu system)

## Initial Configuration

After uploading, the system will:
1. Show splash screen for 5 seconds
2. Load default parameters
3. Enter IDLE state

Use the menu system to:
- Calibrate current and speed readings
- Adjust PID parameters
- Set operating limits

## Troubleshooting

- If the display doesn't show anything, verify I2C connections
- If motor doesn't respond, check PWM output and driver connections
- For overcurrent alarms, verify current sensor calibration 