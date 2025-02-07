/*
 * GateServos.cpp - Optimierte Servo-Steuerung mit LED-Durchschaltung
 */

#include "GateServos.h"

GateServos::GateServos() {
    int gateCount = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    for (int i = 0; i < gateCount; i++) {
        servos[i] = {SERVO_PINS[i], SERVO_MIN[i], SERVO_MAX[i], LED_PINS[i], false, 0};
    }
    currentOpenGate = -1;
}

void GateServos::initializeGates() {
    int gateCount = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    for (int i = 0; i < gateCount; i++) {
        pinMode(servos[i].ledPin, OUTPUT);
        digitalWrite(servos[i].ledPin, LOW);
        servoObjects[i].attach(servos[i].pin);
        servoObjects[i].write(servos[i].maxPosition); // Starte in geschlossener Position
        delay(500);
        servoObjects[i].detach();
        servos[i].isMoving = false;
    }
}

void GateServos::highlightGateLED(int gateNum) {
    int gateCount = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
    for (int i = 0; i < gateCount; i++) {
        digitalWrite(servos[i].ledPin, (i == gateNum) ? HIGH : LOW);
    }
}

void GateServos::openGate(int gateNum) {
    int gateCount = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    if (gateNum < 0 || gateNum >= gateCount) return;
    
    if (currentOpenGate == gateNum) return; // Verhindert doppeltes Öffnen desselben Gates
    
    if (currentOpenGate != -1) {
        closeGate(currentOpenGate);
        delay(200);
    }
    
    servos[gateNum].isMoving = true;
    servos[gateNum].moveStartTime = millis();
    servoObjects[gateNum].attach(servos[gateNum].pin);
    servoObjects[gateNum].write(servos[gateNum].minPosition);
    digitalWrite(servos[gateNum].ledPin, HIGH);
    currentOpenGate = gateNum;
}

void GateServos::closeGate(int gateNum) {
    int gateCount = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    if (gateNum < 0 || gateNum >= gateCount) return;
    
    if (currentOpenGate != gateNum) return;
    
    servos[gateNum].isMoving = true;
    servos[gateNum].moveStartTime = millis();
    servoObjects[gateNum].attach(servos[gateNum].pin);
    servoObjects[gateNum].write(servos[gateNum].maxPosition);
    digitalWrite(servos[gateNum].ledPin, LOW);
    
    delay(300);
    servoObjects[gateNum].detach();
    servos[gateNum].isMoving = false;
    currentOpenGate = -1;
}

void GateServos::updateServos() {
    unsigned long currentMillis = millis();
    int gateCount = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    for (int i = 0; i < gateCount; i++) {
        if (servos[i].isMoving && (currentMillis - servos[i].moveStartTime > CLOSE_DELAY)) {
            servos[i].isMoving = false;
        }
    }
}

void GateServos::disableServosIfIdle() {
    int gateCount = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    for (int i = 0; i < gateCount; i++) {
        if (!servos[i].isMoving) {
            servoObjects[i].detach();
        }
    }
}

void GateServos::manuallyOpenGate(int gateNum) {
    openGate(gateNum);
}
