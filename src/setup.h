#include "data_manager.h"
#include "files.h"
#include "globals.h"

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

    // Sample use of data manager
    WATER_DATA_MANAGER data_manager; // Create a Data Manager variable

    // Add some data
    WATER_DATA::target_data sample_data1 = { 0, 1, "foobar1", 1, 1, 1, 1, 1, 1, false };
    WATER_DATA::target_data sample_data2 = { 12, 50, "foobar2", 1, 1, 1, 1, 1, 1, false };
    WATER_DATA::target_data sample_data3 = { 5, 6, "foobar3", 1, 1, 1, 1, 1, 1, false };
    WATER_DATA::target_data sample_data4 = { 1, 50, "foobar4", 1, 1, 1, 1, 1, 1, false };

    // WATER_DATA::target_data sample_data5;
    // sample_data5.id = 15;

    data_manager.insert_data(sample_data1);
    data_manager.insert_data(sample_data2);
    data_manager.insert_data(sample_data3);
    data_manager.insert_data(sample_data4);

    // Print it out, delete one entry and print again
    data_manager.print_all_data();
    data_manager.delete_data(5);
    data_manager.print_all_data();
}
