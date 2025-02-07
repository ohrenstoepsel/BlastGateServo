/*
 * AcSensors.cpp - Optimierte AC-Sensorsteuerung
 */

#include "AcSensors.h"

AcSensors::AcSensors() {
    for (int i = 0; i < NUM_AC_SENSORS; i++) {
        sensors[i] = {AC_SENSOR_PINS[i], LED_PINS[i], 0.0, AC_SENSOR_SENSITIVITY, {}, 0};
    }
    lastReadingTime = 0;
}

void AcSensors::initializeSensors() {
    for (int i = 0; i < NUM_AC_SENSORS; i++) {
        pinMode(sensors[i].ledPin, OUTPUT);
        digitalWrite(sensors[i].ledPin, LOW);
    }
}

void AcSensors::readSensors() {
    if (millis() - lastReadingTime < 50) return; // Nicht zu oft auslesen
    lastReadingTime = millis();
    
    for (int i = 0; i < NUM_AC_SENSORS; i++) {
        int sensorValue = analogRead(sensors[i].pin);
        sensors[i].recentReadings[sensors[i].currentReadingIndex] = sensorValue;
        sensors[i].currentReadingIndex = (sensors[i].currentReadingIndex + 1) % AVG_READINGS;
    }
}

bool AcSensors::isTriggered(int sensorNum) {
    if (sensorNum < 0 || sensorNum >= NUM_AC_SENSORS) return false;
    
    int total = 0;
    for (int i = 0; i < AVG_READINGS; i++) {
        total += sensors[sensorNum].recentReadings[i];
    }
    float avg = (float)total / AVG_READINGS;
    return avg > (sensors[sensorNum].offReading * sensors[sensorNum].sensitivity);
}
