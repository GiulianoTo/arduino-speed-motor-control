/*
 * EEPROM management implementation for DC Motor Speed Control Project
 */

#include "eeprom_manager.h"
#include "globals.h"

// Function to check if EEPROM contains valid data
bool isEEPROMValid() {
    uint16_t storedKey;
    EEPROM.get(EEPROM_KEY_ADDR, storedKey);
    return storedKey == EEPROM_VALID_KEY;
}

// Function to initialize EEPROM with default values
void initializeEEPROM() {
    // Write validation key
    EEPROM.put(EEPROM_KEY_ADDR, EEPROM_VALID_KEY);
    
    // Write default parameters
    systemParams.currentFullScale = DEFAULT_CURRENT_FULL_SCALE;
    systemParams.speedFullScale = DEFAULT_SPEED_FULL_SCALE;
    systemParams.kp = DEFAULT_KP;
    systemParams.ki = DEFAULT_KI;
    systemParams.kd = DEFAULT_KD;
    
    saveParameters();
}

// Load parameters from EEPROM
void loadParameters() {
    // Read parameters from EEPROM
    EEPROM.get(EEPROM_CURRENT_FS_ADDR, systemParams.currentFullScale);
    EEPROM.get(EEPROM_SPEED_FS_ADDR, systemParams.speedFullScale);
    EEPROM.get(EEPROM_KP_ADDR, systemParams.kp);
    EEPROM.get(EEPROM_KI_ADDR, systemParams.ki);
    EEPROM.get(EEPROM_KD_ADDR, systemParams.kd);
    
    // Check if values are valid, if not load defaults
    if (isnan(systemParams.currentFullScale) || systemParams.currentFullScale <= 0) {
        systemParams.currentFullScale = DEFAULT_CURRENT_FULL_SCALE;
    }
    if (systemParams.speedFullScale <= 0) {
        systemParams.speedFullScale = DEFAULT_SPEED_FULL_SCALE;
    }
    if (isnan(systemParams.kp)) {
        systemParams.kp = DEFAULT_KP;
    }
    if (isnan(systemParams.ki)) {
        systemParams.ki = DEFAULT_KI;
    }
    if (isnan(systemParams.kd)) {
        systemParams.kd = DEFAULT_KD;
    }
}

// Save parameters to EEPROM
void saveParameters() {
    EEPROM.put(EEPROM_CURRENT_FS_ADDR, systemParams.currentFullScale);
    EEPROM.put(EEPROM_SPEED_FS_ADDR, systemParams.speedFullScale);
    EEPROM.put(EEPROM_KP_ADDR, systemParams.kp);
    EEPROM.put(EEPROM_KI_ADDR, systemParams.ki);
    EEPROM.put(EEPROM_KD_ADDR, systemParams.kd);
} 