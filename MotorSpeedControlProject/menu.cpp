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
#include <debounce.h>

// Menu global variables definition
MenuState currentMenu = MENU_NONE;
MenuItem selectedItem = ITEM_RUN;
unsigned long lastMenuActivity = 0;
bool editingValue = false;
unsigned long lastButtonPress = 0;
bool hasUnsavedChanges = false;
unsigned long currentMillis;

static void buttonUpHandler(uint8_t btnId, uint8_t btnState) {
    if (btnState == BTN_PRESSED) {
        lastMenuActivity = currentMillis;
        if (currentMenu == MENU_NONE && currentState == STATE_RUN) {
            // Se siamo nella schermata principale e in running, modifica setpoint
            adjustSetpoint(true);
        }
        else if (editingValue) {
            adjustValue(true);
        } else {
            navigateMenu(true);
        }
    } else {
        // btnState == BTN_OPEN.
    }
}

static void buttonDownHandler(uint8_t btnId, uint8_t btnState) {
    if (btnState == BTN_PRESSED) {
        lastMenuActivity = currentMillis;
        if (currentMenu == MENU_NONE && currentState == STATE_RUN) {
            // Se siamo nella schermata principale e in running, modifica setpoint
            adjustSetpoint(false);
        }
        else if (editingValue) {
            adjustValue(false);
        } else {
            navigateMenu(false);
        }
    } else {
        // btnState == BTN_OPEN.
    }
}

static void buttonEnterHandler(uint8_t btnId, uint8_t btnState) {
    if (btnState == BTN_PRESSED) {
        Serial.println("1");
        lastMenuActivity = currentMillis;
        handleMenuSelection();
    } else {
        // btnState == BTN_OPEN.
        Serial.println("0");
    }
}

static void buttonBackHandler(uint8_t btnId, uint8_t btnState) {
    if (btnState == BTN_PRESSED) {
        lastMenuActivity = currentMillis;
        handleBackButton();
    } else {
        // btnState == BTN_OPEN.
    }
}

// Initialize debounce objects
static Button buttonUp(0, buttonUpHandler);
static Button buttonDown(0, buttonDownHandler);
static Button buttonEnter(0, buttonEnterHandler);
static Button buttonBack(0, buttonBackHandler);

// Process menu navigation
void processMenu() {
    static unsigned long lastSaveCheck = 0;
    const unsigned long AUTO_SAVE_INTERVAL = 30000;  // 30 seconds
    
    currentMillis = millis();
    
    // Auto-save check
    if (hasUnsavedChanges && 
        (currentMillis - lastSaveCheck >= AUTO_SAVE_INTERVAL)) {
        ::saveParameters();
        if (currentMenu == MENU_PID) {
            updatePIDParameters();
        }
        hasUnsavedChanges = false;
        showMessage("Auto-saved");
        lastSaveCheck = currentMillis;
    }
    
    // Check for menu timeout
    if (currentMenu != MENU_NONE && 
        (currentMillis - lastMenuActivity >= MENU_TIMEOUT)) {
        currentMenu = MENU_NONE;
        editingValue = false;
        return;
    }
    
    // Update buttons state
    buttonUp.update(digitalRead(BUTTON_UP_PIN));
    buttonDown.update(digitalRead(BUTTON_DOWN_PIN));
    buttonEnter.update(digitalRead(BUTTON_ENTER_PIN));
    buttonBack.update(digitalRead(BUTTON_BACK_PIN));
    
}

