/*
 * Display management implementation for DC Motor Speed Control Project
 */

#include "display.h"
#include "globals.h"
#include "states.h"
#include "menu.h"
#include "pins.h"
#include "alarms.h"  // For getAlarmText()
#include "logo.h"    // For logo bitmap

// Initialize display
void initializeDisplay() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
}

// Show splash screen
void showSplashScreen() {
    u8g2.clearBuffer();
    
    // Draw full screen logo
    drawLogo(0, 0);  // Logo occupa tutto lo schermo
    
    u8g2.sendBuffer();
    delay(2000);  // Mostra il logo per 2 secondi
}

// Global variables for message handling
static char currentMessage[32] = "";
static unsigned long messageStartTime = 0;
static bool messageActive = false;

// Global variables for popup handling
static char popupTitle[16] = "";
static char popupMessage[32] = "";
static bool popupActive = false;
static bool popupNeedsConfirmation = false;
static unsigned long popupStartTime = 0;

void clearDisplay() {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
}

void showMessage(const char* message) {
    strncpy(currentMessage, message, sizeof(currentMessage) - 1);
    messageStartTime = millis();
    messageActive = true;
}

void showPopup(const char* title, const char* message, bool needConfirmation) {
    strncpy(popupTitle, title, sizeof(popupTitle) - 1);
    strncpy(popupMessage, message, sizeof(popupMessage) - 1);
    popupActive = true;
    popupNeedsConfirmation = needConfirmation;
    popupStartTime = millis();
}

bool isDisplayError() {
    return !u8g2.begin();  // Returns false if display initialization failed
}

void handleDisplayError() {
    // If we can't use the display, use LED indicators
    digitalWrite(RGB_RED_PIN, HIGH);
    digitalWrite(RGB_GREEN_PIN, LOW);
    digitalWrite(RGB_BLUE_PIN, LOW);
}

// Update the existing updateDisplay() to handle messages and popups
void updateDisplay() {
    static unsigned long lastUpdate = 0;
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastUpdate >= DISPLAY_UPDATE_INTERVAL) {
        u8g2.clearBuffer();
        
        // Check for active popup
        if (popupActive) {
            // Draw popup box
            u8g2.drawFrame(10, 10, 108, 44);
            u8g2.drawStr(12, 12, popupTitle);
            u8g2.drawHLine(10, 22, 108);
            u8g2.drawStr(12, 24, popupMessage);
            
            if (popupNeedsConfirmation) {
                u8g2.drawStr(12, 36, "Press ENTER to continue");
            } else if (currentMillis - popupStartTime >= POPUP_TIMEOUT) {
                popupActive = false;
            }
            
            u8g2.sendBuffer();
            lastUpdate = currentMillis;
            return;
        }
        
        // Check for active message
        if (messageActive) {
            if (currentMillis - messageStartTime >= MESSAGE_DISPLAY_TIME) {
                messageActive = false;
            } else {
                u8g2.drawStr(0, HEADER_HEIGHT, currentMessage);
                u8g2.sendBuffer();
                lastUpdate = currentMillis;
                return;
            }
        }
        
        // Normal display update
        // Draw header with system state
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(0, 0, "Status:");
        switch(currentState) {
            case STATE_IDLE:
                u8g2.drawStr(50, 0, "IDLE");
                break;
            case STATE_RUN:
                u8g2.drawStr(50, 0, "RUNNING");
                break;
            case STATE_ALARM:
                u8g2.drawStr(50, 0, getAlarmText());
                break;
        }
        
        // If in menu mode, show menu
        if (currentMenu != MENU_NONE) {
            drawMenuScreen();
        } else {
            // Show main operating screen
            char buffer[20];
            
            // Show current speed
            snprintf(buffer, sizeof(buffer), "Speed: %d RPM", (int)currentSpeed);
            u8g2.drawStr(0, MENU_START_Y, buffer);
            
            // Show current current
            snprintf(buffer, sizeof(buffer), "Current: %.1fA", currentCurrent);
            u8g2.drawStr(0, MENU_START_Y + LINE_HEIGHT, buffer);
            
            // Show setpoint if running
            if (currentState == STATE_RUN) {
                snprintf(buffer, sizeof(buffer), "Set: %d RPM", (int)pidSetpoint);
                u8g2.drawStr(0, MENU_START_Y + LINE_HEIGHT * 2, buffer);
            }
        }
        
        u8g2.sendBuffer();
        lastUpdate = currentMillis;
    }
}

