/*
 * State machine implementation for DC Motor Speed Control Project
 */

#include "states.h"
#include "globals.h"
#include "Mapf.h"

// Current measurements
float currentSpeed = 0.0;
float currentCurrent = 0.0;
bool isOvercurrent = false;
SystemState previousState = STATE_UNDEFINED;

// RGB LED colors for different states
void setStateColor(SystemState state) {
    switch(state) {
        case STATE_IDLE:
            digitalWrite(RGB_BLUE_PIN, LOW);
            digitalWrite(RGB_RED_PIN, LOW);
            digitalWrite(RGB_GREEN_PIN, HIGH);
            break;
        case STATE_RUN:
            digitalWrite(RGB_BLUE_PIN, HIGH);
            digitalWrite(RGB_RED_PIN, LOW);
            digitalWrite(RGB_GREEN_PIN, LOW);
            break;
        case STATE_ALARM:
            digitalWrite(RGB_BLUE_PIN, LOW);
            digitalWrite(RGB_RED_PIN, HIGH);
            digitalWrite(RGB_GREEN_PIN, LOW);
            break;
    }
}

// Read analog inputs and convert to actual values
void readInputs() {
    // Read speed input
    int speedRaw = analogRead(SPEED_SENSE_PIN);
    currentSpeed = mapf(speedRaw, 0, ADC_RESOLUTION-1, 0, systemParams.speedFullScale);
    
    // Read current input
    int currentRaw = analogRead(CURRENT_SENSE_PIN);
    currentCurrent = mapf(currentRaw, 0, ADC_RESOLUTION-1, 0, systemParams.currentFullScale);
    
    // Check for overcurrent condition
    isOvercurrent = (currentCurrent >= (systemParams.currentFullScale * OVERCURRENT_THRESHOLD));
}

// Alarm handling
void handleAlarm() {
    static unsigned long lastBuzzerToggle = 0;
    static bool buzzerState = false;
    
    if (currentState == STATE_ALARM) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastBuzzerToggle >= ALARM_BUZZER_INTERVAL) {
            buzzerState = !buzzerState;
            if (buzzerState) {
                tone(BUZZER_PIN, ALARM_BUZZER_FREQ);
            } else {
                noTone(BUZZER_PIN);
            }
            lastBuzzerToggle = currentMillis;
        }
    } else {
        noTone(BUZZER_PIN);
        buzzerState = false;
    }
}

// State machine update
void updateStateMachine() {
    
    // Check for alarm conditions
    if (isOvercurrent) {
        currentState = STATE_ALARM;
    }
    
    // State-specific behavior
    switch(currentState) {
        case STATE_IDLE:
            analogWrite(MOTOR_PWM_PIN, 0);
            break;
            
        case STATE_RUN:
            // Motor control handled by PID
            if (isOvercurrent) {
                currentState = STATE_ALARM;
            }
            break;
            
        case STATE_ALARM:
            analogWrite(MOTOR_PWM_PIN, 0);
            // Can only exit ALARM state by going to IDLE
            break;
    }
    
    // Update RGB LED if state changed
    if (previousState != currentState) {
        previousState = currentState;
        setStateColor(currentState);
    }
    
    // Handle alarm buzzer
    handleAlarm();
} 