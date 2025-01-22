/*
 * Display management declarations for DC Motor Speed Control Project
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "config.h"
#include "menu.h"
#include "alarms.h"  // For getAlarmText()

// Display pins
#define TFT_CS    10  // CS pin
#define TFT_RST   9   // Reset pin
#define TFT_DC    8   // DC pin (A0)

// Display colors (16-bit 5-6-5 RGB format)
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

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
void drawMenuItem(const char* text, MenuItem item, uint8_t y, uint16_t color);
void drawLogo(uint8_t x, uint8_t y);

// New functions
void clearDisplay();
void showMessage(const char* message);
void showPopup(const char* title, const char* message, bool needConfirmation = false);
bool isDisplayError();
void handleDisplayError();

extern Adafruit_ST7735 tft;  // Display instance

#endif 