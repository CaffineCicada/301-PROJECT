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
uint8_t pathInstructions[] = {0, 0, 1};
uint8_t turnsUntilNextNode[] = {};
uint8_t remainingDistance[] = {};
//--------------------------------------------

void motorControl() {
    
    if(stop == 1){
        PWMLeftMotor = 127;
        PWMRightMotor = 127;
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
    }
            
    // light turn
//    else if(Q4 == 0) {
//        PWMLeftMotor = 160;
//        PWMRightMotor = 160;
//       PWM_1_WriteCompare(PWMLeftMotor);
//        PWM_2_WriteCompare(PWMRightMotor);
//   }
    
    // left turn
//    else if(Q3 == 0) {
//        PWMLeftMotor = 90;
//        PWMRightMotor = 90;
//        PWM_1_WriteCompare(PWMLeftMotor);
//        PWM_2_WriteCompare(PWMRightMotor);
       
//    }
    
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
    
    
    // ignore (this works)
    //---------------------------------------------------
    // Straight line
    else if(Q5 == 1 && Q6 == 1) {
        
        //calculates the percentage the robot travels at and feeds to the PWM 
        
        percentage1 = (((percentageOfSpeed - 40)*32)/60); //calculates percentage1
        percentage2 = (((percentageOfSpeed - 40)*34)/60); //calculates percentage2
        
        //feeds the PWM with the updated values
        //minimum speed value = 155, 100
        //maximum speed value = 187, 66
        PWMLeftMotor = 155 + (uint8_t)percentage1;
        PWMRightMotor = 100 - (uint8_t)percentage2;
        
        //write to the motor
        PWM_1_WriteCompare(PWMLeftMotor);
        PWM_2_WriteCompare(PWMRightMotor);
    }
    //--------------------------------------------------
    
    
}

/* [] END OF FILE */
