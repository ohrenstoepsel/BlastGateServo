/*
 * BlastGateServo.ino - Optimierte Hauptdatei für das Blast Gate System
 *
 * Optimierungen:
 * - Nicht-blockierende Steuerung mit millis() statt delay()
 * - Effiziente Button-Verarbeitung mit Entprellung
 * - Saubere Initialisierung und Debugging-Infos
 * - Automatische Sensorauswertung zur Servo-Steuerung (optional mit ACSensoren)
 * - Verhindert unnötige Servo-Bewegungen durch Doppeltrigger
 * - Servos werden nach Bewegung stromlos gemacht, um Energie zu sparen
 *
 * Credits: Originalcode von Greg Pringle
 */

#include "Arduino.h"
#include "GateServos.h"
#include "AcSensors.h"
#include "Configuration.h"

GateServos gateController;
#if HAS_ACSENSOR
AcSensors sensorController;
#endif

unsigned long lastButtonPress = 0;
const int debounceTime = 200; // Entprellzeit für den Button
bool buttonPressed = false;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    gateController.initializeGates();
    #if HAS_ACSENSOR
    sensorController.initializeSensors();
    #endif
    DEBUG_INFO("System gestartet\n");
}

void loop() {
    // Button-Eingabe verarbeiten
    if (HAS_BUTTON && digitalRead(BUTTON_PIN) == LOW && !buttonPressed && millis() - lastButtonPress > debounceTime) {
        buttonPressed = true;
        lastButtonPress = millis();
        int nextGate = (gateController.getCurrentOpenGate() + 1) % NUM_GATES;
        gateController.manuallyOpenGate(nextGate);
        DEBUG_INFO("Button gedrückt - nächstes Gate öffnen: ");
        DEBUG_INFO(nextGate);
        DEBUG_INFO("\n");
    }
    if (digitalRead(BUTTON_PIN) == HIGH) {
        buttonPressed = false;
    }
    
    #if HAS_ACSENSOR
    // Sensorauslesung und automatische Steuerung
    sensorController.readSensors();
    for (int i = 0; i < NUM_AC_SENSORS; i++) {
        if (sensorController.isTriggered(i) && gateController.getCurrentOpenGate() != i) {
            gateController.openGate(i);
            DEBUG_INFO("Sensor ausgelöst - Gate öffnen: ");
            DEBUG_INFO(i);
            DEBUG_INFO("\n");
        }
    }
    #endif
    
    // Servos aktualisieren, um nicht-blockierende Steuerung zu gewährleisten
    gateController.updateServos();
    
    // Servos stromlos machen, wenn sie nicht in Bewegung sind
    gateController.disableServosIfIdle();
}
