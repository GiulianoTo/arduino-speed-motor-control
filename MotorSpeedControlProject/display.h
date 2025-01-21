/*
 * Display management declarations for DC Motor Speed Control Project
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>
#include "config.h"
#include "menu.h"
#include "alarms.h"  // For getAlarmText()

// Display layout constants
const uint8_t HEADER_HEIGHT = 12;
const uint8_t LINE_HEIGHT = 10;
const uint8_t MENU_START_Y = 15;
const uint8_t VALUE_X = 75;

// Message display timing
const unsigned long MESSAGE_DISPLAY_TIME = 2000;  // 2 seconds
const unsigned long POPUP_TIMEOUT = 5000;        // 5 seconds

// Function declarations
void initializeDisplay();
void showSplashScreen();
void updateDisplay();
void updateLedBar();
void drawMenuScreen();
void drawMenuItem(const char* text, MenuItem item, uint8_t y);

// New functions
void clearDisplay();
void showMessage(const char* message);
void showPopup(const char* title, const char* message, bool needConfirmation = false);
bool isDisplayError();
void handleDisplayError();

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

#endif 