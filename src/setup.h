
#include "globals.h"

void setup()
{
    Serial.begin(115200);
    mountLFS(); //* Return message to app if failed?
    ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface
    delay(5000); //! Temporary for testing

    // homeStepper(Hstepper, hHomePin);
    // homeStepper(Vstepper, vHomePin);
    // homeStepper(Sstepper, sHomePin);

    int numFiles = 0;
    numFiles = listFiles(LittleFS, "/TARGETS", 1, 0);
    debug("NumFiles before creating targets is ");
    debugln(numFiles);

    createTestFiles(2);
    delay(100); //! Temporary for testing

    // View result of creating test targets
    numFiles = 0;
    numFiles = listFiles(LittleFS, "/TARGETS", 1, 0);
    debug("NumFiles afeter creating targets is ");
    debugln(numFiles);

    // removeTestFiles(2);

    // Shutoff water
    // TODO start a timer to shut down steppers after a specific time period.
}
