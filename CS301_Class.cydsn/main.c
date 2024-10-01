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
//* ========================================
#include "defines.h"
#include "vars.h"
#include "tacho.h"
#include "windowRoller.h"
#include "controlMotor.h"
#include "distance.h"
//* ========================================
void usbPutString(char *s);
void usbPutChar(char c);
void handle_usb();
void whiteOrBlack();
void motorControl();
//* ========================================

volatile int16 resultQ5, resultQ6, resultQ2, resultQ4, resultQ1, resultQ3 = 0;
volatile int8 new_result = 0;
uint16_t maxValQ5, maxValQ6, maxValQ4, maxValQ2, maxValQ1, maxValQ3 = 0;
uint16_t minValQ5, minValQ6, minValQ4, minValQ2, minValQ1, minValQ3 = 5000;
struct rollingWindow sampleBufferQ5;
struct rollingWindow sampleBufferQ6;
struct rollingWindow sampleBufferQ4;
struct rollingWindow sampleBufferQ1;
struct rollingWindow sampleBufferQ3;
struct rollingWindow sampleBufferQ2;
uint16_t counter = 0;
uint16_t Q5 = 0;
uint16_t Q6 = 0;
uint16_t Q4 = 0;
uint16_t Q3 = 0;
uint16_t Q2 = 0;
uint16_t Q1 = 0;
uint16_t motor_counter = 0;

uint32_t quad1 = 0;
uint32_t quad2 = 0;


CY_ISR(isr_eoc){
    resultQ5 = ADC_CountsTo_mVolts(ADC_GetResult16(4)); // sensor Q5
    resultQ6 = ADC_CountsTo_mVolts(ADC_GetResult16(5)); // sensor Q6
    resultQ4 = ADC_CountsTo_mVolts(ADC_GetResult16(3)); // sensor Q4
    resultQ3 = ADC_CountsTo_mVolts(ADC_GetResult16(2)); // sensor Q3
    resultQ2 = ADC_CountsTo_mVolts(ADC_GetResult16(1)); // sensor Q2
    resultQ1 = ADC_CountsTo_mVolts(ADC_GetResult16(0)); // sensor Q1
   
    new_result = 1;
}

CY_ISR(isr_motor){
    motor_counter++;
    
    if(motor_counter == 1){
        motorControl();
        motor_counter = 0;
    }
    
    
    Timer_TS_ReadStatusRegister();
}

CY_ISR(isr_quad){
    distance();
    Timer_1_ReadStatusRegister();
}

CY_ISR_PROTO(isr_eoc);

