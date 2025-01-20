/*
 * Pin definitions for DC Motor Speed Control Project
 */

#ifndef PINS_H
#define PINS_H

#include <stdint.h>
#include <Arduino.h>  // For pinMode, digitalWrite, etc.

// LED Bar pins
const uint8_t LED_BAR_PINS[] = {2, 4, 7, 8, 11, 12, 13};
const uint8_t LED_BAR_COUNT = 7;

// Analog inputs
const uint8_t CURRENT_SENSE_PIN = 21;  // A7
const uint8_t SPEED_SENSE_PIN = 20;    // A6

// Button pins
const uint8_t BUTTON_UP_PIN = 14;
const uint8_t BUTTON_DOWN_PIN = 15;
const uint8_t BUTTON_ENTER_PIN = 16;
const uint8_t BUTTON_BACK_PIN = 17;

// Buzzer pin
const uint8_t BUZZER_PIN = 10;

// RGB LED pins
const uint8_t RGB_RED_PIN = 3;
const uint8_t RGB_GREEN_PIN = 5;
const uint8_t RGB_BLUE_PIN = 6;

// Motor control
const uint8_t MOTOR_PWM_PIN = 9;

// I2C pins (defined in Wire.h)
// SDA = 18
// SCL = 19

// Function declaration
void initializePins();

#endif 