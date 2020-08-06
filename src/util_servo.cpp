#include "util_servo.h"
#include "pin_define.h"
#include <Arduino.h>

void rampDown(int isDown){
  if (isDown) {
    //120 DEGREES - RAMP GO DOWN
    pwm_start(SERVO_PIN, 50, 2070, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  } else {
    //0 DEGREES - RAMP GO UP
    pwm_start(SERVO_PIN, 50, 800, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  }
  return;
}

void armOpen() {
    pwm_start(SERVO_ARM, 50, 410, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
}

void armClose() {
    int angle_pwm = 410;
  // loop to slow down the servo arm
  while (angle_pwm < 1110) {
        pwm_start(SERVO_ARM, 50, angle_pwm, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        Serial.println(angle_pwm);
        angle_pwm += 10;
        delayMicroseconds(250);
  }
}

void rampLift() {
    pwm_start(SERVO_PIN, 50, 2060, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
}

void armCollectCan() {
  //arm open (0 degrees)
  armOpen();

  //arm close (90 degrees)
  armClose();

  delay(800);

  //arm open
  armOpen();
  
  delay(370);
  //ramp down 
  rampDown(true);

  //ramp up
  rampDown(false);

  delay(800);

  //ramp back down
  rampDown(true);
}
