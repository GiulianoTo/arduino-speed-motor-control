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
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

// Initialize display instance
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Colors for states
const uint16_t STATE_COLORS[] = {
    BLUE,    // STATE_IDLE
    GREEN,   // STATE_RUN
    RED      // STATE_ALARM
};

// Colors for menu items
const uint16_t MENU_COLORS[] = {
    WHITE,   // Normal text
    YELLOW,  // Selected item
    CYAN,    // Editing value
    RED      // Alarm/Error
};

// Global flag for display initialization status
static bool displayInitialized = false;

// Initialize display
void initializeDisplay() {
    tft.initR(INITR_BLACKTAB);  // Initialize ST7735S chip
    displayInitialized = true;   // Set flag if initialization succeeds
    
    tft.setRotation(2);         // Rotate 180 degrees if needed
    tft.fillScreen(BLACK);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
}

// Show splash screen
void showSplashScreen() {
    tft.fillScreen(BLACK);
    drawLogo(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    delay(2000);
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
    tft.fillScreen(BLACK);
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
    return !displayInitialized;  // Return initialization status
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
        tft.fillScreen(BLACK);
        
        // Draw header with colored background
        tft.fillRect(0, 0, 128, HEADER_HEIGHT, STATE_COLORS[currentState]);
        tft.setTextColor(BLACK);  // Black text on colored background
        tft.setCursor(2, 2);
        tft.print("Status: ");
        
        switch(currentState) {
            case STATE_IDLE:
                tft.print("IDLE");
                break;
            case STATE_RUN:
                tft.print("RUNNING");
                break;
            case STATE_ALARM:
                tft.print(getAlarmText());
                break;
        }
        
        // If in menu mode, show menu
        if (currentMenu != MENU_NONE) {
            drawMenuScreen();
        } else {
            // Show main operating screen with colored values
            char buffer[20];
            
            // Speed with color based on value
            uint16_t speedColor = map(currentSpeed, 0, systemParams.speedFullScale, 
                                    0x07, 0xE0);  // Blue to green gradient
            tft.setTextColor(speedColor);
            snprintf(buffer, sizeof(buffer), "Speed: %d RPM", (int)currentSpeed);
            tft.setCursor(0, MENU_START_Y);
            tft.print(buffer);
            
            // Current with warning colors
            uint16_t currentColor = currentCurrent > (systemParams.currentFullScale * 0.8) ? 
                                  RED : WHITE;
            tft.setTextColor(currentColor);
            snprintf(buffer, sizeof(buffer), "Current: %.1fA", currentCurrent);
            tft.setCursor(0, MENU_START_Y + LINE_HEIGHT);
            tft.print(buffer);
            
            // Setpoint in cyan
            if (currentState == STATE_RUN) {
                tft.setTextColor(CYAN);
                snprintf(buffer, sizeof(buffer), "Set: %d RPM", (int)pidSetpoint);
                tft.setCursor(0, MENU_START_Y + LINE_HEIGHT * 2);
                tft.print(buffer);
            }
        }
        
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
    tft.setTextColor(WHITE);
    
    switch(currentMenu) {
        case MENU_MAIN:
            drawMenuItem("Run", ITEM_RUN, MENU_START_Y, GREEN);
            drawMenuItem("Settings", ITEM_SETTINGS, MENU_START_Y + LINE_HEIGHT, CYAN);
            drawMenuItem("Back", ITEM_BACK, MENU_START_Y + LINE_HEIGHT * 2, WHITE);
            break;
            
        case MENU_SETTINGS:
            {
                char buffer[20];
                
                snprintf(buffer, sizeof(buffer), "Current: %.1fA", systemParams.currentFullScale);
                drawMenuItem(buffer, ITEM_CURRENT_FS, MENU_START_Y, YELLOW);
                
                snprintf(buffer, sizeof(buffer), "Speed: %dRPM", systemParams.speedFullScale);
                drawMenuItem(buffer, ITEM_SPEED_FS, MENU_START_Y + LINE_HEIGHT, MAGENTA);
                
                drawMenuItem("PID Settings", ITEM_PID_P, MENU_START_Y + LINE_HEIGHT * 2, CYAN);
                drawMenuItem("Back", ITEM_BACK, MENU_START_Y + LINE_HEIGHT * 3, WHITE);
            }
            break;
            
        case MENU_PID:
            {
                char buffer[20];
                uint16_t pidColor = 0x07E0;  // Base green for PID parameters
                
                snprintf(buffer, sizeof(buffer), "Kp: %.2f", systemParams.kp);
                drawMenuItem(buffer, ITEM_PID_P, MENU_START_Y, pidColor);
                
                snprintf(buffer, sizeof(buffer), "Ki: %.2f", systemParams.ki);
                drawMenuItem(buffer, ITEM_PID_I, MENU_START_Y + LINE_HEIGHT, pidColor + 0x0400);
                
                snprintf(buffer, sizeof(buffer), "Kd: %.2f", systemParams.kd);
                drawMenuItem(buffer, ITEM_PID_D, MENU_START_Y + LINE_HEIGHT * 2, pidColor + 0x0800);
                
                drawMenuItem("Back", ITEM_BACK, MENU_START_Y + LINE_HEIGHT * 3, WHITE);
            }
            break;
    }
}

// Draw single menu item with color
void drawMenuItem(const char* text, MenuItem item, uint8_t y, uint16_t color) {
    // Draw selection indicator
    if (selectedItem == item) {
        tft.fillRect(0, y-1, 128, LINE_HEIGHT, color);
        tft.setTextColor(BLACK);  // Black text on colored background
    } else {
        tft.setTextColor(color);  // Colored text on black background
    }
    
    // Draw menu item text
    tft.setCursor(10, y);
    tft.print(text);
    
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
        
        // Draw editing indicator and value
        static bool flash = false;
        static unsigned long lastFlash = 0;
        if (millis() - lastFlash > 300) {
            flash = !flash;
            lastFlash = millis();
        }
        
        if (flash) {
            tft.setTextColor(YELLOW);
        } else {
            tft.setTextColor(WHITE);
        }
        
        tft.setCursor(VALUE_X, y);
        tft.print(buffer);
    }
}

void drawLogo(uint8_t x, uint8_t y) {
    static uint16_t buffer[128];  // Buffer per una riga di pixel colorati
    
    // Disegna il logo una riga alla volta
    for(uint8_t row = 0; row < logo_height; row++) {
        decompressLogoRow(buffer, row);
        tft.drawRGBBitmap(x, y + row, buffer, logo_width, 1);
    }
}

// ... rest of the implementation ... 