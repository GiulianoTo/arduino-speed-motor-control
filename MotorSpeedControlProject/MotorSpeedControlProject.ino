/*
 * DC Motor Speed Control Project
 * 
 * This project implements a complete DC motor speed control system
 * using an Arduino Nano Every board.
 */

#include <Wire.h>
#include <EEPROM.h>
#include <PID_v1.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "pins.h"
#include "config.h"
#include "menu.h"
#include "states.h"
#include "display.h"
#include "eeprom_manager.h"  // For loadParameters()
#include "alarms.h"          // For checkAlarms()
#include "globals.h"

// Global variables definition
SystemParameters systemParams;  // Actual definition
double pidInput = 0, pidOutput = 0, pidSetpoint = 0;
PID motorPID(&pidInput, &pidOutput, &pidSetpoint, 
             DEFAULT_KP, DEFAULT_KI, DEFAULT_KD, DIRECT);

// System state
SystemState currentState = STATE_IDLE;

// La definizione di tft è stata spostata in display.cpp

void setup() {
  // Initialize all pins
  initializePins();
  
  // Initialize display
  initializeDisplay();
  
  // Show splash screen
  showSplashScreen();
  delay(5000);
  
  // Initialize PID controller
  motorPID.SetMode(AUTOMATIC);
  motorPID.SetOutputLimits(0, 255);
  
  // Load parameters from EEPROM
  loadParameters();
  
  // Initialize menu system
  initializeMenu();
}

void loop() {
  // Read inputs
  readInputs();
  
  // Update state machine
  updateStateMachine();
  
  // Update display
  updateDisplay();
  
  // Update LED bar
  updateLedBar();
  
  // Process menu if needed
  processMenu();
  
  // Handle alarms
  checkAlarms();
} 