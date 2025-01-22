/*
 * Global variables and external declarations for DC Motor Speed Control Project
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <PID_v1.h>
#include <Adafruit_ST7735.h>
#include "config.h"
#include "states.h"
#include "menu.h"  // Per MenuState e MenuItem

// System parameters instance
extern SystemParameters systemParams;

// PID variables
extern double pidInput, pidOutput, pidSetpoint;
extern PID motorPID;

// Display instance
extern Adafruit_ST7735 tft;

// State variables
extern SystemState currentState;
extern MenuState currentMenu;
extern MenuItem selectedItem;

// Global flags
extern bool isOvercurrent;
extern float currentSpeed;
extern float currentCurrent;

#endif 