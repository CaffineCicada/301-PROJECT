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

// Returns the value from the right decoder.
//
// Values >0 signify the left wheel has travelled forwards.
int16 getLeftCounter() {
    // Flip decoder value due to physical mounting
    return QuadDec_M1_GetCounter() * -1;   
}

// Returns the value from the left decoder.
//
// Values >0 signify the right wheel has travelled forwards.
int16 getRightCounter() {
    return QuadDec_M2_GetCounter();   
}

// Resets decoder counters to 0
void resetCounters() {
    QuadDec_M1_SetCounter(0);
    QuadDec_M2_SetCounter(0);
    return;   
}

// Stores all the information for the PID motor controller to function/calculate on.
//
// Probably, the only member variables that should be accessed externally are:
//  - leftPowerTarget, rightPowerTarget
//  - leftPower, rightPower
struct MotorPID {
    int16 leftDistCount, rightDistCount;
    int16 leftDistDelta, rightDistDelta;
    float sideError, sideErrorDelta, sideErrorInt;

    // The power target given for the PID controller to achieve for the left motor. This should not be manipulated externally
    // and should instead be manipulated by setPowerTargets(...)
    float leftPowerTarget;
    // The power target given for the PID controller to achieve for the right motor. This should not be manipulated externally
    // and should instead be manipulated by setPowerTargets(...)
    float rightPowerTarget;

    // The power the PID controller wants to send to the left wheel. This number should be fed to
    // setLeftPower(...)
    float leftPower;
    // The power the PID controller wants to send to the right wheel. This number should be fed to
    // setRightPower(...)
    float rightPower;
};

// Constructs a new instance of MotorPID with appropriate starting values.
//
// leftPowerTarget and rightPowerTarget are the intended velocities for the left and right
// wheels to achieve respectively. See setPowerTargets(...) for more information.
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

// Set new power targets for each motor for the given PID controller to achieve.
//
// Power targets should be between -1.0 and 1.0 (inclusive).
void setPowerTargets(struct MotorPID* pid, float leftPowerTarget, float rightPowerTarget) {
    pid->leftPowerTarget = leftPowerTarget; 
    pid->leftPower = leftPowerTarget;
    pid->rightPowerTarget = rightPowerTarget;
    pid->rightPower = rightPowerTarget;
}

// Resets distance counters of PID to 0. Recommended to be called in tandem with resetCounters(...)
void resetDistCounts(struct MotorPID* pid) {
    pid->leftDistCount = 0;
    pid->rightDistCount = 0;
}

// Performs the appropriate calculations and updates the pid given with new decoder information.
//
// newLDist and newRDist are expected to be int16 from the left decoder and right decoder respectively.
// Recommended for this information to be retrieved with getLeftCounter() and getRightCounter().
void updateMotorPID(struct MotorPID* pid, int16 newLDist, int16 newRDist) {
    // Update deltas
    pid->leftDistDelta = newLDist - pid->leftDistCount;
    pid->rightDistDelta = newRDist - pid->rightDistCount;
    
    // Update counts
    pid->leftDistCount = newLDist;
    pid->rightDistCount = newRDist;
    
    // Calculate new error
    //
    // Logic is as follows:
    // ---
    // In theory, the ratio of the speeds achieved by each wheel should be equal to the
    // ratio of the power targets for each wheel. 
    //
    // So:
    //    (leftDistDelta / rightDistDelta) = (leftPowerTarget / rightPowerTarget)
    //
    // Manipulating this we can see:
    //    leftDistDelta * rightPowerTarget = rightDistDelta * leftPowerTarget
    //
    // So we take the difference between the LHS and RHS to construct an error. In theory we want
    // this difference to be 0.
    //
    // Other ways explored of constructing an error function involved division. Division has the issue of things blowing up
    // when the denominator is 0 or close to it.
    // ---
    float newError = pid->leftDistDelta*pid->rightPowerTarget - pid->rightDistDelta*pid->leftPowerTarget;
    // Calculate error delta
    pid->sideErrorDelta = newError - pid->sideError;
    // Store new error
    pid->sideError = newError;
    // Calculate error integral. Luckilly this can be achieved using the decoder distance information.
    pid->sideErrorInt = pid->leftDistCount*pid->rightPowerTarget - pid->rightDistCount*pid->leftPowerTarget;

    // Apply coefficients to calculate power corrections
    float correction = pid->sideError*PROP_COEFF + pid->sideErrorDelta*DELTA_COEFF + pid->sideErrorInt*INT_COEFF;
    pid->leftPower -= correction;
    pid->rightPower += correction;
    
    // Ensure power values are never greater in magnitude than specified power targets
    pid->leftPower = clamp(pid->leftPower, -fabs(pid->leftPowerTarget), fabs(pid->leftPowerTarget));
    pid->rightPower = clamp(pid->rightPower, -fabs(pid->rightPowerTarget), fabs(pid->rightPowerTarget));
}

#endif



