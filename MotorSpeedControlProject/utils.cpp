/*
 * Utility functions implementation for DC Motor Speed Control Project
 */

#include "utils.h"
#include "globals.h"
#include "pid.h"

// Function to adjust selected parameter value
void adjustSelectedValue(bool increase) {
    switch(selectedItem) {
        case ITEM_CURRENT_FS:
            if (increase) {
                systemParams.currentFullScale += CURRENT_FS_STEP;
            } else {
                systemParams.currentFullScale = max(CURRENT_FS_STEP, 
                    systemParams.currentFullScale - CURRENT_FS_STEP);
            }
            break;
            
        case ITEM_SPEED_FS:
            if (increase) {
                systemParams.speedFullScale += SPEED_FS_STEP;
            } else {
                systemParams.speedFullScale = max(SPEED_FS_STEP, 
                    systemParams.speedFullScale - SPEED_FS_STEP);
            }
            break;
            
        case ITEM_PID_P:
            if (increase) {
                systemParams.kp += PID_P_STEP;
            } else {
                systemParams.kp = max(0.0f, systemParams.kp - PID_P_STEP);
            }
            updatePIDParameters();
            break;
            
        case ITEM_PID_I:
            if (increase) {
                systemParams.ki += PID_I_STEP;
            } else {
                systemParams.ki = max(0.0f, systemParams.ki - PID_I_STEP);
            }
            updatePIDParameters();
            break;
            
        case ITEM_PID_D:
            if (increase) {
                systemParams.kd += PID_D_STEP;
            } else {
                systemParams.kd = max(0.0f, systemParams.kd - PID_D_STEP);
            }
            updatePIDParameters();
            break;
    }
}

// Function to handle back button in menus
void handleBackButton() {
    switch(currentMenu) {
        case MENU_MAIN:
            currentMenu = MENU_NONE;
            break;
        case MENU_SETTINGS:
            currentMenu = MENU_MAIN;
            selectedItem = ITEM_SETTINGS;
            break;
        case MENU_PID:
            currentMenu = MENU_SETTINGS;
            selectedItem = ITEM_PID_P;
            break;
        case MENU_CALIBRATION:
            currentMenu = MENU_SETTINGS;
            break;
        default:
            break;
    }
}

// Function to navigate menu items
void navigateMenu(bool up) {
    switch(currentMenu) {
        case MENU_MAIN:
            if (up) {
                if (selectedItem > ITEM_RUN) {
                    selectedItem = (MenuItem)(selectedItem - 1);
                }
            } else {
                if (selectedItem < ITEM_SETTINGS) {
                    selectedItem = (MenuItem)(selectedItem + 1);
                }
            }
            break;
            
        case MENU_SETTINGS:
            if (up) {
                if (selectedItem > ITEM_CURRENT_FS) {
                    selectedItem = (MenuItem)(selectedItem - 1);
                }
            } else {
                if (selectedItem < ITEM_PID_P) {
                    selectedItem = (MenuItem)(selectedItem + 1);
                }
            }
            break;
            
        case MENU_PID:
            if (up) {
                if (selectedItem > ITEM_PID_P) {
                    selectedItem = (MenuItem)(selectedItem - 1);
                }
            } else {
                if (selectedItem < ITEM_PID_D) {
                    selectedItem = (MenuItem)(selectedItem + 1);
                }
            }
            break;
    }
} 