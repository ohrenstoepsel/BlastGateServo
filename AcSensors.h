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
    ACSensor sensors[NUM_AC_SENSORS];
    int lastReadingTime;
    
  public:
    AcSensors();
    void initializeSensors();
    void readSensors();
    bool isTriggered(int sensorNum);
};

#endif // ACSENSORS_H
