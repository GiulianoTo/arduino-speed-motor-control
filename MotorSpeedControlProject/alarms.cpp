/*
 * Alarm management implementation for DC Motor Speed Control Project
 */

#include "alarms.h"
#include "globals.h"

// Current alarm status
AlarmType currentAlarm = ALARM_NONE;

// Timer for clearing the alarm
unsigned long alarmClearTime = 0;
const unsigned long ALARM_CLEAR_DELAY = 5000; // 5 seconds

// Function to check for alarm conditions
void checkAlarms() {
    if (isOvercurrent) {
        currentAlarm = ALARM_OVERCURRENT;
        currentState = STATE_ALARM;
        alarmClearTime = millis(); // Reset exit timer
    } else if (currentSpeed > systemParams.speedFullScale * 1.1) {  // 10% margin
        currentAlarm = ALARM_OVERSPEED;
        currentState = STATE_ALARM;
        alarmClearTime = millis(); // Reset exit timer
    } else if (currentState == STATE_ALARM) {
        // If current is below threshold, check elapsed time
        if (millis() - alarmClearTime >= ALARM_CLEAR_DELAY) {
            currentAlarm = ALARM_NONE;
            currentState = STATE_IDLE;
        }
    }
}

// Function to reset alarms
void resetAlarms() {
    if (currentState == STATE_ALARM) {
        if (!isOvercurrent && currentSpeed <= systemParams.speedFullScale) {
            currentAlarm = ALARM_NONE;
            currentState = STATE_IDLE;
        }
    }
}

// Function to get alarm description
const char* getAlarmText() {
    switch(currentAlarm) {
        case ALARM_OVERCURRENT:
            return "OVERCURRENT";
        case ALARM_OVERSPEED:
            return "OVERSPEED";
        case ALARM_SENSOR_FAULT:
            return "SENSOR FAULT";
        default:
            return "NO ALARM";
    }
}
