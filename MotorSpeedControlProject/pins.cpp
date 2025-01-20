/*
 * Pin initialization implementation for DC Motor Speed Control Project
 */

#include "pins.h"

void initializePins() {
    // Initialize LED bar pins
    for(uint8_t i = 0; i < LED_BAR_COUNT; i++) {
        pinMode(LED_BAR_PINS[i], OUTPUT);
        digitalWrite(LED_BAR_PINS[i], LOW);
    }
    
    // Initialize button pins with internal pullup
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_ENTER_PIN, INPUT_PULLUP);
    pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
    
    // Initialize buzzer pin
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Initialize RGB LED pins
    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);
    
    // Initialize motor PWM pin
    pinMode(MOTOR_PWM_PIN, OUTPUT);
    analogWrite(MOTOR_PWM_PIN, 0);
    
    // Initialize analog inputs
    pinMode(CURRENT_SENSE_PIN, INPUT);
    pinMode(SPEED_SENSE_PIN, INPUT);
} 