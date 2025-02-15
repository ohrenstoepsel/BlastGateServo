#include "Arduino.h"
#include <Servo.h>
#include "Configuration.h"
#include "GateServos.h"

// Constructor.. usually called with -1 to indicate no gates are open
GateServos::GateServos(int initcuropengate) { 
  curopengate = initcuropengate;
}

// Open the given gate number
void GateServos::opengate(int gatenum) {
  DPRINT("OPENING GATE #"); 
  DPRINT(gatenum);
  DPRINT(" SERVO PIN:");
  DPRINT(servopin[gatenum]);
  DPRINT(" VALUE:");
  DPRINT(FULL_FORWARD_PWM); 
  DPRINT(" RUN TIME:");
  DPRINT(run_time);
  DPRINTLN("");
  curopengate = gatenum;
  digitalWrite(ledpin[gatenum], HIGH);
  myservo.attach(servopin[gatenum]);  // attaches the servo
  myservo.write(FULL_FORWARD_PWM); // run servo forward
  delay(run_time); // wait for gate to open
  myservo.write(STOP_PWM); // stop servo
  myservo.detach();
  DPRINTLN("OPENED GATE");
}

// Close the given gate number
void GateServos::closegate(int gatenum) {
  DPRINT("CLOSING GATE #");
  DPRINT(gatenum);
  DPRINTLN("");
  digitalWrite(ledpin[gatenum], LOW);
  myservo.attach(servopin[gatenum]);  // attaches the servo
  myservo.write(FULL_BACKWARD_PWM); // run servo backward
  delay(run_time); // wait for gate to close
  myservo.write(STOP_PWM); // stop servo
  myservo.detach();
  DPRINTLN("CLOSED GATE");
}

// Debug function to test servo by opening and closing it given a pin number
void GateServos::testServo(int servopin) {  
  DPRINT("TESTING SERVO #");
  DPRINTLN(servopin);
  myservo.attach(servopin);  // attaches the servo
  myservo.write(FULL_FORWARD_PWM); 
  delay(2000);
  DPRINTLN("Set to FULL_FORWARD_PWM");
  myservo.write(FULL_BACKWARD_PWM);
  delay(2000);
  DPRINTLN("Set to FULL_BACKWARD_PWM");
  myservo.write(STOP_PWM);
  myservo.detach();
}

// Initialize gates and close them all
void GateServos::initializeGates() {
  // Close all gates one by one
  for (int thisgate = 0; thisgate < NUM_GATES; thisgate++) {
    pinMode(ledpin[thisgate], OUTPUT);
    digitalWrite(ledpin[thisgate], HIGH);
    myservo.attach(servopin[thisgate]);  // attaches the servo
    myservo.write(FULL_BACKWARD_PWM); // run servo backward
    delay(run_time); // wait for gate to close
    myservo.write(STOP_PWM); // stop servo
    myservo.detach();
    digitalWrite(ledpin[thisgate], LOW);
  }
}

// Turn LED on for given gate number
void GateServos::ledon(int gatenum) {
  digitalWrite(ledpin[gatenum], HIGH);
}

// Turn LED off for given gate number
void GateServos::ledoff(int gatenum) {
  digitalWrite(ledpin[gatenum], LOW);
}

// User has pushed button to manually open given gate
void GateServos::ManuallyOpenGate(int curselectedgate) {
  if (curselectedgate == NUM_GATES) {
    closegate(curopengate);
    curopengate = -1;
    curselectedgate = -1;
  } else {
    if (curopengate > -1) {
      DPRINT("Button Closing Gate= ");  
      DPRINTLN(curopengate);
      closegate(curopengate);
    }      
    curopengate = curselectedgate;   
    DPRINT("Button Opening Gate= ");  
    DPRINTLN(curopengate);
    opengate(curopengate);      
  }
}

// Return index of first open gate or -1 for none
int GateServos::firstgateopen() {
  for (int curgate = 0; curgate < NUM_GATES; curgate++) {
    if (gateopen[curgate]) return curgate;
  }
  return -1;
}
