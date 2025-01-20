/*
 * Display management declarations for DC Motor Speed Control Project
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>
#include "config.h"
#include "menu.h"  // Per MenuItem

// Display layout constants
const uint8_t HEADER_HEIGHT = 12;
const uint8_t LINE_HEIGHT = 10;
const uint8_t MENU_START_Y = 15;
const uint8_t VALUE_X = 75;

// Function declarations
void initializeDisplay();
void showSplashScreen();
void drawMainScreen();
void drawMenuScreen();
void drawMenuItems();
void drawMenuItem(const char* text, MenuItem item, uint8_t y);
void updateDisplay();
void updateLedBar();

#endif 