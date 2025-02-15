#include "Arduino.h"
#include "Configuration.h"
#include "AcSensors.h"

AcSensors::AcSensors() { }

//////////////////////////////////////////////////////////////////
// InitializeSensors()
//
// Initialize sensors and read a baseline sensor reading with tools off
//
//////////////////////////////////////////////////////////////////
void AcSensors::InitializeSensors() {
  for (int x = 0; x < NUM_AC_SENSORS; x++) {
    pinMode(LED_PINS[x], OUTPUT);
  }

  DPRINTLN("Getting Sensor Readings");
  // getAvgOffSensorReadings();
  getMaxOffSensorReadings();
}

//////////////////////////////////////////////////////////////////
// AvgSensorReading(int forsensor)
//
// Returns an average of the last X sensors readings for given sensor
//
//////////////////////////////////////////////////////////////////
float AcSensors::AvgSensorReading(int forsensor) {
  int res = 0;
  for (int x = 0; x < AVG_READINGS; x++) {
    res = res + recentReadings[forsensor][x];
  }
  return (float)res / (float)AVG_READINGS;
}

//////////////////////////////////////////////////////////////////
// getMaxOffSensorReadings()
//
// Poll for NUM_OFF_MAX_SAMPLES ms to determine maximum 'off' sensor
// reading for this sensor
//
//////////////////////////////////////////////////////////////////
void AcSensors::getMaxOffSensorReadings() {
  for (int x = 0; x < NUM_AC_SENSORS; x++) {
    int maxsensorval = 0;
    for (long y = 0; y < NUM_OFF_MAX_SAMPLES; y++) {
      int sensorval = analogRead(AC_SENSOR_PINS[x]);
      if (sensorval > maxsensorval) maxsensorval = sensorval;
      delay(1);
    }
    offReadings[x] = maxsensorval;
    DPRINT("OFF READING: ");
    DPRINTLN(offReadings[x]);
  }
}

//////////////////////////////////////////////////////////////////
// ReadSensors()
//
// Read values for AC current sensors and add to list of values we will average
//
//////////////////////////////////////////////////////////////////
void AcSensors::ReadSensors() {
  curreadingindex++;
  if (curreadingindex >= AVG_READINGS) curreadingindex = 0;

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  for (int cursensor = 0; cursensor < NUM_AC_SENSORS; cursensor++) {
    int sensorValue = analogRead(AC_SENSOR_PINS[cursensor]);
    recentReadings[cursensor][curreadingindex] = sensorValue;
  }
}

//////////////////////////////////////////////////////////////////
// DisplayMeter()
//
// Use LEDs to display a meter for positioning AC sensor clamps. 
// Turn on a device like a tool or a space heater on the cable the sensor is on.
// Rotate the sensor around the cable until the light for that 
// sensor is flashing as fast as possible (or has become solidly lit).
//
//////////////////////////////////////////////////////////////////
void AcSensors::DisplayMeter() {
  for (int cursensor = 0; cursensor < NUM_AC_SENSORS; cursensor++) {
    int avgthissensor = AvgSensorReading(cursensor);
    float percent = (float)(avgthissensor - offReadings[cursensor]) / (920 - offReadings[cursensor]);
    blinktimers[cursensor] += 1;
    int blinklen = MAX_BLINK_LEN * (1 - percent);

    // Print out the value you read:
    DPRINT(percent);
    DPRINT("    ");
    DPRINT(avgthissensor);
    DPRINT(" BlinkLen:");
    DPRINT(blinklen);
    DPRINT(" BlinkTimer:");
    DPRINT(blinktimers[cursensor]);
    DPRINTLN("");

    if (blinktimers[cursensor] > blinklen) {
      blinktimers[cursensor] = 0;
      if (blinkon[cursensor]) {
        digitalWrite(LED_PINS[cursensor], LOW);
        DPRINT("LED OFF ");
        DPRINTLN(cursensor);
        blinkon[cursensor] = false;
      } else {
        digitalWrite(LED_PINS[cursensor], HIGH);
        DPRINT("LED ON");
        DPRINTLN(cursensor);
        blinkon[cursensor] = true;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////
// getAvgOffSensorReadings()
//
// Determine average 'off' reading for each sensor.  
// Currently unused but might be useful if you have a tool that isn't 
// drawing enough power to trigger the sensor using max values.
//
//////////////////////////////////////////////////////////////////
void AcSensors::getAvgOffSensorReadings() {
  for (int x = 0; x < NUM_AC_SENSORS; x++) {
    long totalsensorval = 0;
    for (int y = 0; y < NUM_OFF_SAMPLES; y++) {
      totalsensorval += analogRead(AC_SENSOR_PINS[x]);
      delay(100);
    }
    offReadings[x] = (float)totalsensorval / (float)NUM_OFF_SAMPLES;
    DPRINT("OFF READING: ");
    DPRINTLN(offReadings[x]);
  }
}

//////////////////////////////////////////////////////////////////
// Triggered(int forsensor)
//
// Returns true if the given AC current sensor number is triggered
//////////////////////////////////////////////////////////////////
bool AcSensors::Triggered(int forsensor) {
  return (AvgSensorReading(forsensor) > ((float)offReadings[forsensor] * AC_SENSOR_SENSITIVITY));
}

//////////////////////////////////////////////////////////////////
// displayaverages(int cursensor)
//
// Debugging function to display values polled for given sensor
//////////////////////////////////////////////////////////////////
void AcSensors::displayaverages(int cursensor) {
  for (int i = 0; i < AVG_READINGS; i++) {
    Serial.print(recentReadings[cursensor][i]);
    Serial.print(" ");
  }
  Serial.println();
}


