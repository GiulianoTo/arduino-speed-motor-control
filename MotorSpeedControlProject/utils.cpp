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