/*
 * AcSensors.h - Optimierte AC-Sensorsteuerung
 */

#ifndef ACSENSORS_H
#define ACSENSORS_H

#include "Arduino.h"
#include "Configuration.h"

struct ACSensor {
    int pin;
    int ledPin;
    float offReading;
    float sensitivity;
    int recentReadings[AVG_READINGS];
    int currentReadingIndex;
};

class AcSensors {
  private:
    ACSensor sensors[sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0])];
    
  public:
    AcSensors();
    void initializeSensors();
    void readSensors();
    bool isTriggered(int sensorNum);
};

#endif // ACSENSORS_H
