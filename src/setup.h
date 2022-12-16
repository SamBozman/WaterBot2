

void setup()
{
    Serial.begin(115200);
    mountLFS(); //* Return message to app if failed?
    // ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface
    delay(5000); //! Temporary for testing
    // creat directorys for max positions and Target data
    // if (!isDir(LittleFS, "/MAX", 1))
    //     createDir(LittleFS, "/MAX");
    // if (!isDir(LittleFS, "/TARGETS", 1))
    //     createDir(LittleFS, "/TARGETS");

    // String s;

    // int numFiles = listFiles(LittleFS, "/MAX", 1, 0);
    // debug("NumFiles in MAX directory is ");
    // debugln(numFiles);

    // if (!numFiles == 3) {
    //     debug("NOT ALL MAXIMUM POSITIONS HAVE BEEN SET: ");
    // } else { // load all maximum positions

    //     s = "/MAX/M1";
    //     MaxPtr = &H_MaxPos;
    //     strcpy(path, s.c_str());
    //     loadMax(path, MaxPtr);

    //     s = "/MAX/M2";
    //     MaxPtr = &V_MaxPos;
    //     strcpy(path, s.c_str());
    //     loadMax(path, MaxPtr);

    //     s = "/MAX/M3";
    //     MaxPtr = &S_MaxPos;
    //     strcpy(path, s.c_str());
    //     loadMax(path, MaxPtr);
    // }

    // homeStepper(Hstepper, hHomePin);
    // homeStepper(Vstepper, vHomePin);
    // homeStepper(Sstepper, sHomePin);

    // numFiles = 0;
    // numFiles = listFiles(LittleFS, "/TARGETS", 1, 0);
    // debug("NumFiles before creating targets is ");
    // debugln(numFiles);

    // createTestFiles(2);
    // numFiles = 0;
    // numFiles = listFiles(LittleFS, "/TARGETS", 1, 0);
    // debug("NumFiles afeter creating targets is ");
    // debugln(numFiles);

    // removeTestFiles(2);
    // numFiles = 0;
    // numFiles = listFiles(LittleFS, "/TARGETS", 1, 0);
    // debug("NumFiles afeter deleting targets is ");
    // debugln(numFiles);
    // Shutoff water
    // TODO start a timer to shut down steppers after a specific time period.
}
