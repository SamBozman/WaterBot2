#include "globals.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void runWaterTargets()
{

    // Timed events
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void homeStepper(AccelStepper& Stepper, int homePin)
{
    int move_finished = 1; // Used to check if move is completed
    long initial_homing = -1; // Used to Home Stepper at startup
    pinMode(homePin, INPUT_PULLUP);

    delay(5);

    //  Set Max Speed and Acceleration of each Steppers at startup for homing
    Stepper.setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper

    // Start Homing procedure of Stepper Motor at startup

    while (digitalRead(homePin)) // Do WHILE switch not activated (NOT true)
    {
        // Make the Stepper move CCW
        // until the switch is activated
        Stepper.moveTo(initial_homing); // Set the position to move to
        initial_homing--; // Decrease by 1 for next move if needed
        Stepper.run(); // Non Blocking - move to position
    }

    Stepper.setCurrentPosition(0); // Set the current position as zero for now
    Stepper.setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper

    initial_homing = 1;
    while (!digitalRead(homePin)) {
        // Make the Stepper move CW until the
        // until the switch is deactivated
        Stepper.moveTo(initial_homing);
        Stepper.run(); // Non Blocking - move to position
        initial_homing++;
    }
    Stepper.setCurrentPosition(0);
    Serial.println("Homing Completed");
    Serial.println("");

    Stepper.setMaxSpeed(200.0); // Set Max Speed of Stepper (Faster for regular movements)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processIncoming(int incoming)
{
    unsigned char textBlock2[] = "Goodbye to everyone who hates me \n";
    unsigned char deleteAll[] = "All saved water targets have been deleted! \n";
    switch (incoming) {

    case 201:
        StepPtr = &Hstepper; // Hstepper
        MaxPtr = &H_MaxPos;
        ESP_BT.write(201); // Set app to use Hstepper
        debugln("Sending 201 back to app");
        debugln("Set Hstepper to current Stepper Motor");
        ESP_BT.write(resetSlider);
        break;
    case 202:
        StepPtr = &Vstepper; // Vstepper
        MaxPtr = &V_MaxPos;
        ESP_BT.write(202); // Set app to use Vstepper
        debugln("Sending 202 back to app");
        debugln("Set Vstepper to current Stepper Motor");
        ESP_BT.write(resetSlider);
        break;
    case 203:
        StepPtr = &Sstepper; // Sstepper
        MaxPtr = &S_MaxPos;
        ESP_BT.write(203); // Set app to use Sstepper
        debugln("Sending 203 back to app");
        debugln("Set Spray to current Stepper Motor");
        ESP_BT.write(resetSlider);
        break;
    case 204: //! Will eventually be used to control water on/off
        ESP_BT.write(204);
        debugln("Sending 204 back to app");
        break;
    case 205: // SetMax for current stepper motor
        setMax(StepPtr);
        ESP_BT.write(205);
        debugln("Sending 205 back to app");
        debugln("SetMax for current stepper motor");
        break;
    case 206: // RESET MAX to 10,000 for current stepper motor
        resetMax(StepPtr);
        ESP_BT.write(206);
        debugln("Sending 206 back to app");
        debugln("RESET MAX to 10,000 for current stepper motor");
        break;
    case 207: // Get Json from WaterBot App
        saveTarget();
        ESP_BT.write(207);
        debugln("Sending 207 back to app");
        debugln("Process Json from app and save new target");
        break;
    case 208: // Confirmation we are going to add a new target
        ESP_BT.write(208);
        debugln("Sending 208 back to app");
        debugln("Switching to add/edit screen to get ready to add a new Target");
        break;
    case 209: // Reboot ESP32
        ESP_BT.write(209);
        debugln("Sending 209 back to app");
        debugln("Rebooting ESP-32");
        ESP.restart();
        break;
    case 210: // Delete all Targets;
        debugln("Deleting all saved Targets");
        ESP_BT.write(deleteAll, sizeof(deleteAll));
        deleteAllTargets();
        break;
    case 254: //! Testing blob text to WaterBot app
        // ESP_BT.write(resetSlider); // reset slider poition to 100 (center) & clear buttons
        debugln("Trying to send unsigned block to text to app! ");

        loadTarget(1);
        ESP_BT.write((unsigned char*)&g_output, sizeof(g_output));

        break;
    case 255: //! disableOutputs for testing
        ESP_BT.write(resetSlider); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(255);
        debugln("Sending 255 back to app");
        Hstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Vstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Sstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        break;
    default:
        processStepper(StepPtr, incoming);
        break;
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processStepper(AccelStepper* Stepper, int incoming)
{
    int stepperSpeed = 0;
    Stepper->setMaxSpeed(300);
    Stepper->setSpeed(stepperSpeed);
    incoming = incoming - 100;

    if (incoming >= -5 && incoming <= 5) {
        stepperSpeed = 0;
        ESP_BT.write(stepperSpeed);
    } else if (incoming >= -100 && incoming <= 100) {
        stepperSpeed = incoming;
        ESP_BT.write(abs(stepperSpeed)); // abs changes negative values to positive
    } else {
        stepperSpeed = 0;
        ESP_BT.write(stepperSpeed);
    }
    Stepper->setSpeed(stepperSpeed);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void doStepLoop(AccelStepper* Stepper, long* MaxPos)
{

    if (Stepper->currentPosition() < -10) { // If Stepper is < than HOME by 10 then ...
        Stepper->setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
        Stepper->setAcceleration(50.0); // Set Acceleration of Stepper
        Stepper->moveTo(home); // Set Target potion to Home position
        Stepper->runToPosition(); // Run to HOME position (BLOCKING)
        ESP_BT.write(resetSlider); // Reset slider to center position

    } else if (Stepper->currentPosition() > (*MaxPos + 10)) { // If stepper is > MAX position by 10 then ...
        Stepper->setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
        Stepper->setAcceleration(50.0); // Set Acceleration of Stepper
        Stepper->moveTo(*MaxPos); // Set Target potion to MAX position
        Stepper->runToPosition(); // Run to MAX position (BLOCKING)
        ESP_BT.write(resetSlider); // Reset slider to center position
    } else
        Stepper->runSpeed();
}
