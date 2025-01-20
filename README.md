# DC Motor Speed Control Project

This project implements a complete DC motor speed control system using an Arduino Nano Every board. The system features a PID controller, OLED display interface, and various monitoring and safety features.

## Project Structure

### Core Files
- `MotorSpeedControlProject.ino` - Main program file
- `globals.h` - Global variables and external declarations
- `config.h` - System configuration and constants
- `pins.h` - Pin definitions and initialization

### Control System
- `pid.h` - PID controller implementation
- `states.h` - State machine management
- `alarms.h` - Alarm system management

### User Interface
- `display.h` - OLED display management
- `menu.h` - Menu system implementation
- `utils.h` - Utility functions
- `logo.h` - Splash screen logo bitmap

### Data Management
- `eeprom_manager.h` - Parameter storage in EEPROM

### Documentation
- `README.md` - This file
- `INSTALL.md` - Installation and setup guide

### File Dependencies
```
MotorSpeedControlProject.ino
├── globals.h
│   ├── config.h
│   └── states.h
├── pins.h
│   ├── stdint.h
│   └── Arduino.h
├── display.h
│   ├── U8g2lib.h
│   └── Wire.h
├── menu.h
│   ├── config.h
│   └── display.h
├── pid.h
│   └── PID_v1.h
├── states.h
│   └── config.h
├── alarms.h
│   ├── config.h
│   └── pins.h
└── eeprom_manager.h
    ├── EEPROM.h
    └── config.h
```

## Hardware Components

- Arduino Nano Every board
- 128x128 monochrome OLED display (I2C interface)
- 4 push buttons for menu navigation
- Tachometer dynamo for speed measurement
- Motor current sensing circuit
- RGB LED for controller status indication
- 7 LEDs bar graph for speed indication
- Buzzer for alarm conditions
- Power stage for motor control (PWM driven)

## Features

- PID speed control
- Current limiting protection
- Multiple operation states (IDLE, RUN, ALARM)
- OLED display with menu system for:
  - Real-time speed monitoring (RPM)
  - Current monitoring
  - PID parameters configuration
  - System calibration settings
- Visual feedback through LED bar graph
- Audible alarm notifications
- Parameter persistence in EEPROM

## Default Parameters

- Current full scale: 30A
- Speed full scale: 3000 RPM
- PID Parameters:
  - Proportional gain: 1.0
  - Integral gain: 0.0
  - Derivative gain: 0.0

## Dependencies

- U8g2lib (OLED display)
- PID_v1 (PID control)
- Wire (I2C communication)
- EEPROM (Parameter storage)

## Pin Configuration

- LED Bar: D2, D4, D7, D8, D11, D12, D13
- Motor current input: A7 (D21)
- Motor speed input: A6 (D20)
- Push buttons: D14, D15, D16, D17
- Buzzer: D10
- OLED Display: SDA (D18), SCL (D19)
- Motor PWM output: D9
- RGB LED: D3 (R), D5 (G), D6 (B)

## License

This project is released under the MIT License.

## Installation

See `INSTALL.md` for detailed setup and configuration instructions. 