void updateLedBar() {
    static unsigned long lastUpdate = 0;
    unsigned long currentMillis = millis();
    
    // Update LED bar only at specified intervals
    if (currentMillis - lastUpdate >= LED_BAR_UPDATE_INTERVAL) {
        // Map current speed to LED bar range
        int ledCount = map(currentSpeed, 0, systemParams.speedFullScale, 0, LED_BAR_COUNT);
        
        // Update each LED
        for(uint8_t i = 0; i < LED_BAR_COUNT; i++) {
            digitalWrite(LED_BAR_PINS[i], i < ledCount ? HIGH : LOW);
        }
        
        lastUpdate = currentMillis;
    }
}

// Draw menu screen
void drawMenuScreen() {
    u8g2.setFont(u8g2_font_6x10_tf);
    
    switch(currentMenu) {
        case MENU_MAIN:
            drawMenuItem("Run", ITEM_RUN, MENU_START_Y);
            drawMenuItem("Settings", ITEM_SETTINGS, MENU_START_Y + LINE_HEIGHT);
            drawMenuItem("Back", ITEM_BACK, MENU_START_Y + LINE_HEIGHT * 2);
            break;
            
        case MENU_SETTINGS:
            {
                char buffer[20];
                
                snprintf(buffer, sizeof(buffer), "Current: %.1fA", systemParams.currentFullScale);
                drawMenuItem(buffer, ITEM_CURRENT_FS, MENU_START_Y);
                
                snprintf(buffer, sizeof(buffer), "Speed: %dRPM", systemParams.speedFullScale);
                drawMenuItem(buffer, ITEM_SPEED_FS, MENU_START_Y + LINE_HEIGHT);
                
                drawMenuItem("PID Settings", ITEM_PID_P, MENU_START_Y + LINE_HEIGHT * 2);
                drawMenuItem("Back", ITEM_BACK, MENU_START_Y + LINE_HEIGHT * 3);
            }
            break;
            
        case MENU_PID:
            {
                char buffer[20];
                
                snprintf(buffer, sizeof(buffer), "Kp: %.2f", systemParams.kp);
                drawMenuItem(buffer, ITEM_PID_P, MENU_START_Y);
                
                snprintf(buffer, sizeof(buffer), "Ki: %.2f", systemParams.ki);
                drawMenuItem(buffer, ITEM_PID_I, MENU_START_Y + LINE_HEIGHT);
                
                snprintf(buffer, sizeof(buffer), "Kd: %.2f", systemParams.kd);
                drawMenuItem(buffer, ITEM_PID_D, MENU_START_Y + LINE_HEIGHT * 2);
                
                drawMenuItem("Back", ITEM_BACK, MENU_START_Y + LINE_HEIGHT * 3);
            }
            break;
    }
}

// Draw single menu item
void drawMenuItem(const char* text, MenuItem item, uint8_t y) {
    // Draw selection indicator
    if (selectedItem == item) {
        u8g2.drawStr(0, y, ">");
        
        // Draw edit indicator if editing
        if (editingValue) {
            u8g2.drawStr(VALUE_X - 10, y, "*");
        }
    }
    
    // Draw menu item text
    u8g2.drawStr(10, y, text);
    
    // If editing this item, draw value with edit indicator
    if (editingValue && selectedItem == item) {
        char buffer[10];
        switch(item) {
            case ITEM_CURRENT_FS:
                snprintf(buffer, sizeof(buffer), "%.1f", systemParams.currentFullScale);
                break;
            case ITEM_SPEED_FS:
                snprintf(buffer, sizeof(buffer), "%d", systemParams.speedFullScale);
                break;
            case ITEM_PID_P:
                snprintf(buffer, sizeof(buffer), "%.2f", systemParams.kp);
                break;
            case ITEM_PID_I:
                snprintf(buffer, sizeof(buffer), "%.2f", systemParams.ki);
                break;
            case ITEM_PID_D:
                snprintf(buffer, sizeof(buffer), "%.2f", systemParams.kd);
                break;
            default:
                buffer[0] = '\0';
                break;
        }
        u8g2.drawStr(VALUE_X, y, buffer);
    }
}

void drawLogo(uint8_t x, uint8_t y) {
    static uint8_t buffer[16];  // Buffer per una singola riga
    
    // Disegna il logo una riga alla volta
    for(uint8_t row = 0; row < logo_height; row++) {
        decompressLogoRow(buffer, row);
        u8g2.drawXBM(x, y + row, logo_width, 1, buffer);
    }
}

// ... rest of the implementation ... 