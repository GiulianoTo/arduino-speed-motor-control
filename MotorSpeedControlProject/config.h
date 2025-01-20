/*
 * Configuration and constants for DC Motor Speed Control Project
 */

#ifndef CONFIG_H
#define CONFIG_H

// System parameters default values
const float DEFAULT_CURRENT_FULL_SCALE = 30.0;  // Ampere
const int DEFAULT_SPEED_FULL_SCALE = 3000;      // RPM
const float DEFAULT_KP = 1.0;
const float DEFAULT_KI = 0.0;
const float DEFAULT_KD = 0.0;

// EEPROM addresses for parameters
const int EEPROM_CURRENT_FS_ADDR = 0;
const int EEPROM_SPEED_FS_ADDR = 4;
const int EEPROM_KP_ADDR = 8;
const int EEPROM_KI_ADDR = 12;
const int EEPROM_KD_ADDR = 16;

// System parameters (runtime)
struct SystemParameters {
    float currentFullScale;
    int speedFullScale;
    float kp;
    float ki;
    float kd;
};

// Alarm thresholds
const float OVERCURRENT_THRESHOLD = 0.9;  // 90% of full scale

// Display update interval
const unsigned long DISPLAY_UPDATE_INTERVAL = 100;  // ms

// Menu timeout
const unsigned long MENU_TIMEOUT = 30000;  // 30 seconds

// LED bar update interval
const unsigned long LED_BAR_UPDATE_INTERVAL = 100;  // ms

// Alarm buzzer frequency
const unsigned int ALARM_BUZZER_FREQ = 2000;  // Hz
const unsigned long ALARM_BUZZER_INTERVAL = 500;  // ms

// ADC configuration
const int ADC_RESOLUTION = 1024;  // 10-bit ADC

#endif 