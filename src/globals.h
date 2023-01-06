#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <ArduinoLog.h>

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <Array.h>
#include <LittleFS.h>
#include <Streaming.h>
#include <algorithm>
#include <iostream> // for testing output in main
#include <set>
#include <string>

#include "BluetoothSerial.h" // Bluetooth Serial -> works ONLY on ESP32
#include "FS.h"

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

#define FORMAT_LITTLEFS_IF_FAILED true
//* Hall Effect sensor pins
#define hHomePin 34
#define vHomePin 39
#define sHomePin 36

#define resetSlider 255 // reset slider poition & clear button backgrounds
//* The above also temporarily disables the steppers

extern BluetoothSerial ESP_BT;
extern AccelStepper Hstepper;
extern AccelStepper Vstepper;
extern AccelStepper Sstepper;
extern AccelStepper* StepPtr; // init StepPtr;

extern long H_MaxPos; // Horizontal Stepper Maximum travel position from home
extern long V_MaxPos; // Vertical Stepper Maximum travel position from home
extern long S_MaxPos; // Spray Stepper Maximum travel position from home
extern long* MaxPtr; // init MaxPtr

extern String textIncoming;
extern char g_output[255]; // Serialized water target object
extern char path[25]; // path to saved files

#define arraySize 5 //! Set array size to 100 for production?
typedef Array<int, arraySize> Elements; // defintion of array

// All Function Declatations go here
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void stochar(int x); // Temporary
void createTestFiles(int n); // Temporary
void removeTestFiles(int n); // Temporary

void loadTargets();
void saveTarget();
void configureMaximumPositions();
void loadMax(char* path, long* Maxptr);
void setMax(AccelStepper* Stepper);
void resetMax(AccelStepper* Stepper);

void homeStepper(AccelStepper& Stepper, int homePin);
void processIncoming(int incoming);
void processStepper(AccelStepper* Stepper, int incoming);
void doStepLoop(AccelStepper* Stepper, long* MaxPos);

void mountLFS();
void makeWaterTarget(int id, String name, long hs, int hf, long vs, int vf, long ss, int sf, int rwt, bool water);
void listDir(fs::FS& fs, const char* dirname, uint8_t levels);
bool isDir(fs::FS& fs, const char* dirname, uint8_t levels);

int listFiles(fs::FS& fs, const char* dirname, uint8_t levels, int numFiles);
void createDir(fs::FS& fs, const char* path);
void removeDir(fs::FS& fs, const char* path);
void readFile(fs::FS& fs, const char* path);
void writeFile(fs::FS& fs, const char* path, const char* message);
void appendFile(fs::FS& fs, const char* path, const char* message);
void renameFile(fs::FS& fs, const char* path1, const char* path2);
void deleteFile(fs::FS& fs, const char* path);
// SPIFFS-like write and delete file,
// better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

// void writeFile2(fs::FS& fs, const char* path, const char* message);
// void deleteFile2(fs::FS& fs, const char* path);
// void testFileIO(fs::FS& fs, const char* path);
void testDataManager();
class WATER_DATA {

public:
    struct target_data {
        short int id; // Computer created Target ID
        short int hp; // Horizontal position
        String name; // User created Water Target 'Name'
        short int vp; // Vertical Position
        short int sp; // Spray position – a needle to break up the spray stream
        short int hf; // Horizontal fluctuation -small variations in the horizontal position over time
        short int vf; // Vertical fluctuation -small variations in the vertical position over time
        short int sf; // Spray fluctuation -small variations in the spray needle position over time
        short int rwt; // Relative Watering Time (Certain plants will get more or less)
        bool water; // Do we want the water on or off while traversing to next target?

        bool operator<(const target_data& other) const
        {
            // Sort on hp ID, if they are equal use the id to sort
            if (hp < other.hp)
                return true;
            else if (hp > other.hp)
                return false;
            else
                return id < other.id;
        }
    } s_target_data;
};

class WATER_DATA_MANAGER {
public:
    // Define a sorted set of data elements
    typedef std::set<WATER_DATA::target_data> WATER_DATA_SET;
    WATER_DATA_SET water_data_set; // Create a Data set variable

    // Provide insert and delete access to it
    void insert_data(WATER_DATA::target_data& data); // Adds a new data entry
    void delete_data(int id); // Removes the entry by ID if it exists
    void print_all_data(void); // prints the entire data structure

private:
};

#endif