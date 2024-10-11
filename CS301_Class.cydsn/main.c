/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>
#include <stdbool.h>
//* ========================================
#include "defines.h"
#include "vars.h"
#include "tacho.h"
//#include "light_sensor.h"
//#include "distance.h"
//#include "motor_power_control.h"
#include "actions.h"
#include "controlMotor.h"
//* ========================================
void usbPutString(char *s);
void usbPutChar(char c);
void handle_usb();
void whiteOrBlack();
void motorControl();
//* ========================================

volatile int16 newLightResults[6] = {0};
volatile int8 newLightResultsFlag = false;
uint16_t motor_counter = 0;
uint32_t quad1 = 0;
uint32_t quad2 = 0;
uint16_t Q5, Q6, Q4, Q2, Q1, Q3;

CY_ISR(isr_eoc){
    newLightResults[0] = ADC_CountsTo_mVolts(ADC_GetResult16(0)); // sensor Q1
    newLightResults[1] = ADC_CountsTo_mVolts(ADC_GetResult16(1)); // sensor Q2
    newLightResults[2] = ADC_CountsTo_mVolts(ADC_GetResult16(2)); // sensor Q3
    newLightResults[3] = ADC_CountsTo_mVolts(ADC_GetResult16(3)); // sensor Q4
    newLightResults[4] = ADC_CountsTo_mVolts(ADC_GetResult16(4)); // sensor Q5
    newLightResults[5] = ADC_CountsTo_mVolts(ADC_GetResult16(5)); // sensor Q6
   
    newLightResultsFlag = 1;
}


CY_ISR_PROTO(isr_eoc);

int main()
{
    // Initialisation
    struct LightSensor lightSensors[6] = {
        newLightSensor(), newLightSensor(), newLightSensor(),
        newLightSensor(), newLightSensor(), newLightSensor(),
    };
    struct MotorPID pid = newMotorPID(0.0, 0.0);
    struct Action currentAction = newAction(DO_NOTHING);
    uint8 warmupCount = 0;
    
    ADC_Start();
    Timer_TS_Start();
    Timer_1_Start();
    PWM_1_Start();
    PWM_2_Start();
    QuadDec_M1_Start();
    QuadDec_M2_Start();
    isr_eoc_StartEx(isr_eoc);
    

// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;

// ------USB SETUP ----------------    
#ifdef USE_USB    
    USBUART_Start(0,USBUART_5V_OPERATION);
#endif        
        
//    RF_BT_SELECT_Write(0);

    // Collect light information before proceeding to main loop
    while(warmupCount < 20) {
        if(newLightResultsFlag == true) {
            for(int i = 0; i < 6; i++) {
                updateLightSensor(&lightSensors[i], newLightResults[i]);
            }
            newLightResultsFlag = false;
            warmupCount++;
        } 
    }
    currentAction = newAction(GOING_STRAIGHT);
    usbPutString(displaystring);
    
    // Main loop
    for(;;)
    {        
        /* Place your application code here. */
        handle_usb();
        // Update light sensor state with new readings
        if(newLightResultsFlag == true) {
            for(int i = 0; i < 6; i++) {
                updateLightSensor(&lightSensors[i], newLightResults[i]);
            }
            newLightResultsFlag = false;
        }
        
        // Update LEDs
        if(lightSensors[0].underBlack) {
            Q1 = false;
            Q1_LED_Write(0);
        } else {
            Q1 = true;
            Q1_LED_Write(1);
        }
        if(lightSensors[1].underBlack) {
            Q2 = false;
            Q2_LED_Write(0);
        } else {
            Q2 = true;
            Q2_LED_Write(1);
        }
        if(lightSensors[2].underBlack) {
            Q3 = false;
            Q3_LED_Write(0);
        } else {
            Q3 = true;
            Q3_LED_Write(1);
        }
        if(lightSensors[3].underBlack) {
            Q4 = false;
            Q4_LED_Write(0);
        } else {
            Q4 = true;
            Q4_LED_Write(1);
        }
        if(lightSensors[4].underBlack) {
            Q5 = false;
            Q5_LED_Write(0);
        } else {
            Q5 = true;
            Q5_LED_Write(1);
        }
        if(lightSensors[5].underBlack) {
            Q6 = false;
            Q6_LED_Write(0);
        } else {
            Q6 = true;
            Q6_LED_Write(1);
        }
        

        // Decide next action if need be
        if(currentAction.type == GOING_STRAIGHT || currentAction.stage == -1) {
            motorControl(&currentAction);   
        }
        
        // Process current action
        updateMotorPID(&pid, getLeftCounter(), getRightCounter());
        processAction(&currentAction, &pid, lightSensors);
        setLeftPower(pid.leftPower);
        setRightPower(pid.rightPower);

        
        char buffer[64];
        sprintf(buffer, "Value1: %d , Value2: %d \r\n" , quad1, quad2);

        usbPutString(buffer);
    
        
        //get_speed();
 
        
        if (flag_KB_string == 1)
        {
            usbPutString(line);
            flag_KB_string = 0;
        }
    }   
}
//* ========================================
void usbPutString(char *s)
{
// !! Assumes that *s is a string with allocated space >=64 chars     
//  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  length to 62 char (63rd char is a '!')

#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    s[63]='\0';
    s[62]='!';
    USBUART_PutData((uint8*)s,strlen(s));
#endif
}
//* ========================================
void usbPutChar(char c)
{
#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);
#endif    
}

//* ========================================
void handle_usb()
{
    // handles input at terminal, echos it back to the terminal
    // turn echo OFF, key emulation: only CR
    // entered string is made available in 'line' and 'flag_KB_string' is set
    
    static uint8 usbStarted = FALSE;
    static uint16 usbBufCount = 0;
    uint8 c; 
    

    if (!usbStarted)
    {
        if (USBUART_GetConfiguration())
        {
            USBUART_CDC_Init();
            usbStarted = TRUE;
        }
    }
    else
    {
        if (USBUART_DataIsReady() != 0)
        {  
            c = USBUART_GetChar();

            if ((c == 13) || (c == 10))
            {
//                if (usbBufCount > 0)
                {
                    entry[usbBufCount]= '\0';
                    strcpy(line,entry);
                    usbBufCount = 0;
                    flag_KB_string = 1;
                }
            }
            else 
            {
                if (((c == CHAR_BACKSP) || (c == CHAR_DEL) ) && (usbBufCount > 0) )
                    usbBufCount--;
                else
                {
                    if (usbBufCount > (BUF_SIZE-2) ) // one less else strtok triggers a crash
                    {
                       USBUART_PutChar('!');        
                    }
                    else
                        entry[usbBufCount++] = c;  
                }  
            }
        }
    }    
}


/* [] END OF FILE */
