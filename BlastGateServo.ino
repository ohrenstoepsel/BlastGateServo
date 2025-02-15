/*  Blast gate servo controller for Arduino
 *   
 *   Allows a single push button to switch between different blast gates, with only a single one open at a time.
 *   Currently open blast gate is indicated by an LED.
 *    
 *   Requires:  A servo for each blast gate, one button
 *   Optional: An LED for each blast gate
 *   
 *   In the future I plan on adding an option to automate this based on detected AC current from the tool
 *   (if my sensor ever shows up)
 *   
 *   Created 2019-01-02 - Greg Pringle
 *   Updated 2019-01-20 - Greg Pringle - Refactored, added support for AC current sensor
 */
#include <Servo.h>
#include "Configuration.h"
#include "GateServos.h"
#include "AcSensors.h"

int buttonState = 0;         // variable for reading the pushbutton status
bool hasbutton = HAS_BUTTON; // define and initialize hasbutton
unsigned long lastButtonPushMs = 0; // Define the variable
int curSelectedGate = 0;
bool waitingforbuttonrelease = false;
int curopengate = -1; // Define and initialize the curopengate variable

#if HAS_AC_SENSORS
int offReadings[NUM_AC_SENSORS]; // Array to store off readings for each sensor
#endif

int avgSensorReading(int sensorIndex) {
  // Placeholder function to calculate average sensor reading
  // Replace with actual implementation
  return 0;
}
bool metermode = false;

GateServos gateservos(-1);  // object controlling blast gate servos
#if HAS_AC_SENSORS
AcSensors acsensors;        // Object controlling AC current sensors
#endif

void setup() {
  #ifdef DEBUG
  Serial.begin(9600); // Initialize serial communication for debugging
  #endif

  gateservos.initializeGates(); // Initialize the gate servos

  // Initialize the pushbutton pin as an input
  if (HAS_BUTTON) pinMode(BUTTON_PIN, INPUT);
  
  // Check if the button is held down on startup to enter meter mode
  if (digitalRead(BUTTON_PIN) == HIGH)
      metermode = true;
  else
      metermode = false;

  #ifdef DEBUGMETER
      metermode = true; // Force meter mode if DEBUGMETER is defined
  #endif

  #if HAS_AC_SENSORS
  acsensors.InitializeSensors(); // Initialize the AC sensors
  #endif
  
  // Ensure only the gate with the lowest pin number is open and its LED is on
  int lowestPinIndex = 0;
  for (int i = 1; i < NUM_GATES; i++) {
    if (SERVO_PINS[i] < SERVO_PINS[lowestPinIndex]) {
      lowestPinIndex = i;
    }
  }
  curSelectedGate = lowestPinIndex;
  gateservos.gateopen[curSelectedGate] = true;
  gateservos.ledon(curSelectedGate);
  gateservos.opengate(curSelectedGate);
}

void loop() 
{
  bool toolon = false; // Indicates if there is any current sensed

  #if HAS_AC_SENSORS
  acsensors.ReadSensors(); // Read all the AC current sensors
  #endif
  
  if (metermode) {
    #if HAS_AC_SENSORS
    acsensors.DisplayMeter();  // If in meter mode, use LED lights to display sensor signal
    #endif
  } else { // Not in meter mode
    #if HAS_AC_SENSORS
    for (int cursensor = 0; cursensor < NUM_AC_SENSORS; cursensor++) {
        // Check if this sensor is triggered by a power tool
        if (acsensors.Triggered(cursensor)) {
            // Tool is active, output info to debug
            DPRINT(" TOOL ON #"); DPRINT(cursensor); DPRINT(" OFF READING:"); DPRINT(offReadings[cursensor]); DPRINT(" AVG SENSOR READING:"); DPRINTLN(avgSensorReading(cursensor));

            // Ignore button if tool detected
            waitingforbuttonrelease = false;
            toolon = true;
            if (curSelectedGate != cursensor) curSelectedGate = cursensor;

            // Gate hasn't been opened yet, open it
            if (gateservos.gateopen[cursensor] != true) {
                gateservos.gateopen[cursensor] = true;
                gateservos.ledon(cursensor);
                gateservos.opengate(cursensor);
            }
        } else {
            // this tool is not active and gate hasn't been closed yet. Close it.
            if (gateservos.gateopen[cursensor]) {
                gateservos.gateopen[cursensor] = false;
                gateservos.ledoff(cursensor);
                DPRINT(" TOOL OFF #"); DPRINTLN(cursensor);
                gateservos.closegate(cursensor);      
                curSelectedGate = gateservos.firstgateopen();  // change currently active gate to first open one
            }
        }
    }
    #endif

    // read the state of the pushbutton value.  
    // Allow the user to light up the appropriate gate and pause before opening/closing the gates.
    if (hasbutton) buttonState = digitalRead(BUTTON_PIN); // Correct variable name
    if (hasbutton && !toolon && buttonState == HIGH)
    {
      if (! waitingforbuttonrelease)
      {
        lastButtonPushMs = millis();
        DPRINT("Button Pushed, curSelectedGate= ");  DPRINTLN(curSelectedGate);
        gateservos.ledoff(curSelectedGate);
        curSelectedGate++;
        if (curSelectedGate == gateservos.num_gates) 
          curSelectedGate = 0;
        gateservos.ledon(curSelectedGate);
        waitingforbuttonrelease= true;
      }
    }
    
    if (hasbutton && !toolon && buttonState == LOW)
    {
      if (waitingforbuttonrelease)
      {
         waitingforbuttonrelease=false;
      }
    
      if (millis() - lastButtonPushMs > 1000)
      {    
          gateservos.ManuallyOpenGate(curSelectedGate);
      }
    }
  }
  
  if (metermode)
   delay(1);  // minimal delay while metering so we can collect as many samples as possible
  else 
   delay(50);
  if (!toolon)
  {    
     // User is picking which gate to open manually. this code 'debounces' the button so it only moves one at a time
     //
     if (curSelectedGate == gateservos.curopengate) 
        lastButtonPushMs = millis();
  }
}


