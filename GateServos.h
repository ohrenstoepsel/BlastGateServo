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
    GateServo servos[NUM_GATES];
    Servo servoObjects[NUM_GATES];
    int currentOpenGate = -1;
    
  public:
    GateServos();
    void initializeGates();
    void openGate(int gateNum);
    void closeGate(int gateNum);
    void updateServos(); // Neue Funktion, die Servos mit millis() aktualisiert
    void manuallyOpenGate(int gateNum);
    int getCurrentOpenGate() { return currentOpenGate; } // Getter-Methode hinzugefügt
    void disableServosIfIdle(); // Neue Methode, um Servos stromlos zu machen
};

#endif // GATESERVOS_H
