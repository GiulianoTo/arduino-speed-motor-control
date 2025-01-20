/*
 * EEPROM management declarations for DC Motor Speed Control Project
 */

#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <EEPROM.h>
#include "config.h"

// EEPROM validation
const uint16_t EEPROM_VALID_KEY = 0xABCD;
const int EEPROM_KEY_ADDR = 0;

// Function declarations
bool isEEPROMValid();
void initializeEEPROM();
void loadParameters();
void saveParameters();

#endif 