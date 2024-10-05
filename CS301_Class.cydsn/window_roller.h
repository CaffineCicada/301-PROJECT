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

#include <stdint.h>

#define WINDOW_SIZE 10

struct RollingWindow {
    int16_t data[WINDOW_SIZE];
    uint16_t i;
};

// Function to initialize a rollingWindow
struct RollingWindow newWindow() {
    struct RollingWindow output;
    // Initialize all data elements to 0 and set index i to 0
    for(int i=0; i < WINDOW_SIZE; i++) {
        output.data[i]=0;
    }
    output.i = 0;
    return output;
}

// Function to add an element to the buffer
void addElement(struct RollingWindow* buffer, int16_t num) { 
    buffer->data[buffer->i] = num;
    // Update position i, wrap around if i reaches WINDOW_SIZE
    buffer->i = (buffer->i + 1) % WINDOW_SIZE;
}

/* [] END OF FILE */
