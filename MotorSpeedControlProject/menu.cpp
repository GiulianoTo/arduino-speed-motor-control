/*
 * Menu system implementation for DC Motor Speed Control Project
 */

#include "menu.h"
#include "globals.h"
#include "pins.h"
#include "display.h"
#include "eeprom_manager.h"  // For saveParameters()
#include "pid.h"            // For updatePIDParameters()
#include "states.h"         // For SystemState and currentState

// Menu global variables definition
MenuState currentMenu = MENU_NONE;
MenuItem selectedItem = ITEM_RUN;
unsigned long lastMenuActivity = 0;
bool editingValue = false;
unsigned long lastButtonPress = 0;

// Read buttons with debounce
bool readButton(uint8_t pin) {
    static unsigned long lastDebounceTime = 0;
    static int lastButtonState = HIGH;
    bool buttonPressed = false;
    
    int reading = digitalRead(pin);
    
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading == LOW) {  // Button pressed (active low)
            buttonPressed = true;
            lastMenuActivity = millis();
        }
    }
    
    lastButtonState = reading;
    return buttonPressed;
}

// Process menu navigation
void processMenu() {
    static unsigned long lastButtonCheck = 0;
    unsigned long currentMillis = millis();
    
    // Check for menu timeout
    if (currentMenu != MENU_NONE && 
        (currentMillis - lastMenuActivity >= MENU_TIMEOUT)) {
        currentMenu = MENU_NONE;
        editingValue = false;
        return;
    }
    
    // Check buttons at debounce interval
    if (currentMillis - lastButtonCheck >= DEBOUNCE_DELAY) {
        // Read buttons
        if (readButton(BUTTON_UP_PIN)) {
            lastMenuActivity = currentMillis;
            if (editingValue) {
                adjustValue(true);
            } else {
                navigateMenu(true);
            }
        }
        
        if (readButton(BUTTON_DOWN_PIN)) {
            lastMenuActivity = currentMillis;
            if (editingValue) {
                adjustValue(false);
            } else {
                navigateMenu(false);
            }
        }
        
        if (readButton(BUTTON_ENTER_PIN)) {
            lastMenuActivity = currentMillis;
            handleMenuSelection();
        }
        
        if (readButton(BUTTON_BACK_PIN)) {
            lastMenuActivity = currentMillis;
            handleBackButton();
        }
        
        lastButtonCheck = currentMillis;
    }
}

// Handle menu selection
void handleMenuSelection() {
    switch(currentMenu) {
        case MENU_NONE:
            currentMenu = MENU_MAIN;
            selectedItem = ITEM_RUN;
            break;
            
        case MENU_MAIN:
            switch(selectedItem) {
                case ITEM_RUN:
                    if (currentState != STATE_ALARM) {
                        currentState = STATE_RUN;
                        currentMenu = MENU_NONE;
                    }
                    break;
                case ITEM_SETTINGS:
                    currentMenu = MENU_SETTINGS;
                    selectedItem = ITEM_CURRENT_FS;
                    break;
                case ITEM_BACK:
                    currentMenu = MENU_NONE;
                    break;
            }
            break;
            
        case MENU_SETTINGS:
            switch(selectedItem) {
                case ITEM_CURRENT_FS:
                case ITEM_SPEED_FS:
                    editingValue = !editingValue;
                    if (!editingValue) {
                        ::saveParameters();
                    }
                    break;
                case ITEM_PID_P:
                    currentMenu = MENU_PID;
                    selectedItem = ITEM_PID_P;
                    break;
                case ITEM_BACK:
                    currentMenu = MENU_MAIN;
                    selectedItem = ITEM_RUN;
                    break;
            }
            break;
            
        case MENU_PID:
            switch(selectedItem) {
                case ITEM_PID_P:
                case ITEM_PID_I:
                case ITEM_PID_D:
                    editingValue = !editingValue;
                    if (!editingValue) {
                        ::saveParameters();
                        updatePIDParameters();
                    }
                    break;
                case ITEM_BACK:
                    currentMenu = MENU_SETTINGS;
                    selectedItem = ITEM_PID_P;
                    break;
            }
            break;
    }
}

