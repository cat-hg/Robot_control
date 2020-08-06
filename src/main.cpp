#include <Wire.h>
#include <Arduino.h>
#include "pin_define.h"
#include "util_motor.h"
#include "util_servo.h"
#include "util_state.h"
#include <Ultrasonic.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible

#define NUM_READINGS 10
#define RESO_OFFSET 20
#define DISTANCE_MARGIN 2
#define TARGET_MARGIN 1

#define TAPE_THRESHOLD 120

#define DISTANCE_SWEEP 17


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint32_t tape_detected_L;
uint32_t tape_detected_R;
uint32_t distance;
uint32_t test_dist;
uint32_t distance_target;
int dists[NUM_READINGS];

Ultrasonic ultrasonic(TRIG, ECHO);

bool tapeDetected();


void takeMeasurements(uint32_t i) {
  distance = ultrasonic.read();
  Serial.println(distance);
  dists[i] = distance;

}

void sweepLeft() {
    for (int i = 0; i < NUM_READINGS; i++) {
        turn_Left(TURN_SPEED, 150);
        takeMeasurements(i);
        delayMicroseconds(50);
    }

    int myMin = 10000;
    int myMinIndex = -1;

    for (int j = 0; j < NUM_READINGS; j++) {
        if (dists[j] < myMin) {
            myMin = dists[j];
            myMinIndex = j;
        }
    }
    
    // this was for debugging
    delay(1000);
    Serial.println("smallest");
    Serial.println(dists[myMinIndex]);

    turn_Right(TURN_SPEED, (NUM_READINGS-myMinIndex+1) * 150);
}

void approach() {
  int distToCan = ultrasonic.read();
    while (distToCan > DISTANCE_SWEEP) {
      motor_run(RUN_FORWARD, 400);
    }
}

bool tapeDetected() {
  if (analogRead(TAPESENSOR_R) > TAPE_THRESHOLD || analogRead(TAPESENSOR_L) > TAPE_THRESHOLD) {
    return true;
  }
   return false;
}

void fineTune() {
  uint32_t distToCan = ultrasonic.read();
  int turn_count=0;

  turn_Left(TURN_SPEED, 1000);
  // while (true) loops are too scary
  while (turn_count < 1000) {
    turn_Right(TURN_SPEED, 100);
    if (distToCan <= distance_target + DISTANCE_MARGIN) {
      motor_run(RUN_FORWARD, 300);
      return;
    }
    turn_count += 100;
  }
  return;
}


bool search() {
  Serial.println("searching...");
  armOpen();
  delay(250);
  Serial.println("arms open");

  for (int i = 0; i < NUM_READINGS; i++) {
    if (tapeDetected()) {return false;}
        takeMeasurements(i);
        turn_Left(TURN_SPEED, 100);
        delayMicroseconds(50);
    }

    int myMin = 10000;
 

    for (int j = 0; j < NUM_READINGS; j++) {
        if (dists[j] < myMin) {
            myMin = dists[j];
          
        }
    }
    Serial.println("found min");
    distance_target = myMin;
    Serial.println("target");
    Serial.println(distance_target);
    delay(1000);

    while (true) {
      if (tapeDetected()) {
        return false;
      }
      if (ultrasonic.read() <= distance_target + DISTANCE_MARGIN) {
        Serial.println("final");
        Serial.println(ultrasonic.read());
        return true;
      }
      
      turn_Right(TURN_SPEED, 100);
      delay(20);
      
    }
  
  
}


void simpleTest() {
  // This just runs into the can and deposits it
  rampDown(true);
  motor_run(1023,1000);
  motor_stop();
  delay(500);
  rampDown(false);
  delay(750);
  rampDown(true);
}

void sonarTest() {
  // Quick sonar distancing check
  test_dist = ultrasonic.read();
  Serial.println(test_dist);
}

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


  // Pin init
  pinMode(MOTOR_LF, OUTPUT);
  pinMode(MOTOR_LB, OUTPUT);
  pinMode(MOTOR_RF, OUTPUT);
  pinMode(MOTOR_RB, OUTPUT);
  pinMode(MOTOR_CONTROL, INPUT);
  pinMode(SERVO_CONTROL, INPUT_PULLUP);
  pinMode(TAPESENSOR_L, INPUT_PULLDOWN);
  pinMode(TAPESENSOR_R, INPUT_PULLDOWN);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT_PULLDOWN);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(2000);

  // Displays "Manhattan Project" on the screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Manhattan Project");
  display.display();
  delay(1000);
  display.clearDisplay();

  // Backup into box and attach

  //motor_reverse(800, 800);
  //display.println("Got the box!");
  //display.display();
  //delay(230);
  display.clearDisplay();
  display.println("I am going to eat your batteries.");
  display.display();
  //motor_run(900, 800);
  
  while (true) {
        if (digitalRead(BUTTON)) {
            display.println("we're done waiting!");
            display.display();
            state = STATE_INIT; 
            break;
        }
  }
  display.clearDisplay();
}


void loop() {
  
  search();
  delay(5000);
  /*
  if (state == STATE_INIT) {
    // Back up onto box and set servo arm position
    armOpen();
    delay(500);
    motor_reverse(800, 800);
    delay(500);
    motor_run(RUN_FORWARD, 1000);
    delay(1000);
    state = STATE_SEARCH;
  }

  else if (state == STATE_SEARCH) {
    // Searches for a can and stops when one is found OR tape is detected
    if (search()) {
      // its always undershooting so give it an extra push
      turn_Right(TURN_SPEED, 200);
      state = STATE_MOVE_FORWARD;
    }
    else {
      state = STATE_TAPE_DETECTED;
    }
  
  }

  else if (state == STATE_MOVE_FORWARD) {
    if (ultrasonic.read() > distance_target + TARGET_MARGIN) {
      fineTune();
      state = STATE_MOVE_FORWARD;
    }
    
    if (ultrasonic.read() <= DISTANCE_SWEEP) {
      state = STATE_COLLECT;
    }

    else {
      motor_run(RUN_FORWARD, 200);
      state = STATE_MOVE_FORWARD;
    }
  }

   //else if (state == STATE_APPROACH_FORWARD_TUNE) {
     // Drives forward to can making sure to stay centered
    //approach();
    //state = STATE_COLLECT;
  //}

  else if (state == STATE_COLLECT) {
    armCollectCan();
    delay(250);
    state = STATE_SEARCH;
  }

  else if (state == STATE_TAPE_DETECTED) {
    // Tape is detected, rotate 90 degrees
    motor_stop();
    turn_Left(TURN_SPEED, 900);
    state = STATE_SEARCH; 
  }
  
*/
}
