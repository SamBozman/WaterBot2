#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

// #include <AccelStepper.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <algorithm>
// #include <iostream> // for testing output in main
// #include <set>
// #include <string>

// #include "BluetoothSerial.h" // Bluetooth Serial -> works ONLY on ESP32
// #include "FS.h"

#define Debug 1 // set to 0 if Serial prints not needed

#if Debug == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) // defined to nothing
#define debugln(x) // defined to nothing
#endif

#define waterPin 2 // Water off-LOW on - HIGH

#define MotorInterfaceType 8
#define home 0 // All steppers homing position is 0

//* Hall Effect sensor pins
#define hHomePin 34
#define vHomePin 39
#define sHomePin 36

#define resetSlider 255 // reset slider poition & clear button backgrounds
//* The above also temporarily disables the steppers

// BluetoothSerial ESP_BT;

// AccelStepper Hstepper = AccelStepper(MotorInterfaceType, 26, 33, 25, 32);
// AccelStepper Vstepper = AccelStepper(MotorInterfaceType, 13, 14, 12, 27);
// AccelStepper Sstepper = AccelStepper(MotorInterfaceType, 5, 19, 18, 21);
// AccelStepper* StepPtr = &Hstepper; // init StepPtr;

// long H_MaxPos = 10000; // Horizontal Stepper Maximum travel position from home
// long V_MaxPos = 10000; // Vertical Stepper Maximum travel position from home
// long S_MaxPos = 10000; // Spray Stepper Maximum travel position from home
// long* MaxPtr = &H_MaxPos; // init MaxPtr

// #define FORMAT_LITTLEFS_IF_FAILED true
// char g_output[255]; // Serialized water target object
// String textIncoming = "";
// char path[25]; // path to saved files
