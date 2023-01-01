
#include "setup.h"

void loop()

{
    if (ESP_BT.available()) {
        int incoming = ESP_BT.read(); // Read byte from cell-phone
        processIncoming(incoming);
    }

    doStepLoop(StepPtr, MaxPtr);
}