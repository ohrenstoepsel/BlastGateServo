/*
 * GateServos.cpp - Optimierte Servo-Steuerung
 */

#include "GateServos.h"

GateServos::GateServos() {
    int servoArraySize = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    int gateCount = min(NUM_GATES, servoArraySize);
    for (int i = 0; i < gateCount; i++) {
        servos[i] = {SERVO_PINS[i], SERVO_MIN[i], SERVO_MAX[i], LED_PINS[i], false, 0};
    }
    currentOpenGate = -1;
}

void GateServos::initializeGates() {
    int servoArraySize = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    int gateCount = min(NUM_GATES, servoArraySize);
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

void GateServos::openGate(int gateNum) {
    int servoArraySize = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    if (gateNum < 0 || gateNum >= min(NUM_GATES, servoArraySize)) return;
    
    if (currentOpenGate == gateNum) return; // Verhindert doppeltes Öffnen desselben Gates
    
    if (currentOpenGate != -1) {
        closeGate(currentOpenGate);
        delay(200); // Kleine Wartezeit, um Servo-Übergänge zu vermeiden
    }
    
    DEBUG_INFO("Öffne Gate: ");
    DEBUG_INFO(gateNum);
    DEBUG_INFO("\n");
    
    servos[gateNum].isMoving = true;
    servos[gateNum].moveStartTime = millis();
    servoObjects[gateNum].attach(servos[gateNum].pin);
    servoObjects[gateNum].write(servos[gateNum].minPosition);
    digitalWrite(servos[gateNum].ledPin, HIGH);
    currentOpenGate = gateNum;
}

void GateServos::closeGate(int gateNum) {
    int servoArraySize = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    if (gateNum < 0 || gateNum >= min(NUM_GATES, servoArraySize)) return;
    
    if (currentOpenGate != gateNum) return; // Verhindert doppeltes Schließen
    
    DEBUG_INFO("Schließe Gate: ");
    DEBUG_INFO(gateNum);
    DEBUG_INFO("\n");
    
    servos[gateNum].isMoving = true;
    servos[gateNum].moveStartTime = millis();
    servoObjects[gateNum].attach(servos[gateNum].pin);
    servoObjects[gateNum].write(servos[gateNum].maxPosition);
    digitalWrite(servos[gateNum].ledPin, LOW);
    
    // Verzögertes Freigeben des Servos, um Zittern zu verhindern
    delay(300);
    servoObjects[gateNum].detach();
    servos[gateNum].isMoving = false;
    currentOpenGate = -1; // Kein Gate mehr offen
}

void GateServos::updateServos() {
    unsigned long currentMillis = millis();
    int servoArraySize = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    int gateCount = min(NUM_GATES, servoArraySize);
    for (int i = 0; i < gateCount; i++) {
        if (servos[i].isMoving && (currentMillis - servos[i].moveStartTime > CLOSE_DELAY)) {
            servos[i].isMoving = false;
            // Keine sofortige Detach(), um Servos stabil zu halten
        }
    }
}

void GateServos::disableServosIfIdle() {
    int servoArraySize = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);
    int gateCount = min(NUM_GATES, servoArraySize);
    for (int i = 0; i < gateCount; i++) {
        if (!servos[i].isMoving) {
            servoObjects[i].detach();
        }
    }
}

void GateServos::manuallyOpenGate(int gateNum) {
    openGate(gateNum);
}
