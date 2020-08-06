#ifndef UTIL_MOTOR_H_
#define UTIL_MOTOR_H_

#include <Arduino.h>

#define LEFT_OFFSET 15
#define RIGHT_OFFSET 0
#define RUN_FORWARD 767
#define RUN_REVERSE 767

#define MOTOR_OFF 0
#define PWMFREQ 500

#define RUN_FORWARD 767
#define RUN_REVERSE 767

#define FULL_SPEED 1023
#define TURN_SPEED 1000


void motor_stop();
void motor_run(uint32_t speed, uint32_t time_ms);
void motor_reverse(uint32_t speed, uint32_t time_ms);
void turn_Left(uint32_t speed, uint32_t time_ms);
void turn_Right(uint32_t speed, uint32_t time_ms);

#endif