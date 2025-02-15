#ifndef CONFIGURATION_H
#define CONFIGURATION_H 
#define CONFIGURATION_H_VERSION 01

// All user-defined settings required to configure operation are in this file

//#define DEBUG   // Enable debug output
//#define DEBUGMETER // Debug meter mode for placing sensors on cables

// Debugging Macros
#ifdef DEBUG
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     // DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   // DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     // now defines a blank line
  #define DPRINTLN(...)   // now defines a blank line
#endif

// Servo settings
#define STOP_PWM 90       // PWM value to stop the continuous servo
#define FULL_FORWARD_PWM 180 // PWM value for full speed forward
#define FULL_BACKWARD_PWM 0  // PWM value for full speed backward
#define RUN_TIME 200     // Time to run the servo in milliseconds

#define HAS_BUTTON true  // True if button is attached
#define BUTTON_PIN 2    // The number of the pushbutton pin

// Current sensor settings
#define HAS_AC_SENSORS true  // Set to false if AC sensors are not used
#define NUM_AC_SENSORS (sizeof(AC_SENSOR_PINS) / sizeof(AC_SENSOR_PINS[0]))  // Number of AC sensors connected
#define NUM_LEDS (sizeof(LED_PINS) / sizeof(LED_PINS[0]))  // Number of LEDs connected (optional, but should be 1 per gate)
#define NUM_GATES (sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]))  // Number of blast gates with servos connected
#define NUM_OFF_SAMPLES 50   // Number of samples when checking avg sensor off values (unused)
#define NUM_OFF_MAX_SAMPLES 500 // Milliseconds to sample for max off value for each gate when starting up
#define AVG_READINGS 25      // Number of readings to average when triggering gates, higher number is more accurate but more delay (no more than 50)
#define AC_SENSOR_SENSITIVITY 2.0 // Triggers on twice the max readings of the off setting. The closer to one, the more sensitive

#ifdef DEBUG
#define MAX_BLINK_LEN 10 // Blink faster when debugging to serial since slower
#else 
#define MAX_BLINK_LEN (1000 / NUM_LEDS) // Slowest blink rate in ms
#endif

// Servo Pins
const int SERVO_PINS[] = {9, 11}; // Pins for servos

// AC Sensor Pins
const int AC_SENSOR_PINS[] = {A0, A1}; // Pins for AC sensors

// LED Pins
const int LED_PINS[] = {3, 5}; // Pins for LEDs

#endif // CONFIGURATION_H
