#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "window_roller.h"

#define BLACK_THRESHOLD 300

// Stores all the information needed to process the result of a light sensor on the robot
struct LightSensor {
    struct RollingWindow data;
    uint16_t max;
    uint16_t min;

    // Describes the state/output of the light sensor. True means we are in the black.
    // False means we are in the white.
    //
    // This should only be read from externally. It should never be directly manipulated 
    // and should only be manipulated by updateLightSensor(...)
    bool underBlack;
};

// Initialise a new LightSensor with no data
struct LightSensor newLightSensor() {
    struct LightSensor output;
    output.data = newWindow();
    output.max = 0;
    output.min = 0;
    output.underBlack = 0;
    return output;
}

// Processes a new sensor reading into the LightSensor struct given
void updateLightSensor(struct LightSensor *sensor, uint16_t newReading) {
    addElement(&(sensor->data), newReading);
    sensor->max = 0;
    sensor->min = UINT16_MAX;
    // Update max and min
    for(int i=0; i < WINDOW_SIZE; i++) {
        uint16_t num = sensor->data.data[i];

        if(sensor->max < num) {
            sensor->max = num;
        }
        if(sensor->min > num) {
            sensor->min = num;
        }
    }
    // Update state
    uint16_t diff = sensor->max - sensor->min;
    if(diff < BLACK_THRESHOLD) {
        sensor->underBlack = true;
    } else {
        sensor->underBlack = false;
    }
}

#endif