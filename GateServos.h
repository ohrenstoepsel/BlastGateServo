/*
 * GateServos.h - Optimierte Servo-Steuerung
 */

#ifndef GATESERVOS_H
#define GATESERVOS_H

#include "Arduino.h"
#include <Servo.h>
#include "Configuration.h"

struct GateServo {
    int pin;
    int minPosition;
    int maxPosition;
    int ledPin;
    bool isMoving;
    unsigned long moveStartTime;
};

class GateServos {
  private:
    GateServo servos[sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0])];
    Servo servoObjects[sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0])];
    int currentOpenGate = -1;
    
  public:
    GateServos();
    void initializeGates();
    void openGate(int gateNum);
    void closeGate(int gateNum);
    void updateServos();
    void manuallyOpenGate(int gateNum);
    int getCurrentOpenGate() { return currentOpenGate; }
    void disableServosIfIdle();
    void highlightGateLED(int gateNum);
};

#endif // GATESERVOS_H
