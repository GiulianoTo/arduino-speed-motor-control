/*
 * Pin definitions for DC Motor Speed Control Project
 */

#ifndef PINS_H
#define PINS_H

#include <stdint.h>
#include <Arduino.h>  // For pinMode, digitalWrite, etc.

// LED Bar configuration
//---------------------
const uint8_t LED_BAR_PINS[] = {2, 4, 7, 8, 11, 12, 13};  // D2-D13 pins for LED bar
const uint8_t LED_BAR_COUNT = 7;                          // Number of LEDs in bar

// Analog inputs
//-------------
const uint8_t CURRENT_SENSE_PIN = 21;  // A7: Current sensor input
const uint8_t SPEED_SENSE_PIN = 20;    // A6: Speed sensor input

// User interface pins
//------------------
const uint8_t BUTTON_UP_PIN = 14;      // UP button with internal pullup
const uint8_t BUTTON_DOWN_PIN = 15;    // DOWN button with internal pullup
const uint8_t BUTTON_ENTER_PIN = 16;   // ENTER button with internal pullup
const uint8_t BUTTON_BACK_PIN = 17;    // BACK button with internal pullup
const uint8_t BUZZER_PIN = 10;         // Buzzer output

// Status indication
//----------------
const uint8_t RGB_RED_PIN = 3;     // RGB LED red component (PWM)
const uint8_t RGB_GREEN_PIN = 5;    // RGB LED green component (PWM)
const uint8_t RGB_BLUE_PIN = 6;     // RGB LED blue component (PWM)

// Motor control
//------------
const uint8_t MOTOR_PWM_PIN = 9;    // Motor PWM output

// I2C pins (defined in Wire.h)
// SDA = 18
// SCL = 19

// Function declaration
void initializePins();

#endif 