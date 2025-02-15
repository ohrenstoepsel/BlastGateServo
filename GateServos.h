/*
  GateServos.h - Library for operating blast gate servos
  Created by Greg Pringle 2019-01-20
  Released into the public domain.
*/
#ifndef GateServo_h
#define GateServo_h

#include "Arduino.h"
#include <Servo.h>
#include "Configuration.h"

class GateServos {
  static const int num_servos = NUM_GATES;
  static const int num_leds = NUM_LEDS;

  const int* servopin = SERVO_PINS;
  const int* ledpin = LED_PINS;

  Servo myservo;  // create servo object to control a servo
                  // a maximum of eight servo objects can be created

public:    
  GateServos(int curopengate);  // initialize indicating currently open gate (usually -1 for none)
  void opengate(int gatenum);   // open the given gate number
  void closegate(int gatenum);  // close the given gate number
  void initializeGates();       // initialize gates and close them all
  void ledoff(int gatenum);     // turn off given LED
  void ledon(int gatenum);      // turn on given LED
  void ManuallyOpenGate(int gatenum);   // User manually opening given gate using the button
  int firstgateopen();                  // Returns ID of first gate that is open
  void testServo(int servopin);         // Test given servo pin (debug function)
  const int num_gates = NUM_GATES;      // Number of gates
  int curopengate = -1;                 // currently open gate selected manually with button
  const int run_time = RUN_TIME;        // ms delay to allow servo to run
  bool gateopen[num_servos] = {false};  // array indicating which gates are open
};

#endif