// Handle menu selection
void handleMenuSelection() {
    switch(currentMenu) {
        case MENU_NONE:
            currentMenu = MENU_MAIN;
            selectedItem = (currentState == STATE_RUN) ? ITEM_STOP : ITEM_RUN;
            break;
            
        case MENU_MAIN:
            switch(selectedItem) {
                case ITEM_RUN:
                    if (currentState != STATE_ALARM) {
                        currentState = STATE_RUN;
                        currentMenu = MENU_NONE;
                    }
                    break;
                case ITEM_STOP:
                    currentState = STATE_IDLE;
                    pidSetpoint = 0;  // Reset setpoint
                    currentMenu = MENU_NONE;
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
                case ITEM_RESET:
                    showPopup("Warning", "Reset to defaults?", true);
                    break;
                case ITEM_CALIBRATION:
                    currentMenu = MENU_CALIBRATION;
                    handleCalibration();
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
            
        case MENU_CALIBRATION:
            // Handled by handleCalibration()
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
                if (selectedItem == ITEM_RUN || selectedItem == ITEM_STOP) 
                    selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_SETTINGS) 
                    selectedItem = (currentState == STATE_RUN) ? ITEM_STOP : ITEM_RUN;
                else if (selectedItem == ITEM_BACK) 
                    selectedItem = ITEM_SETTINGS;
            } else {
                if (selectedItem == ITEM_RUN || selectedItem == ITEM_STOP) 
                    selectedItem = ITEM_SETTINGS;
                else if (selectedItem == ITEM_SETTINGS) 
                    selectedItem = ITEM_BACK;
                else if (selectedItem == ITEM_BACK) 
                    selectedItem = (currentState == STATE_RUN) ? ITEM_STOP : ITEM_RUN;
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
        if (hasUnsavedChanges) {
            showPopup("Warning", "Save changes?", true);
            return;
        }
        editingValue = false;
        return;
    }
    
    switch(currentMenu) {
        case MENU_MAIN:
            currentMenu = MENU_NONE;
            break;
        case MENU_SETTINGS:
        case MENU_PID:
            if (hasUnsavedChanges) {
                showPopup("Warning", "Discard changes?", true);
                return;
            }
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
    bool limitReached = false;
    
    switch(selectedItem) {
        case ITEM_CURRENT_FS:
            if (increase) {
                if (systemParams.currentFullScale >= 50.0f) {
                    limitReached = true;
                } else {
                    systemParams.currentFullScale += STEP_LARGE;
                }
            } else {
                if (systemParams.currentFullScale <= 1.0f) {
                    limitReached = true;
                } else {
                    systemParams.currentFullScale = max(1.0f, systemParams.currentFullScale - STEP_LARGE);
                }
            }
            break;
            
        case ITEM_SPEED_FS:
            if (increase) {
                if (systemParams.speedFullScale >= 500) {
                    limitReached = true;
                } else {
                    systemParams.speedFullScale += STEP_SPEED;
                }
            } else {
                if (systemParams.speedFullScale <= 100) {
                    limitReached = true;
                } else {
                    systemParams.speedFullScale = max(100, systemParams.speedFullScale - STEP_SPEED);
                }
            }
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
    
    if (limitReached) {
        showMessage("Limit reached!");
    }
    hasUnsavedChanges = true;
}

// Reset to defaults
void resetToDefaults() {
    systemParams.currentFullScale = DEFAULT_CURRENT_FULL_SCALE;
    systemParams.speedFullScale = DEFAULT_SPEED_FULL_SCALE;
    systemParams.kp = DEFAULT_KP;
    systemParams.ki = DEFAULT_KI;
    systemParams.kd = DEFAULT_KD;
    saveParameters();
    updatePIDParameters();
    showMessage("Reset to defaults");
}

// Handle calibration
void handleCalibration() {
    static int calibrationStep = 0;
    
    switch(calibrationStep) {
        case 0:
            showPopup("Calibration", "Remove load", true);
            break;
        case 1:
            showPopup("Calibration", "Apply full load", true);
            break;
        case 2:
            showPopup("Calibration", "Calibration done!", false);
            calibrationStep = 0;
            return;
    }
    calibrationStep++;
}

// Add popup response handling
void handlePopupResponse(bool confirmed) {
    static MenuState previousMenu = MENU_NONE;
    
    if (confirmed) {
        if (editingValue) {
            ::saveParameters();
            if (currentMenu == MENU_PID) {
                updatePIDParameters();
            }
            editingValue = false;
            hasUnsavedChanges = false;
        }
        
        // Handle calibration confirmation
        if (currentMenu == MENU_CALIBRATION) {
            handleCalibration();
        }
    } else {
        // If not confirmed, revert to previous state
        if (editingValue) {
            loadParameters();  // Reload from EEPROM
            editingValue = false;
            hasUnsavedChanges = false;
        }
        currentMenu = previousMenu;
    }
}