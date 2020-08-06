#ifndef UTIL_STATE_H_
#define UTIL_STATE_H_

#include <Arduino.h>

#define DISTANCE_MIN 13
#

typedef enum {
    STATE_INIT = 0,
    STATE_SEARCH,
    STATE_APPROACH_FORWARD_TUNE,
    STATE_MOVE_FORWARD,
    STATE_TAPE_DETECTED,
    STATE_COLLECT
} State_t;

volatile uint32_t state = STATE_INIT;


#endif