/*
  AcSensors.h - Library for http://moderndevice.com/current AC sensors
  Created by Greg Pringle 2019-01-20
  Released into the public domain.
*/
#ifndef AcSensor_h
#define AcSensor_h

#include "Arduino.h"
#include "Configuration.h"

class AcSensors {
  static const int max_sensors = 8; // Maximum number of sensors supported

  int curreadingindex = 0;
  int blinktimers[max_sensors] = {0, 0, 0, 0, 0, 0, 0, 0};
  bool blinkon[max_sensors] = {false, false, false, false, false, false, false, false};
  const int sensorPins[max_sensors] = {AC_SENSOR_PINS[0], AC_SENSOR_PINS[1], AC_SENSOR_PINS[2], AC_SENSOR_PINS[3], AC_SENSOR_PINS[4], AC_SENSOR_PINS[5], AC_SENSOR_PINS[6], AC_SENSOR_PINS[7]};
  const int ledpin[max_sensors] = {LED_PINS[0], LED_PINS[1], LED_PINS[2], LED_PINS[3], LED_PINS[4], LED_PINS[5], LED_PINS[6], LED_PINS[7]}; // LED pins
  float offReadings[NUM_AC_SENSORS];
  int recentReadings[NUM_AC_SENSORS][AVG_READINGS];

public:
  AcSensors();
  void InitializeSensors();               // Initialize sensors and read a baseline sensor reading with tools off
  float AvgSensorReading(int forsensor);  // Returns an average of the last X sensors readings for given sensor
  bool Triggered(int sensor);             // Returns true if the given AC current sensor number is triggered
  void getMaxOffSensorReadings();         // Poll for NUM_OFF_MAX_SAMPLES ms to determine maximum 'off' sensor reading for this sensor
  void getAvgOffSensorReadings();         // Determine average 'off' reading for each sensor
  void ReadSensors();                     // Read values for AC current sensors and add to list of values we will average
  void DisplayMeter();                    // Use LEDs to display a meter for positioning AC sensor clamps
  void displayaverages(int cursensor);    // Debugging function to display values polled for given sensor
  static const int num_ac_sensors = NUM_AC_SENSORS;
};

#endif
