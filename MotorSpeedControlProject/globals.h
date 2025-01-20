/*
 * Global variables and external declarations for DC Motor Speed Control Project
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <PID_v1.h>
#include <U8g2lib.h>
#include "config.h"
#include "states.h"
#include "menu.h"  // Per MenuState e MenuItem

// System parameters instance
extern SystemParameters systemParams;  // Solo dichiarazione

// PID variables
extern double pidInput, pidOutput, pidSetpoint;
extern PID motorPID;  // Solo dichiarazione esterna

// Display instance
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

// State variables
extern SystemState currentState;
extern MenuState currentMenu;
extern MenuItem selectedItem;

// Global flags
extern bool isOvercurrent;
extern float currentSpeed;
extern float currentCurrent;

#endif 