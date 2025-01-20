/*
 * Menu system implementation for DC Motor Speed Control Project
 */

#include "menu.h"
#include "globals.h"
#include "pins.h"
#include "display.h"

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
    // ... implementazione ...
}

// Handle menu selection
void handleMenuSelection() {
    // ... implementazione ...
}

// Initialize menu system
void initializeMenu() {
    currentMenu = MENU_NONE;
    selectedItem = ITEM_RUN;
    editingValue = false;
    lastMenuActivity = millis();
}