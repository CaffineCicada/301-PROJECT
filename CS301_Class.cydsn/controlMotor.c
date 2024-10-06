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
#include <project.h>
#include <distance.h>
#include "controlMotor.h"

extern uint16_t Q5, Q6, Q4, Q2, Q1, Q3;
uint8_t Q3_Flag, Q4_Flag = 0;
uint8_t PWMLeftMotor, PWMRightMotor = 127;
const uint8_t percentageOfSpeed = 100;
float percentage1, percentage2 = 0.00;

uint8_t leftTurnCount = 0;

// to be passed in from the pathfinder algorithm
//-------------------------------------------- 
// 0 = North, 1 = East, 2 = South, 3 = West
uint8_t pathInstructions[] = {0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t turnsUntilNextNode[] = {13};
uint8_t remainingDistance[] = {0};
uint8_t i, j = 0;
uint8_t location = 1;
uint8_t direction = 0;
uint8_t lengthOfArray = sizeof(pathInstructions) / sizeof(pathInstructions[0]);
//--------------------------------------------

void motorControl()
{

    switch (direction)
    {
    case 0:
        // go straight
        PWM_1_WriteCompare(157);
        PWM_2_WriteCompare(97);

        // stop
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }

        // adjust to the right
        if (Q5 == 1 && Q6 == 0)
        {
            PWMLeftMotor = 157;
            PWMRightMotor = 103;
            PWM_1_WriteCompare(PWMLeftMotor);
            PWM_2_WriteCompare(PWMRightMotor);
        }

        // adjust to the left
        else if (Q5 == 0 && Q6 == 1)
        {
            PWMLeftMotor = 152;
            PWMRightMotor = 90;
            PWM_1_WriteCompare(PWMLeftMotor);
            PWM_2_WriteCompare(PWMRightMotor);
        }

        // possible junction occurs
        if (Q3 == 0 || Q4 == 0)
        {
            // if we have eaten all of the food nodes
            // if (j == 5)
            // {
            //     // we have reached the end of the array and will stop (completed all nodes)
            //     direction = 4;
            // }

            // // we have reached the end of the node
            // if (i == turnsUntilNextNode[j])
            // {
            //     // implement travelling the remaining distance here
            //     j++;
            //     i = 0;
            // }

            // want to then determine the nature of the turn
            // go straight
            if ((pathInstructions[location] == 0 && pathInstructions[location - 1] == 0) || (pathInstructions[location] == 1 && pathInstructions[location - 1] == 1) || (pathInstructions[location] == 2 && pathInstructions[location - 1] == 2) || (pathInstructions[location] == 3 && pathInstructions[location - 1] == 3))
            {
                break;
            }
            // go left
            else if ((pathInstructions[location] == 0 && pathInstructions[location - 1] == 1) || (pathInstructions[location] == 3 && pathInstructions[location - 1] == 0) || (pathInstructions[location] == 1 && pathInstructions[location - 1] == 2) || (pathInstructions[location] == 2 && pathInstructions[location - 1] == 3))
            {
                direction = 1;
            }

            // go right
            else if ((pathInstructions[location] == 2 && pathInstructions[location - 1] == 1) || (pathInstructions[location] == 1 && pathInstructions[location - 1] == 0) || (pathInstructions[location] == 3 && pathInstructions[location - 1] == 2) || (pathInstructions[location] == 0 && pathInstructions[location - 1] == 3))
            {
                direction = 2;
            }

            // go back
            else if ((pathInstructions[location] == 0 && pathInstructions[location - 1] == 2) || (pathInstructions[location] == 2 && pathInstructions[location - 1] == 0) || (pathInstructions[location] == 1 && pathInstructions[location - 1] == 3) || (pathInstructions[location] == 3 && pathInstructions[location - 1] == 1))
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
        PWMLeftMotor = 90;
        PWMRightMotor = 90;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        CyDelay(500);
        direction = 0;
        break;

    case 2:

        // turn right
        PWMLeftMotor = 157;
        PWMRightMotor = 157;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        CyDelay(500);
        direction = 0;
        break;

    case 3:
        // turn around
        PWMLeftMotor = 90;
        PWMRightMotor = 90;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
        if (Q1 == 0 && Q2 == 0 && Q3 == 0 && Q4 == 0 && Q5 == 0 && Q6 == 0)
        {
            direction = 4;
        }
        CyDelay(1500);
        direction = 0;
       
        break;

    case 4:
        // stop
        PWM_1_WriteCompare(127);
        PWM_2_WriteCompare(127);
        i = 0;
        location = 1;
        if (Q1 == 1 && Q2 == 1 && Q3 == 1 && Q4 == 1 && Q5 == 1 && Q6 == 1)
        {
            direction = 0;
        }

        break;

    default:
        // stop
        PWM_1_WriteCompare(127);
        PWM_2_WriteCompare(127);
        break;
    }

}

/* [] END OF FILE */
