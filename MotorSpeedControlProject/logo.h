/*
 * Logo bitmap for DC Motor Speed Control Project
 */

#ifndef LOGO_H
#define LOGO_H

#include <avr/pgmspace.h>
#include <stdint.h>

#define logo_width 128
#define logo_height 128

// Funzione per decomprimere il logo riga per riga
void decompressLogoRow(uint8_t* buffer, uint8_t row);

#endif 