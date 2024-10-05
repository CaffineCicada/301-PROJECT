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
uint8_t pathInstructions[] = {0, 0, 1, 2, 3, 3, 0, 3, 2, 3, 0, 3, 2};
uint8_t turnsUntilNextNode[] = {13};
uint8_t remainingDistance[] = {0};
uint8_t j = 0;
uint8_t i = 1;
uint8_t direction;
uint8_t lengthOfArray = sizeof(pathInstructions) / sizeof(pathInstructions[0]);
//--------------------------------------------

void motorControl() {
    
    
    
    if(stop == 1){
        PWMLeftMotor = 127;
        PWMRightMotor = 127;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
    }
    /*
    if (j == 5){
        // we have reached the end of the array and will stop (completed all nodes)
        PWM_1_WriteCompare(127);
        PWM_2_WriteCompare(127);
    }
    */
    else if (Q3 == 0 || Q4 == 0) {
    
        
        // look into this
        if (i == turnsUntilNextNode[j]){
            // we have reached the end of the node
            // implement travelling the remaining distance here
            // j++;
        }
        
        //else {
        
            // apologies for this nasty code
            // this is to determine the nature of the turn
            
            // go straight
            if ((pathInstructions[i] == 0 && pathInstructions[i-1] == 0) || (pathInstructions[i] == 1 && pathInstructions[i-1] == 1) || (pathInstructions[i] == 2 && pathInstructions[i-1] == 2) || (pathInstructions[i] == 3 && pathInstructions[i-1] == 3)){
            
                direction = 0;
            
            }
            // go left
            else if((pathInstructions[i] == 0 && pathInstructions[i-1] == 1) || (pathInstructions[i] == 1 && pathInstructions[i-1] == 3) || (pathInstructions[i] == 2 && pathInstructions[i-1] == 3) || (pathInstructions[i] == 3 && pathInstructions[i-1] == 0)){
            
                direction = 1;
            
            }

            // go right
            else if((pathInstructions[i] == 0 && pathInstructions[i-1] == 3) || (pathInstructions[i] == 1 && pathInstructions[i-1] == 0) || (pathInstructions[i] == 2 && pathInstructions[i-1] == 1) || (pathInstructions[i] == 3 && pathInstructions[i-1] == 2)){
            
                direction = 2;
            
            }

            // go back
            else if((pathInstructions[i] == 0 && pathInstructions[i-1] == 2) || (pathInstructions[i] == 1 && pathInstructions[i-1] == 2) || (pathInstructions[i] == 2 && pathInstructions[i-1] == 0) || (pathInstructions[i] == 3 && pathInstructions[i-1] == 1)){
            
                direction = 3;
            
            }

            // physical turning
            switch (direction)
            {
            case 0:
                // go straight
                PWM_1_WriteCompare(157);
                PWM_2_WriteCompare(97);
                break;
            
            /*case 1:
                // turn left
                PWMLeftMotor = 90;
                PWMRightMotor = 90;
                PWM_1_WriteCompare(PWMLeftMotor);
                PWM_2_WriteCompare(PWMRightMotor);
                break;

            case 2:
                
                // turn right
                PWMLeftMotor = 157;
                PWMRightMotor = 103;
                PWM_1_WriteCompare(PWMLeftMotor);
                PWM_2_WriteCompare(PWMRightMotor); 
                break;

            case 3:
                // turn around
                PWMLeftMotor = 90;
                PWMRightMotor = 90;
                PWM_1_WriteCompare(PWMLeftMotor);
                PWM_2_WriteCompare(PWMRightMotor);
                break;
            */
            default:
                // stop
                PWM_1_WriteCompare(127);
                PWM_2_WriteCompare(127);
                break;
            }
        
        i++;

        
        
    }
    
    
    /*
    //for later use probably      
    // light turn
    else if(Q4 == 0) {
        PWMLeftMotor = 160;
        PWMRightMotor = 160;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
    }
    
    // left turn
    else if(Q3 == 0) {
        PWMLeftMotor = 90;
        PWMRightMotor = 90;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
       
    }
    */
    
    // save
    //----------------------------------------------
    // adjust to the right
    else if(Q5 == 1 && Q6 == 0) {
        PWMLeftMotor = 157;
        PWMRightMotor = 103;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
    }
    
    // adjust to the left
    else if(Q5 == 0 && Q6 == 1) {
        PWMLeftMotor = 152;
        PWMRightMotor = 90;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
    }
    //----------------------------------------------
    
    // ignore (this works)
    //---------------------------------------------------
    // Straight line
    else if(Q5 == 1 && Q6 == 1) {
        /*
        //calculates the percentage the robot travels at and feeds to the PWM 
        
        percentage1 = (((percentageOfSpeed - 40)*32)/60); //calculates percentage1
        percentage2 = (((percentageOfSpeed - 40)*34)/60); //calculates percentage2
        
        //feeds the PWM with the updated values
        //minimum speed value = 155, 100
        //maximum speed value = 187, 66
        PWMLeftMotor = 155 + (uint8_t)percentage1;
        PWMRightMotor = 100 - (uint8_t)percentage2;
        */
        //write to the motor
        PWM_1_WriteCompare(157);
        PWM_2_WriteCompare(97);
    }
    //--------------------------------------------------
    
    
}

/* [] END OF FILE */
