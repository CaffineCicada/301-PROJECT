#ifndef ACTIONS_H
#define ACTIONS_H

#include "motor_power_control.h"
#include "light_sensor.h"

#define BASE_SPEED 0.3
#define CORRECTING_SPEED 0.05

enum ActionType {
    GOING_STRAIGHT, // Robot is travelling straight
    TURNING_RIGHT, // Robot is trying to turn 90deg right
    TURNING_LEFT, // Robot is trying to turn 90deg left
    TURNING_AROUND, // Robot is trying to turn 180deg left
    DO_NOTHING, // Robot sits completely still
};

// Struct to hold information for an action/command for the robot.
//
// stage variable will be 0 at the starts of an action and -1 when action is complete. It is recommended
// that this value is only read and never directly manipulated externally.
//
// It is advised that new actions are only created with newAction(...) and that action structs
// are manipulated from processAction(...).
struct Action {
    enum ActionType type;
    int stage;
};

// Create a new action with proper/beginning staging information.
struct Action newAction(enum ActionType type) {
    struct Action act;
    act.type = type;
    act.stage = 0;
    return act;
}

// Sets the proper motor power targets for the pid to achieve given an action and sensor information.
//
// sensors input is assumed to be an array of LightSensor structs. Index 0 linking to Q1, ..., and index 5 linking to Q6.
//
// The stage member variable of the given action is manipulated when needed.
void processAction(struct Action* act, struct MotorPID* pid, struct LightSensor* sensors) {
    switch (act->type) {
        case GOING_STRAIGHT:
            // Robot is too far right
            if(sensors[4].underBlack) {
                setPowerTargets(pid, BASE_SPEED, BASE_SPEED+CORRECTING_SPEED);
            }
            // Robot is too far left
            else if(sensors[5].underBlack) {
                setPowerTargets(pid, BASE_SPEED+CORRECTING_SPEED, BASE_SPEED);
            } else {
                setPowerTargets(pid, BASE_SPEED, BASE_SPEED);
            }
            break;
        case TURNING_AROUND:
            switch (act->stage) {
                case 0:
                    // Reset pid and decoder information
                    setPowerTargets(pid, 0.0, 0.0);
                    resetDistCounts(pid);
                    resetCounters();
                    act->stage++;
                    break;
                case 1:
                    // Perform the turn
                    setPowerTargets(pid, -BASE_SPEED, BASE_SPEED);
                    if(getRightCounter() > 190) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage = -1;
                    }
                    break;
                default:
                    // Sit still when complete
                    setPowerTargets(pid, 0.0, 0.0);
                    break;
            }
            break;
        case TURNING_LEFT:
            switch (act->stage) {
                case 0:
                    // Reset pid and decoder information
                    setPowerTargets(pid, 0.0, 0.0);
                    resetDistCounts(pid);
                    resetCounters();
                    act->stage++;
                    break;
                case 1:
                    // Reverse slightly
                    setPowerTargets(pid, -BASE_SPEED, -BASE_SPEED);
                    if(getLeftCounter() < -10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        resetCounters();
                        resetDistCounts(pid);
                        act->stage++;
                    }
                    break;
                case 2:
                    // Perform bulk of left turn
                    setPowerTargets(pid, 0.0, BASE_SPEED);
                    if(getRightCounter() > 210) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 3:
                    // Straighten up by turning slightly right
                    setPowerTargets(pid, BASE_SPEED, 0.0);
                    if(getLeftCounter() > 10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 4:
                    // Try straighten up more then mark action as complete
                    if(sensors[4].underBlack) {
                        setPowerTargets(pid, 0.0, BASE_SPEED);
                    } else if(sensors[5].underBlack) {
                        setPowerTargets(pid, BASE_SPEED, 0.0);
                    } else {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage = -1;
                    }
                    break;
                default:
                    // Sit still when complete
                    setPowerTargets(pid, 0.0, 0.0);
                    break;
            }
            break;
        case TURNING_RIGHT:
            switch (act->stage) {
                case 0:
                    // Reset pid and decoder information
                    setPowerTargets(pid, 0.0, 0.0);
                    resetDistCounts(pid);
                    resetCounters();
                    act->stage++;
                    break;
                case 1:
                    // Reverse slightly
                    setPowerTargets(pid, -BASE_SPEED, -BASE_SPEED);
                    if(getRightCounter() < -10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        resetCounters();
                        resetDistCounts(pid);
                        act->stage++;
                    }
                    break;
                case 2:
                    // Perform bulk of right turn
                    setPowerTargets(pid, BASE_SPEED, 0.0);
                    if(getLeftCounter() > 210) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 3:
                    // Straighten up by turning slightly left
                    setPowerTargets(pid, 0.0, BASE_SPEED);
                    if(getRightCounter() > 10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 4:
                    // Try straighten up more then mark action as complete
                    if(sensors[4].underBlack) {
                        setPowerTargets(pid, 0.0, BASE_SPEED);
                    } else if(sensors[5].underBlack) {
                        setPowerTargets(pid, BASE_SPEED, 0.0);
                    } else {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage = -1;
                    }
                    break;
                default:
                    // Sit still when complete
                    setPowerTargets(pid, 0.0, 0.0);
                    break;
            }
            break;
        case DO_NOTHING:
            // Sit still
            setPowerTargets(pid, 0.0, 0.0);
            break;
        default:
            // Sit still
            setPowerTargets(pid, 0.0, 0.0);
    }
}


#endif