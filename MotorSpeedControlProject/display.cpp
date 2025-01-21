/*
 * Display management implementation for DC Motor Speed Control Project
 */

#include "display.h"
#include "globals.h"
#include "states.h"
#include "menu.h"
#include "pins.h"
#include "alarms.h"  // Per getAlarmText()

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
    u8g2.drawStr(10, 20, "DC Motor Control");
    u8g2.drawStr(10, 35, "Version 1.0");
    u8g2.sendBuffer();
}

void updateDisplay() {
    static unsigned long lastUpdate = 0;
    unsigned long currentMillis = millis();
    
    // Update display only at specified intervals
    if (currentMillis - lastUpdate >= DISPLAY_UPDATE_INTERVAL) {
        u8g2.clearBuffer();
        
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

// ... rest of the implementation ... 