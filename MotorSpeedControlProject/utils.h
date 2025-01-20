/*
 * Utility functions declarations for DC Motor Speed Control Project
 */

#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "menu.h"

// Value adjustment step sizes
const float CURRENT_FS_STEP = 0.5;    // Ampere
const int SPEED_FS_STEP = 100;        // RPM
const float PID_P_STEP = 0.1;
const float PID_I_STEP = 0.01;
const float PID_D_STEP = 0.001;

// Function declarations
void adjustSelectedValue(bool increase);

#endif 