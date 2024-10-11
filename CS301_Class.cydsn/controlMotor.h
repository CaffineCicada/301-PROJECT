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

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#define STRAIGHT 0
#define RIGHT 1
#define LEFT 2
#define TURN_AROUND 3
#define TERMINATE 4

#include <project.h>
#include <distance.h>
#include "actions.h"
    

extern uint16_t Q5, Q6, Q4, Q2, Q1, Q3;
uint16_t travelDistance = 0;

// to be passed in from the pathfinder algorithm
//-------------------------------------------- 
uint8_t pathInstructions[] = {};
//--------------------------------------------
uint8_t location = 0;
uint8_t direction = 0;

void motorControl(struct Action* act)
{

    switch (direction)
    {
    case 0:
        // go straight
        *act = newAction(GOING_STRAIGHT);

        // stop
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }

        // if we need to go a certain distance
        if (pathInstructions[location] != STRAIGHT && pathInstructions[location] != RIGHT && pathInstructions[location] != LEFT && pathInstructions[location] != TURN_AROUND && pathInstructions[location] != TERMINATE)
        {
            // convert distance cm to ticks
            travelDistance = (pathInstructions[location] * 228) / 20;
            *act = newGoingStraightForX(travelDistance);
            location ++;

        }

        // we have reached the end of the array
        if (pathInstructions[location] == TERMINATE)
        {
            direction = -1;
        }

        // possible junction occurs
        if (Q3 == 0 || Q4 == 0)
        {
            // want to then determine the nature of the turn
            // go straight
            if (pathInstructions[location] == STRAIGHT)
            {
                break;
            }
            // go left
            else if (pathInstructions[location] == LEFT)
            {
                direction = 1;
            }

            // go right
            else if (pathInstructions[location] == RIGHT)
            {
                direction = 2;
            }

            // go back
            else if (pathInstructions[location] == TURN_AROUND)
            {
                direction = 3;
            }

            // increment for the next turn
            location++;
        }

        break;

    case 1:
        // turn left
        *act = newAction(TURNING_LEFT);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        direction = 0;
        break;

    case 2:
        // turn right
        *act = newAction(TURNING_RIGHT);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        direction = 0;
        break;

    case 3:
        // turn around
        *act = newAction(TURNING_AROUND);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        direction = 0;
       
        break;

    case 4:
        // stop
        *act = newAction(DO_NOTHING);
        location = 0;
        if (Q1 == 1 && Q2 == 1 && Q3 == 1 && Q4 == 1 && Q5 == 1 && Q6 == 1)
        {
            direction = 0;
        }

        break;

    default:
        // stop
        *act = newAction(DO_NOTHING);
        break;
    }

}

#endif /* MOTOR_CONTROL_H */


/* [] END OF FILE */
