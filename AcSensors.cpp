/*
 * AcSensors.cpp - Optimierte AC-Sensorsteuerung
 */

#include "AcSensors.h"

AcSensors::AcSensors() {
    int sensorCount = sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0]);
    for (int i = 0; i < sensorCount; i++) {
        sensors[i] = {AC_SENSOR_PINS[i], LED_PINS[i], 0.0, AC_SENSOR_SENSITIVITY, {}, 0};
    }
}

void AcSensors::initializeSensors() {
    int sensorCount = sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0]);
    for (int i = 0; i < sensorCount; i++) {
        pinMode(sensors[i].ledPin, OUTPUT);
        digitalWrite(sensors[i].ledPin, LOW);
    }
}

void AcSensors::readSensors() {
    int sensorCount = sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0]);
    for (int i = 0; i < sensorCount; i++) {
        int sensorValue = analogRead(sensors[i].pin);
        sensors[i].recentReadings[sensors[i].currentReadingIndex] = sensorValue;
        sensors[i].currentReadingIndex = (sensors[i].currentReadingIndex + 1) % AVG_READINGS;
    }
}

bool AcSensors::isTriggered(int sensorNum) {
    int sensorCount = sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0]);
    if (sensorNum < 0 || sensorNum >= sensorCount) return false;
    
    int total = 0;
    for (int i = 0; i < AVG_READINGS; i++) {
        total += sensors[sensorNum].recentReadings[i];
    }
    int avg = total / AVG_READINGS;
    return avg > (sensors[sensorNum].offReading * sensors[sensorNum].sensitivity);
}