// Initialize menu system
void initializeMenu() {
    currentMenu = MENU_NONE;
    selectedItem = ITEM_RUN;
    editingValue = false;
    lastMenuActivity = millis();
}

// Navigate menu up or down
void navigateMenu(bool up) {
    switch(currentMenu) {
        case MENU_MAIN:
            if (up) {
                if (selectedItem == ITEM_RUN) selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_SETTINGS) selectedItem = ITEM_RUN;
                else if (selectedItem == ITEM_BACK) selectedItem = ITEM_SETTINGS;
            } else {
                if (selectedItem == ITEM_RUN) selectedItem = ITEM_SETTINGS;
                else if (selectedItem == ITEM_SETTINGS) selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_BACK) selectedItem = ITEM_RUN;
            }
            break;
            
        case MENU_SETTINGS:
            if (up) {
                if (selectedItem == ITEM_CURRENT_FS) selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_SPEED_FS) selectedItem = ITEM_CURRENT_FS;
                else if (selectedItem == ITEM_PID_P) selectedItem = ITEM_SPEED_FS;
                else if (selectedItem == ITEM_BACK) selectedItem = ITEM_PID_P;
            } else {
                if (selectedItem == ITEM_CURRENT_FS) selectedItem = ITEM_SPEED_FS;
                else if (selectedItem == ITEM_SPEED_FS) selectedItem = ITEM_PID_P;
                else if (selectedItem == ITEM_PID_P) selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_BACK) selectedItem = ITEM_CURRENT_FS;
            }
            break;
            
        case MENU_PID:
            if (up) {
                if (selectedItem == ITEM_PID_P) selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_PID_I) selectedItem = ITEM_PID_P;
                else if (selectedItem == ITEM_PID_D) selectedItem = ITEM_PID_I;
                else if (selectedItem == ITEM_BACK) selectedItem = ITEM_PID_D;
            } else {
                if (selectedItem == ITEM_PID_P) selectedItem = ITEM_PID_I;
                else if (selectedItem == ITEM_PID_I) selectedItem = ITEM_PID_D;
                else if (selectedItem == ITEM_PID_D) selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_BACK) selectedItem = ITEM_PID_P;
            }
            break;
    }
}

// Handle back button
void handleBackButton() {
    if (editingValue) {
        editingValue = false;
        return;
    }
    
    switch(currentMenu) {
        case MENU_MAIN:
            currentMenu = MENU_NONE;
            break;
        case MENU_SETTINGS:
        case MENU_PID:
            currentMenu = MENU_MAIN;
            selectedItem = ITEM_RUN;
            break;
        default:
            break;
    }
}

// Adjust value being edited
void adjustValue(bool increase) {
    const float STEP_SMALL = 0.1f;
    const float STEP_LARGE = 1.0f;
    const int STEP_SPEED = 100;
    
    switch(selectedItem) {
        case ITEM_CURRENT_FS:
            systemParams.currentFullScale += increase ? STEP_LARGE : -STEP_LARGE;
            if (systemParams.currentFullScale < 1.0f) systemParams.currentFullScale = 1.0f;
            break;
            
        case ITEM_SPEED_FS:
            systemParams.speedFullScale += increase ? STEP_SPEED : -STEP_SPEED;
            if (systemParams.speedFullScale < 100) systemParams.speedFullScale = 100;
            break;
            
        case ITEM_PID_P:
            systemParams.kp += increase ? STEP_SMALL : -STEP_SMALL;
            if (systemParams.kp < 0.0f) systemParams.kp = 0.0f;
            break;
            
        case ITEM_PID_I:
            systemParams.ki += increase ? STEP_SMALL : -STEP_SMALL;
            if (systemParams.ki < 0.0f) systemParams.ki = 0.0f;
            break;
            
        case ITEM_PID_D:
            systemParams.kd += increase ? STEP_SMALL : -STEP_SMALL;
            if (systemParams.kd < 0.0f) systemParams.kd = 0.0f;
            break;
            
        default:
            break;
    }
}