int main()
{
    
    uint16_t peakQ5, peakQ6, peakQ4, peakQ2, peakQ1, peakQ3 = 0;
    
    
    ADC_Start();
    Timer_TS_Start();
    Timer_1_Start();
    PWM_1_Start();
    PWM_2_Start();
    QuadDec_M1_Start();
    QuadDec_M2_Start();
    isr_eoc_StartEx(isr_eoc);
    isr_motor_StartEx(isr_motor);
    isr_quad_StartEx(isr_quad);
    
    
    PWM_1_WriteCompare(127);
    PWM_2_WriteCompare(127);
    
    sampleBufferQ5 = window_new();
    sampleBufferQ6 = window_new();
    sampleBufferQ4 = window_new();
    sampleBufferQ1 = window_new();
    sampleBufferQ3 = window_new();
    sampleBufferQ2 = window_new();

// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;

// ------USB SETUP ----------------    
#ifdef USE_USB    
    USBUART_Start(0,USBUART_5V_OPERATION);
#endif        
        
//    RF_BT_SELECT_Write(0);

    usbPutString(displaystring);
    for(;;)
    {
        /* Place your application code here. */
        handle_usb();
        
        if(new_result == 1) {
            whiteOrBlack(); // call function to update the min and max values of sensor waveform during ADC sampling
            

                
                // calculate the peak to peak of the sensor waveform
                peakQ5 = maxValQ5 - minValQ5;
                peakQ6 = maxValQ6 - minValQ6;
                peakQ4 = maxValQ4 - minValQ4;
                peakQ2 = maxValQ2 - minValQ2;
                peakQ3 = maxValQ3 - minValQ3;
                peakQ1 = maxValQ1 - minValQ1;
                
                // reset the max and min values for the next time sampling 
                maxValQ5 = 0;
                minValQ5 = 5000;

                maxValQ6 = 0;
                minValQ6 = 5000;

                maxValQ2 = 0;
                minValQ2 = 5000;

                maxValQ4 = 0;
                minValQ4 = 5000;

                maxValQ1 = 0;
                minValQ1 = 5000;

                maxValQ3 = 0;
                minValQ3 = 5000;
                
                
                // if the sensor peak to peak is less than 70mV, the sensor is under black
                if(peakQ5 < 300)
                {
                    // Turn off the LED
                    Q5_LED_Write(0);
                    Q5 = 0;
                }
                else
                {
                    // Turn on the LED
                    Q5_LED_Write(1);
                    Q5 = 1;
                    
                }
                
                if(peakQ2 < 500)
                {
                    // Turn off the LED
                    Q2_LED_Write(0);
                    Q2 = 0;
                }
                else
                {
                    // Turn on the LED
                    Q2_LED_Write(1);
                    Q2 = 1;
                    
                }
                
                if(peakQ4 < 300)
                {
                    // Turn off the LED
                    Q4_LED_Write(0);
                    Q4 = 0;
                  
                }
                else
                {
                    // Turn on the LED
                    Q4_LED_Write(1);
                    Q4 = 1;
                }  
                
                
                if(peakQ6 < 300)
                {
                    Q6_LED_Write(0);
                    Q6 =0;
                }
                else
                {
                    Q6_LED_Write(1);
                    Q6 = 1;
                }


                if(peakQ1 < 300)
                {
                    Q1_LED_Write(0);
                    Q1 =0;
                }
                else
                {
                    Q1_LED_Write(1);
                    Q1 = 1;
                }


                if(peakQ3 < 300)
                {
                    //Q3_LED_Write(0);
                    Q3 =0;
                  
                }
                else
                {
                    //Q3_LED_Write(1);
                    Q3 = 1;
                }
               
            
               
                new_result = 0;
                char buffer[64];
                sprintf(buffer, "Value1: %d , Value2: %d \r\n" , quad1, quad2);

                usbPutString(buffer);
            }
        
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

void whiteOrBlack(){
          
    // Read ADC result
    addElement(&sampleBufferQ5, resultQ5);
    addElement(&sampleBufferQ6, resultQ6);
    addElement(&sampleBufferQ4, resultQ4);
    addElement(&sampleBufferQ3, resultQ3);
    addElement(&sampleBufferQ2, resultQ2);
    addElement(&sampleBufferQ1, resultQ1);

   
    // Update max and min values
    for(int i=0; i < WINDOW_SIZE; i++) {
        uint16_t numQ5 = sampleBufferQ5.data[i];
        uint16_t numQ6 = sampleBufferQ6.data[i];
        uint16_t numQ4 = sampleBufferQ4.data[i];
        uint16_t numQ3 = sampleBufferQ3.data[i];
        uint16_t numQ2 = sampleBufferQ2.data[i];
        uint16_t numQ1 = sampleBufferQ1.data[i];
        
        // Update max and min values
        if(numQ5 > maxValQ5) maxValQ5 = numQ5;
        if(numQ5 < minValQ5) minValQ5 = numQ5;

        if(numQ6 > maxValQ6) maxValQ6 = numQ6;
        if(numQ6 < minValQ6) minValQ6 = numQ6;
        
        if(numQ4 > maxValQ4) maxValQ4 = numQ4;
        if(numQ4 < minValQ4) minValQ4 = numQ4;
        
        if(numQ4 > maxValQ3) maxValQ3 = numQ3;
        if(numQ4 < minValQ3) minValQ3 = numQ3;
        
        if(numQ4 > maxValQ2) maxValQ2 = numQ2;
        if(numQ4 < minValQ2) minValQ2 = numQ2;
        
        if(numQ4 > maxValQ1) maxValQ1 = numQ1;
        if(numQ4 < minValQ1) minValQ1 = numQ1;
    }

    //counter++;   // counter to count how many samples have been taken
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
