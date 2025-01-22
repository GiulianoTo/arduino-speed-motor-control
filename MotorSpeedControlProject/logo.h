/*
 * Logo bitmap declarations for DC Motor Speed Control Project
 */

#ifndef LOGO_H
#define LOGO_H

#include <avr/pgmspace.h>
#include <stdint.h>

#define logo_width 128
#define logo_height 128

// Display colors (16-bit 5-6-5 RGB format)
#define BLACK    0x0000
#define WHITE    0xFFFF
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0

// Function to decompress a single row of the logo with color
void decompressLogoRow(uint16_t* buffer, uint8_t row);

#endif 