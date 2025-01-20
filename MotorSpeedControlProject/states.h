/*
 * State machine declarations for DC Motor Speed Control Project
 */

#ifndef STATES_H
#define STATES_H

#include "config.h"
#include "pins.h"

// System states
enum SystemState {
    STATE_IDLE,
    STATE_RUN,
    STATE_ALARM
};

// External declarations
extern float currentSpeed;
extern float currentCurrent;
extern bool isOvercurrent;

// Function declarations
void setStateColor(SystemState state);
void readInputs();
void handleAlarm();
void updateStateMachine();

#endif 