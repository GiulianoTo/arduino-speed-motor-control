/*
 * Logo bitmap declarations for DC Motor Speed Control Project
 */

#ifndef LOGO_H
#define LOGO_H

#include <avr/pgmspace.h>
#include <stdint.h>

#define logo_width 128
#define logo_height 128

// IIS Logo colors (16-bit 5-6-5 RGB format)
#define LOGO_BG    0x0000   // Black background
#define LOGO_BLUE  0x001F   // IIS Blue
#define LOGO_WHITE 0xFFFF   // White for highlights

// Function to decompress a single row of the logo with color
void decompressLogoRow(uint16_t* buffer, uint8_t row);

#endif 