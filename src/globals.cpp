#include "globals.h"

BluetoothSerial ESP_BT;
AccelStepper Hstepper = AccelStepper(MotorInterfaceType, 26, 33, 25, 32);
AccelStepper Vstepper = AccelStepper(MotorInterfaceType, 13, 14, 12, 27);
AccelStepper Sstepper = AccelStepper(MotorInterfaceType, 5, 19, 18, 21);
AccelStepper* StepPtr = &Hstepper; // init StepPtr;

long H_MaxPos = 10000; // Horizontal Stepper Maximum travel position from home
long V_MaxPos = 10000; // Vertical Stepper Maximum travel position from home
long S_MaxPos = 10000; // Spray Stepper Maximum travel position from home
long* MaxPtr = &H_MaxPos; // init MaxPtr

String textIncoming = "";
char g_output[255]; // Serialized water target object
char path[25]; // path to saved files
