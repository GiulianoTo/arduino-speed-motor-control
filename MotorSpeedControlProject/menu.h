/*
 * Menu system declarations for DC Motor Speed Control Project
 */

#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include "config.h"
#include "pid.h"

// Menu states
enum MenuState {
    MENU_MAIN,
    MENU_SETTINGS,
    MENU_PID,
    MENU_CALIBRATION,
    MENU_NONE
};

// Menu items
enum MenuItem {
    ITEM_RUN,
    ITEM_STOP,
    ITEM_SETTINGS,
    ITEM_BACK,
    ITEM_CURRENT_FS,
    ITEM_SPEED_FS,
    ITEM_PID_P,
    ITEM_PID_I,
    ITEM_PID_D,
    ITEM_RESET,      // Reset to defaults
    ITEM_CALIBRATION // Start calibration
};

// Button debounce
const unsigned long DEBOUNCE_DELAY = 50;

// External declarations
extern MenuState currentMenu;
extern MenuItem selectedItem;
extern unsigned long lastMenuActivity;
extern bool editingValue;
extern unsigned long lastButtonPress;
extern bool hasUnsavedChanges;  // Solo dichiarazione extern

// Function declarations
void resetToDefaults();
void handleCalibration();

// Function declarations
void initializeMenu();
void processMenu();
void handleMenuSelection();
void handleBackButton();
void navigateMenu(bool up);
bool readButton(uint8_t pin);
void adjustValue(bool increase);

#endif 