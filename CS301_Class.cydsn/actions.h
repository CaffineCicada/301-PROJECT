#ifndef ACTIONS_H
#define ACTIONS_H

#include <project.h>
#include "motor_power_control.h"
#include "light_sensor.h"

#define BASE_SPEED 0.3
#define CORRECTING_SPEED 0.05

enum ActionType {
    GOING_STRAIGHT,
    TURNING_RIGHT,
    TURNING_LEFT,
    DO_NOTHING,
};

// Struct to hold information for an action/command for the robot.
//
// stage variable will be 0 at the starts of an action and -1 when action is complete
struct Action {
    enum ActionType type;
    int stage;
};

struct Action newAction(enum ActionType type) {
    struct Action act;
    act.type = type;
    act.stage = 0;
    return act;
}

void processAction(struct Action* act, struct MotorPID* pid, struct LightSensor* sensors) {
    switch (act->type) {
        case GOING_STRAIGHT:
            if(sensors[4].underBlack) {
                setPowerTargets(pid, BASE_SPEED, BASE_SPEED+CORRECTING_SPEED);
            }
            else if(sensors[5].underBlack) {
                setPowerTargets(pid, BASE_SPEED+CORRECTING_SPEED, BASE_SPEED);
            } else {
                setPowerTargets(pid, BASE_SPEED, BASE_SPEED);
            }
            break;
        case TURNING_LEFT:
            switch (act->stage) {
                case 0:
                    setPowerTargets(pid, -BASE_SPEED, -BASE_SPEED);
                    if(getLeftCounter() < -10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        resetCounters();
                        resetDistCounts(pid);
                        act->stage++;
                    }
                    break;
                case 1:
                    setPowerTargets(pid, 0.0, BASE_SPEED);
                    if(getRightCounter() > 210) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 2:
                    setPowerTargets(pid, BASE_SPEED, 0.0);
                    if(getLeftCounter() > 10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 3:
                    setPowerTargets(pid, 0.0, 0.0);
                    if(sensors[4].underBlack) {
                        setPowerTargets(pid, 0.0, BASE_SPEED);
                    } else if(sensors[5].underBlack) {
                        setPowerTargets(pid, BASE_SPEED, 0.0);
                    } else {
                        act->stage = -1;
                    }
                    break;
                default:
                    break;
            }
            break;
        case TURNING_RIGHT:
            switch (act->stage) {
                case 0:
                    setPowerTargets(pid, -BASE_SPEED, -BASE_SPEED);
                    if(getRightCounter() < -10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        resetCounters();
                        resetDistCounts(pid);
                        act->stage++;
                    }
                    break;
                case 1:
                    setPowerTargets(pid, BASE_SPEED, 0.0);
                    if(getLeftCounter() > 210) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 2:
                    setPowerTargets(pid, 0.0, BASE_SPEED);
                    if(getRightCounter() > 10) {
                        setPowerTargets(pid, 0.0, 0.0);
                        act->stage++;
                    }
                    break;
                case 3:
                    setPowerTargets(pid, 0.0, 0.0);
                    if(sensors[4].underBlack) {
                        setPowerTargets(pid, 0.0, BASE_SPEED);
                    } else if(sensors[5].underBlack) {
                        setPowerTargets(pid, BASE_SPEED, 0.0);
                    } else {
                        act->stage = -1;
                    }
                    break;
                default:
                    break;
            }
            break;
        case DO_NOTHING:
            setPowerTargets(pid, 0.0, 0.0);
            break;
        default:
            setPowerTargets(pid, 0.0, 0.0);
    }
}


#endif