/*
 * Alarm management declarations for DC Motor Speed Control Project
 */

#ifndef ALARMS_H
#define ALARMS_H

#include "config.h"
#include "pins.h"

// Alarm types
enum AlarmType {
    ALARM_NONE,
    ALARM_OVERCURRENT,
    ALARM_OVERSPEED,
    ALARM_SENSOR_FAULT
};

// External declarations
extern AlarmType currentAlarm;

// Function declarations
void checkAlarms();
void resetAlarms();
const char* getAlarmText();

#endif 