#include "util_motor.h"
#include "pin_define.h"

void motor_stop() {
  pwm_start(MOTOR_LF, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_LB, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RF, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RB, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
}

void motor_run(uint32_t speed, uint32_t time_ms) {
  pwm_start(MOTOR_LF, PWMFREQ, speed + LEFT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_LB, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RF, PWMFREQ, speed + RIGHT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RB, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  delay(time_ms);
  motor_stop();
}

void motor_reverse(uint32_t speed, uint32_t time_ms) {
  pwm_start(MOTOR_LF, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_LB, PWMFREQ, speed + LEFT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RF, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RB, PWMFREQ, speed + RIGHT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  delay(time_ms);
  motor_stop();
}

void turn_Left(uint32_t speed, uint32_t time_ms) {
  pwm_start(MOTOR_LF, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_LB, PWMFREQ, speed + LEFT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RF, PWMFREQ, speed + RIGHT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RB, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  delay(time_ms);
  motor_stop();
}

void turn_Right(uint32_t speed, uint32_t time_ms) {
  pwm_start(MOTOR_LF, PWMFREQ, speed + LEFT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_LB, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RF, PWMFREQ, MOTOR_OFF, RESOLUTION_10B_COMPARE_FORMAT);
  pwm_start(MOTOR_RB, PWMFREQ, speed + RIGHT_OFFSET, RESOLUTION_10B_COMPARE_FORMAT);
  delay(time_ms);
  motor_stop();
}

