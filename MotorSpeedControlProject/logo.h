/*
 * Logo bitmap declarations for DC Motor Speed Control Project
 */

#ifndef LOGO_H
#define LOGO_H

#include <avr/pgmspace.h>
#include <stdint.h>

#define logo_width 128
#define logo_height 64

// Function to decompress a single row of the logo
void decompressLogoRow(uint8_t* buffer, uint8_t row);

#endif 