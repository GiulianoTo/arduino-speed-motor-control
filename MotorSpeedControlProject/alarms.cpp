/*
 * Alarm management implementation for DC Motor Speed Control Project
 */

#include "alarms.h"
#include "globals.h"

// Current alarm status
AlarmType currentAlarm = ALARM_NONE;

// Function to check for alarm conditions
void checkAlarms() {
    if (isOvercurrent) {
        currentAlarm = ALARM_OVERCURRENT;
        currentState = STATE_ALARM;
    } else if (currentSpeed > systemParams.speedFullScale * 1.1) {  // 10% margin
        currentAlarm = ALARM_OVERSPEED;
        currentState = STATE_ALARM;
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