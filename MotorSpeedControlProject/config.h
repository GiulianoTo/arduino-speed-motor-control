/*
 * Configuration and constants for DC Motor Speed Control Project
 */

#ifndef CONFIG_H
#define CONFIG_H

// System parameters default values
//---------------------------------
const float DEFAULT_CURRENT_FULL_SCALE = 30.0;  // Maximum current in Ampere
const int DEFAULT_SPEED_FULL_SCALE = 3000;      // Maximum speed in RPM
const float DEFAULT_KP = 1.0;                   // Default proportional gain
const float DEFAULT_KI = 0.0;                   // Default integral gain
const float DEFAULT_KD = 0.0;                   // Default derivative gain

// EEPROM memory map
//------------------
const int EEPROM_CURRENT_FS_ADDR = 0;   // 4 bytes (float)
const int EEPROM_SPEED_FS_ADDR = 4;     // 4 bytes (int)
const int EEPROM_KP_ADDR = 8;           // 4 bytes (float)
const int EEPROM_KI_ADDR = 12;          // 4 bytes (float)
const int EEPROM_KD_ADDR = 16;          // 4 bytes (float)

// System parameters structure
struct SystemParameters {
    float currentFullScale;  // Current full scale in Ampere
    int speedFullScale;      // Speed full scale in RPM
    float kp;               // Proportional gain
    float ki;               // Integral gain
    float kd;               // Derivative gain
};

// System timing constants
//----------------------
const unsigned long DISPLAY_UPDATE_INTERVAL = 100;   // Display refresh period in ms
const unsigned long MENU_TIMEOUT = 30000;           // Menu timeout in ms
const unsigned long LED_BAR_UPDATE_INTERVAL = 100;  // LED bar refresh period in ms
const unsigned long ALARM_BUZZER_INTERVAL = 500;    // Buzzer toggle period in ms

// System thresholds and limits
//---------------------------
const float OVERCURRENT_THRESHOLD = 0.9;         // 90% of full scale
const unsigned int ALARM_BUZZER_FREQ = 2000;     // Buzzer frequency in Hz
const int ADC_RESOLUTION = 1024;                 // 10-bit ADC resolution

#endif 