/*
 * PID control declarations for DC Motor Speed Control Project
 */

#ifndef PID_H
#define PID_H

#include <PID_v1.h>
#include "config.h"
#include "states.h"

// PID timing
const unsigned long PID_COMPUTE_INTERVAL = 10;  // 10ms = 100Hz control loop

// PID output limits
const int PID_OUTPUT_MIN = 0;
const int PID_OUTPUT_MAX = 255;  // 8-bit PWM

// Function declarations
void updatePIDParameters();
void resetPID();
void processPID();
void setSpeedSetpoint(double newSetpoint);
void adjustSetpoint(bool increase);

#endif 