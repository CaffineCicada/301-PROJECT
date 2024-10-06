/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef MOTOR_POWER_CONTROL_H
#define MOTOR_POWER_CONTROL_H

#define PROP_COEFF 0.8
#define DELTA_COEFF 0.2
#define INT_COEFF 0.25

#include <project.h>
#include <stdbool.h>
#include <math.h>

// Helper clamping function
float clamp(float val, float minval, float maxval )
{
    if(val < minval) {
        return minval;   
    }
    if(val > minval) {
        return maxval;   
    }
    return val;
}

    
// Sets the power of the left motor.
// 1.0 is full power forward. -1.0 is full power backward
void setLeftPower(float power) {
    power = power*128 + 128;
    if(power < 0) {
        power = 0;   
    } else if(power > UINT8_MAX) {
        power = UINT8_MAX;   
    }
    PWM_1_WriteCompare((uint8) power);
}

// Sets the power of the right motor.
// 1.0 is full power forward. -1.0 is full power backward
void setRightPower(float power) {
    // Flip power value since motor is mounted opposite direction compared to left
    power = power * -1;
    power = power*128 + 128;
    if(power < 0) {
        power = 0;   
    } else if(power > UINT8_MAX) {
        power = UINT8_MAX;
    }
    PWM_2_WriteCompare((uint8) power);
}

int16 getLeftCounter() {
    return QuadDec_M1_GetCounter() * -1;   
}

int16 getRightCounter() {
    return QuadDec_M2_GetCounter();   
}

// Resets decoder counters to 0
void resetCounters() {
    QuadDec_M1_SetCounter(0);
    QuadDec_M2_SetCounter(0);
    return;   
}

struct MotorPID {
    int16 leftDistCount, rightDistCount;
    int16 leftDistDelta, rightDistDelta;
    float leftPowerTarget, rightPowerTarget;
    float leftPower, rightPower;
    float sideError, sideErrorDelta, sideErrorInt;
};

struct MotorPID newMotorPID(float leftPowerTarget, float rightPowerTarget) {
    struct MotorPID pid;
    
    pid.leftDistCount = 0;
    pid.rightDistCount = 0;
    pid.leftDistDelta = 0;
    pid.rightDistDelta = 0;
    pid.leftPowerTarget = leftPowerTarget;
    pid.rightPowerTarget = rightPowerTarget;
    pid.leftPower = leftPowerTarget;
    pid.rightPower = rightPowerTarget;
    pid.sideError = 0;
    pid.sideErrorDelta = 0;
    pid.sideErrorInt = 0;
    
    return pid;
}

// Set new power targets for the motor controller to achieve.
//
// Power targets should be between -1.0 and 1.0 (inclusive).
void setPowerTargets(struct MotorPID* pid, float leftPowerTarget, float rightPowerTarget) {
    pid->leftPowerTarget = leftPowerTarget; 
    pid->leftPower = leftPowerTarget;
    pid->rightPowerTarget = rightPowerTarget;
    pid->rightPower = rightPowerTarget;
}

// Resets distance counters of PID to 0
void resetDistCounts(struct MotorPID* pid) {
    pid->leftDistCount = 0;
    pid->rightDistCount = 0;
}

void updateMotorPID(struct MotorPID* pid, int16 newLDist, int16 newRDist) {
    
    // Update deltas
    pid->leftDistDelta = newLDist - pid->leftDistCount;
    pid->rightDistDelta = newRDist - pid->rightDistCount;
    
    // Update counts
    pid->leftDistCount = newLDist;
    pid->rightDistCount = newRDist;
    
    // Calculate errors
    float sideError = pid->leftDistDelta*pid->rightPowerTarget - pid->rightDistDelta*pid->leftPowerTarget;
    pid->sideErrorDelta = sideError - pid->sideError;
    pid->sideError = sideError;
    pid->sideErrorInt = pid->leftDistCount*pid->rightPowerTarget - pid->rightDistCount*pid->leftPowerTarget;
    float error = pid->sideError*PROP_COEFF + pid->sideErrorDelta*DELTA_COEFF + pid->sideErrorInt*INT_COEFF;
    
    pid->leftPower -= error;
    pid->rightPower += error;
    
    pid->leftPower = clamp(pid->leftPower, -fabs(pid->leftPowerTarget), fabs(pid->leftPowerTarget));
    pid->rightPower = clamp(pid->rightPower, -fabs(pid->rightPowerTarget), fabs(pid->rightPowerTarget));
    
}

#endif



