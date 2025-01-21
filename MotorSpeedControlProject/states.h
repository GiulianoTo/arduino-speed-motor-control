/*
 * State machine declarations for DC Motor Speed Control Project
 */

#ifndef STATES_H
#define STATES_H

#include "config.h"
#include "pins.h"

// System states definition
enum SystemState {
    STATE_IDLE,    // Motor stopped, system ready
    STATE_RUN,     // Motor running with PID control
    STATE_ALARM    // Alarm condition, motor stopped
};

// External declarations for system measurements
extern float currentSpeed;    // Current motor speed in RPM
extern float currentCurrent;  // Current motor current in Ampere
extern bool isOvercurrent;    // Overcurrent condition flag

// State machine functions
void setStateColor(SystemState state);   // Set RGB LED color based on state
void readInputs();                       // Read and process analog inputs
void handleAlarm();                      // Handle alarm conditions
void updateStateMachine();               // Update system state

#endif 