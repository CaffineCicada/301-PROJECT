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
#include "distance.h"

uint32 newL, newR, addDistanceL, addDistanceR, oldR, oldL, sumL, sumR ,totalTickDistanceRight,totalTickDistanceLeft, totalDistance, currentSpeed = 0;
uint8 stop = 0;
uint8 test = 1;

// everything is in metres
void distance (void){
    newL = QuadDec_M1_GetCounter();
    //newR = QuadDec_M2_GetCounter();
    
    newL = newL * -1;
    totalTickDistanceLeft = (newL * 20)/228;
    
    //if(totalTickDistanceLeft == 200){
    //    stop = 1;
    //    QuadDec_M1_Stop();
    //    Timer_1_Stop();
    //}

}

/* [] END OF FILE */
