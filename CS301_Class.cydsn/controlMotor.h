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
#define N 0
#define E 1
#define S 2
#define W 3

#include <project.h>
#include <distance.h>
#include "actions.h"
    

extern uint16_t Q5, Q6, Q4, Q2, Q1, Q3;
uint8_t Q3_Flag, Q4_Flag = 0;
uint8_t PWMLeftMotor, PWMRightMotor = 127;
const uint8_t percentageOfSpeed = 100;
float percentage1, percentage2 = 0.00;

uint8_t leftTurnCount = 0;

// to be passed in from the pathfinder algorithm
//-------------------------------------------- 
// 0 = North, 1 = East, 2 = South, 3 = West
uint8_t pathInstructions[] = {S, E, S, W, S, E, S, E, N, S, S, W, N, W, N, E, N, E, E, S, S, E, S, W, S, E, S, W, W, N, E, N, E, S};
// uint8_t pathInstructions[] = {S, E, S, W, S,| E, S, E, N ,| S, S, W, N, W, N, E, N, E,| E, S, S,| E, S, W, S, E, S, W, W, N, E, N, E, S};
uint8_t turnsUntilNextNode[] = {4, 3, 8, 2, 12};
uint8_t remainingDistance[] = {36, 54, 52, 18, 36};
uint8_t i, j = 0;
uint8_t location = 1;
uint8_t direction = 0;
//--------------------------------------------

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

        // possible junction occurs
        if (Q3 == 0 || Q4 == 0)
        {

            // // we have reached the end of the node (think more about)
            // ------------------------------------------------
            // if (i == turnsUntilNextNode[j])
            // {
            //  
            //     distance = remainingDistance[j];
            //     endNode = True;
            //     godistiance(distance);
            //     j++;
            //    i++;
            //     
            // ------------------------------------------------

            // if (i == turnsUntilNextNode[j] + 1)
            // //     // want to then determine the nature of the turn
            // // go straight
            // if ((pathInstructions[location + 1] == N && pathInstructions[location] == N) || (pathInstructions[location + 1] == E && pathInstructions[location] == E) || (pathInstructions[location + 1] == S && pathInstructions[location] == S) || (pathInstructions[location + 1] == W && pathInstructions[location] == W))
            // {
            //     break;
            // }
            // // go left
            // else if ((pathInstructions[location + 1] == N && pathInstructions[location] == E) || (pathInstructions[location + 1] == W && pathInstructions[location] == N) || (pathInstructions[location + 1] == E && pathInstructions[location] == S) || (pathInstructions[location + 1] == S && pathInstructions[location] == W))
            // {
            //     direction = 1;
            // }

            // // go right
            // else if ((pathInstructions[location + 1] == S && pathInstructions[location] == E) || (pathInstructions[location + 1] == E && pathInstructions[location] == N) || (pathInstructions[location + 1] == W && pathInstructions[location] == S) || (pathInstructions[location + 1] == N && pathInstructions[location] == W))
            // {
            //     direction = 2;
            // }

            // // go back
            // else if ((pathInstructions[location + 1] == N && pathInstructions[location] == S) || (pathInstructions[location + 1] == S && pathInstructions[location] == N) || (pathInstructions[location + 1] == E && pathInstructions[location] == W) || (pathInstructions[location + 1] == W && pathInstructions[location] == E))
            // {
            //     direction = 3;
            // }

            //     // if all of the nodes have been visited
            //     if (j == 5) {
            //         direction = 4;
            //     }
            //     location += 2;
            // }

            // i = 0;

            //if ( i == turnsUntilNextNode[j] - 1)
            // {
            //  
            //     distance = remainingDistance[j];
            //     endNode = True;
            //     
            //     }

            // want to then determine the nature of the turn
            // go straight
            if ((pathInstructions[location] == N && pathInstructions[location - 1] == N) || (pathInstructions[location] == E && pathInstructions[location - 1] == E) || (pathInstructions[location] == S && pathInstructions[location - 1] == S) || (pathInstructions[location] == W && pathInstructions[location - 1] == W))
            {
                break;
            }
            // go left
            else if ((pathInstructions[location] == N && pathInstructions[location - 1] == E) || (pathInstructions[location] == W && pathInstructions[location - 1] == N) || (pathInstructions[location] == E && pathInstructions[location - 1] == S) || (pathInstructions[location] == S && pathInstructions[location - 1] == W))
            {
                direction = 1;
            }

            // go right
            else if ((pathInstructions[location] == S && pathInstructions[location - 1] == E) || (pathInstructions[location] == E && pathInstructions[location - 1] == N) || (pathInstructions[location] == W && pathInstructions[location - 1] == S) || (pathInstructions[location] == N && pathInstructions[location - 1] == W))
            {
                direction = 2;
            }

            // go back
            else if ((pathInstructions[location] == N && pathInstructions[location - 1] == S) || (pathInstructions[location] == S && pathInstructions[location - 1] == N) || (pathInstructions[location] == E && pathInstructions[location - 1] == W) || (pathInstructions[location] == W && pathInstructions[location - 1] == E))
            {
                direction = 3;
            }

            // increment for the next turn
            location++;
            i++;
        }

        break;

    case 1:
        // turn left
        *act = newAction(TURNING_LEFT);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        // if (endNode == True) {
        //     godistiance(distance);
        //     endNode = False;
        // }
        direction = 0;
        break;

    case 2:
        // turn right
        *act = newAction(TURNING_RIGHT);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        // if (endNode == True) {
        //     godistiance(distance);
        //     endNode = False;
        // }
        direction = 0;
        break;

    case 3:
        // turn around
        *act = newAction(TURNING_AROUND);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        // if (endNode == True) {
        //     godistiance(distance);
        //     endNode = False;
        // }
        direction = 0;
       
        break;

    case 4:
        // stop
        *act = newAction(DO_NOTHING);
        i = 0;
        location = 1;
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
