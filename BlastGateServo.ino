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
 * - Button-Steuerung: LEDs durchschalten mit Tastendruck, Gate nach Wartezeit öffnen
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
unsigned long lastStableTime = 0;
unsigned long selectionStartTime = 0;
const int debounceTime = 300; // Entprellzeit für den Button erhöht
const int stabilityThreshold = 50; // Zeit in ms für stabilen LOW-Zustand
const int selectionTimeout = 2000; // Zeit in ms nach letztem Tastendruck, bis Gate öffnet
int selectedGate = 0;
bool buttonPressed = false;
bool selectionActive = false;
bool buttonReleased = true;
bool lastButtonState = HIGH;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    gateController.initializeGates();
    #if HAS_ACSENSOR
    sensorController.initializeSensors();
    #endif
    Serial.print("System gestartet\n");
}

void loop() {
    // Button-Zustand lesen und stabilen Zustand prüfen
    bool currentButtonState = digitalRead(BUTTON_PIN);
    unsigned long currentMillis = millis();

    if (currentButtonState == LOW && lastButtonState == HIGH) {
        // Übergang von HIGH zu LOW erkannt
        lastStableTime = currentMillis;
    }

    if (currentButtonState == LOW && (currentMillis - lastStableTime > stabilityThreshold) && buttonReleased) {
        // Button stabil LOW und nicht mehr released
        lastButtonPress = currentMillis;
        selectedGate = (selectedGate + 1) % (sizeof(LED_PINS) / sizeof(LED_PINS[0]));
        gateController.highlightGateLED(selectedGate); // LEDs durchschalten
        selectionStartTime = currentMillis; // Startzeit der Auswahl setzen
        selectionActive = true;
        buttonReleased = false;
        Serial.print("LED Durchschalten - Ausgewähltes Gate: ");
        Serial.println(selectedGate);
    }

    if (currentButtonState == HIGH) {
        // Button losgelassen
        buttonReleased = true;
    }

    lastButtonState = currentButtonState; // Aktualisiere den Zustand

    // Automatisches Öffnen nach Timeout, aber nur wenn eine Auswahl aktiv ist
    if (selectionActive && currentMillis - selectionStartTime > selectionTimeout) {
        gateController.manuallyOpenGate(selectedGate);
        Serial.print("Timeout erreicht - Öffne Gate: ");
        Serial.println(selectedGate);
        selectionActive = false; // Auswahl zurücksetzen
    }

    #if HAS_ACSENSOR
    // Sensorauslesung und automatische Steuerung
    sensorController.readSensors();
    for (int i = 0; i < (sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0])); i++) {
        if (sensorController.isTriggered(i) && gateController.getCurrentOpenGate() != i) {
            gateController.openGate(i);
            Serial.print("Sensor ausgelöst - Gate öffnen: ");
            Serial.println(i);
        }
    }
    #endif

    // Servos aktualisieren, um nicht-blockierende Steuerung zu gewährleisten
    gateController.updateServos();

    // Servos stromlos machen, wenn sie nicht in Bewegung sind
    gateController.disableServosIfIdle();
}
