
#include <stdint.h>
#include <stdbool.h>
#include "window_roller.h"

#define BLACK_THRESHOLD 300

struct LightSensor {
    struct RollingWindow data;
    uint16_t max;
    uint16_t min;
    bool underBlack;
};

// Initialise a new LightSensor with no data
struct LightSensor newLightSensor() {
    struct LightSensor output;
    output.data = newWindow();
    output.max = 0;
    output.min = 0;
    bool underBlack = 0;
    return output;
}

// Add a new sensor reading and update struct accordingly
void updateLightSensor(struct LightSensor *sensor, uint16_t newReading) {
    addElement(&(sensor->data), newReading);
    // Update max and min
    for(int i=0; i < WINDOW_SIZE; i++) {
        uint16_t num = sensor->data.data[i];
        sensor->max = 0;
        sensor->min = UINT16_MAX;

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
