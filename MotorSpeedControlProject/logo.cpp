/*
 * Logo bitmap implementation for DC Motor Speed Control Project
 */

#include "logo.h"
#include <string.h>
#include "globals.h"



void Logo(void) {
  u8g2.setFontDirection(0);
  u8g2.setFont(u8g2_font_inb24_mf);
  u8g2.drawStr(5, 0, "IIS");
  u8g2.drawStr(5, 30, "Lonigo");

  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(5,57,"DC Motor Control V1.0");
}
