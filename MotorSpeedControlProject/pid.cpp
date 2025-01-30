/*
 * PID control implementation for DC Motor Speed Control Project
 */

#include "pid.h"
#include "pins.h"
#include "globals.h"

// Function to update PID parameters
void updatePIDParameters() {
    motorPID.SetTunings(systemParams.kp, systemParams.ki, systemParams.kd);
}

// Function to reset PID controller
void resetPID() {
    motorPID.SetMode(MANUAL);
    pidOutput = 0;
    motorPID.SetMode(AUTOMATIC);
}

// Function to process PID control
void processPID() {
    static unsigned long lastPIDCompute = 0;
    unsigned long currentMillis = millis();
    
    if (currentState == STATE_RUN && 
        (currentMillis - lastPIDCompute >= PID_COMPUTE_INTERVAL)) {
        
        // Update input
        pidInput = currentSpeed;
        
        // Compute new output
        if (motorPID.Compute()) {
            // Apply output only if not in alarm state
            if (currentState != STATE_ALARM) {
                analogWrite(MOTOR_PWM_PIN, pidOutput);
            }else{
                analogWrite(MOTOR_PWM_PIN, 0);
            }
        }
        
        lastPIDCompute = currentMillis;
    }
}

// Function to set speed setpoint
void setSpeedSetpoint(double newSetpoint) {
    // Limit setpoint to valid range
    if (newSetpoint < 0) {
        newSetpoint = 0;
    } else if (newSetpoint > systemParams.speedFullScale) {
        newSetpoint = systemParams.speedFullScale;
    }
    
    pidSetpoint = newSetpoint;
}

// Function to adjust setpoint incrementally
void adjustSetpoint(bool increase) {
    const double SETPOINT_STEP = 50.0;  // RPM
    double newSetpoint = pidSetpoint;
    
    if (increase) {
        newSetpoint += SETPOINT_STEP;
    } else {
        newSetpoint -= SETPOINT_STEP;
    }
    
    setSpeedSetpoint(newSetpoint);
} 