/*
 * Configuration.h - Optimierte Konfigurationsdatei
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONFIGURATION_H_VERSION 01

// Debugging aktivieren/deaktivieren
#define DEBUG   // Aktiviert generelles Debugging
//#define DEBUGMETER // Debugging für Sensoren

// Debug Level (INFO = 1, WARNING = 2, ERROR = 3)
#ifdef DEBUG
  #define DEBUG_LEVEL 1
  #define DEBUG_INFO(...)    if (DEBUG_LEVEL <= 1) { Serial.print(__VA_ARGS__); }
  #define DEBUG_WARNING(...) if (DEBUG_LEVEL <= 2) { Serial.print(__VA_ARGS__); }
  #define DEBUG_ERROR(...)   if (DEBUG_LEVEL <= 3) { Serial.print(__VA_ARGS__); }
#else
  #define DEBUG_INFO(...)
  #define DEBUG_WARNING(...)
  #define DEBUG_ERROR(...)
#endif

// === SERVO EINSTELLUNGEN === //
constexpr int CLOSE_DELAY = 1000; // Zeit zum Schließen eines Gates (ms)
constexpr int OPEN_DELAY = 1000;  // Wartezeit nach letzter Betätigung zum Öffnen (ms)

// === BUTTON EINSTELLUNGEN === //
constexpr bool HAS_BUTTON = true;  // Falls ein Button verwendet wird
constexpr int BUTTON_PIN = 2;      // Pin für den Button

// === SENSOR EINSTELLUNGEN === //
constexpr bool HAS_ACSENSOR = false; // Falls AC-Sensoren verwendet werden
constexpr int NUM_OFF_SAMPLES = 50; // Anzahl der Samples für Sensor-Kalibrierung
constexpr int NUM_OFF_MAX_SAMPLES = 500; // Maximale Sample-Zeit beim Start
constexpr int AVG_READINGS = 25;   // Durchschnittliche Sensormessungen
constexpr float AC_SENSOR_SENSITIVITY = 2.0; // Empfindlichkeit des AC-Sensors

// === SERVO PIN DEFINITIONS === //
constexpr int SERVO_PINS[] = {9, 10, 11};
constexpr int SERVO_MAX[] = {180, 180, 180};
constexpr int SERVO_MIN[] = {0, 0, 0};

// === AC SENSOR PINS === //
constexpr int AC_SENSOR_PINS[] = {A0, A1,A2};

// === LED PINS === //
constexpr int LED_PINS[] = {3, 4, 5};

#endif // CONFIGURATION_H
