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
    // Draw message with fade effect
    static const uint16_t fadeColors[] = {0x0410, 0x0308, 0x0204, 0x0102};
    
    for(uint8_t i = 0; i < 4; i++) {
        tft.drawRect(0+i, HEADER_HEIGHT+i, 128-i*2, 20-i*2, fadeColors[i]);
    }
    
    tft.setTextColor(WHITE);
    tft.setCursor(4, HEADER_HEIGHT + 6);
    tft.print(message);
}

void showPopup(const char* title, const char* message, bool needConfirmation) {
    tft.fillScreen(BLACK);
    
    // Draw popup box with gradient border
    for(uint8_t i = 0; i < 3; i++) {
        uint16_t borderColor = 0x0410 + (i * 0x0410);  // Gradient blue
        tft.drawRect(10-i, 10-i, 108+i*2, 44+i*2, borderColor);
    }
    
    // Draw title with highlight
    tft.fillRect(11, 11, 106, 12, BLUE);
    tft.setTextColor(WHITE);
    tft.setCursor(12, 12);
    tft.print(title);
    
    // Draw message
    tft.setTextColor(CYAN);
    tft.setCursor(12, 24);
    tft.print(message);
    
    if (needConfirmation) {
        // Draw button with 3D effect
        tft.fillRect(20, 36, 88, 12, 0x0410);
        tft.drawRect(20, 36, 88, 12, WHITE);
        tft.setTextColor(WHITE);
        tft.setCursor(22, 38);
        tft.print("Press ENTER to continue");
    }
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
        
        // Draw header with gradient
        for(uint8_t i = 0; i < HEADER_HEIGHT; i++) {
            uint16_t gradientColor = STATE_COLORS[currentState] - (i * 0x0101);
            tft.drawFastHLine(0, i, 128, gradientColor);
        }
        
        // Draw status text with shadow effect
        tft.setTextColor(BLACK);
        tft.setCursor(3, 3);
        tft.print("Status: ");
        tft.setTextColor(WHITE);
        tft.setCursor(2, 2);
        tft.print("Status: ");
        
        switch(currentState) {
            case STATE_IDLE:
                tft.setTextColor(BLUE);
                tft.print("IDLE");
                break;
            case STATE_RUN:
                tft.setTextColor(GREEN);
                tft.print("RUNNING");
                break;
            case STATE_ALARM:
                // Blinking effect for alarm
                static bool alarmBlink = false;
                alarmBlink = !alarmBlink;
                tft.setTextColor(alarmBlink ? RED : YELLOW);
                tft.print(getAlarmText());
                break;
        }
        
        // Draw separator line with gradient
        for(uint8_t x = 0; x < 128; x++) {
            uint16_t lineColor = 0x0410 + (x * 0x0020);
            tft.drawPixel(x, HEADER_HEIGHT, lineColor);
        }
        
        // Rest of display update...
        if (currentMenu != MENU_NONE) {
            drawMenuScreen();
        } else {
            // Show main screen with enhanced colors
            char buffer[20];
            
            // Speed gauge with color gradient
            int speedPercent = map(currentSpeed, 0, systemParams.speedFullScale, 0, 100);
            uint16_t speedColor = map(speedPercent, 0, 100, 0x001F, 0x07E0);
            tft.fillRect(0, MENU_START_Y, speedPercent * 1.28, 8, speedColor);
            tft.drawRect(0, MENU_START_Y, 128, 8, WHITE);
            
            tft.setTextColor(speedColor);
            snprintf(buffer, sizeof(buffer), "Speed: %d RPM", (int)currentSpeed);
            tft.setCursor(0, MENU_START_Y + 10);
            tft.print(buffer);
            
            // Current bar with warning colors
            int currentPercent = map(currentCurrent, 0, systemParams.currentFullScale, 0, 100);
            uint16_t currentColor;
            if (currentPercent > 90) currentColor = RED;
            else if (currentPercent > 75) currentColor = YELLOW;
            else currentColor = GREEN;
            
            tft.fillRect(0, MENU_START_Y + LINE_HEIGHT + 2, currentPercent * 1.28, 8, currentColor);
            tft.drawRect(0, MENU_START_Y + LINE_HEIGHT + 2, 128, 8, WHITE);
            
            tft.setTextColor(currentColor);
            snprintf(buffer, sizeof(buffer), "Current: %.1fA", currentCurrent);
            tft.setCursor(0, MENU_START_Y + LINE_HEIGHT + 12);
            tft.print(buffer);
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