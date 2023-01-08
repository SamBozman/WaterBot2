#include "globals.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void deleteAllTargets()
{
    int numFiles = getNumTargetFiles();
    if (numFiles == 0) {
        debugln("No saved TargetFiles exist!");
    } else {
        for (int i = 1; i < numFiles + 1; i++) { // Run through this code for each target file
            stochar(i);
            deleteFile(LittleFS, 
            path);
        }
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loadTargets()
{
    WATER_DATA_MANAGER data_manager; // Create a Data Manager variable
    int numFiles = getNumTargetFiles();
    if (numFiles == 0) {
        debugln("No saved TargetFiles exist!");
    } else {
        for (int i = 1; i < numFiles + 1; i++) { // Run through this code for each target file
            stochar(i);
            File file = LittleFS.open(path);
            if (!file || file.isDirectory()) {
                debugln("- failed to open MAX directory for reading");
                return;
            }

            debug("- reading from /TARGET direcory file: ");
            debugln(path);

            int x = 0;
            while (file.available()) {
                g_output[x] = file.read();
                x++;
            }
            g_output[x] = 0; // delimiter

            file.close();

            StaticJsonDocument<255> doc;
            DeserializationError err = deserializeJson(doc, g_output);
            if (err) {
                Serial.print(F("deserializeJson() of *MaxPtr failed with code "));
                Serial.println(err.f_str());
            } else {
                short int id = doc["id"]; // Computer created Target ID
                const char* name = doc["name"]; // Name of the Target Position
                int hp = doc["hp"]; // Horizontal position
                int vp = doc["vp"]; // Vertical Position
                int sp = doc["sp"]; // Spray position – a needle to break up the spray stream
                short int hf = doc["hf"]; // Horizontal fluctuation -small variations in the horizontal position over time
                short int vf = doc["vf"]; // Vertical fluctuation -small variations in the vertical position over time
                short int sf = doc["sf"]; // Spray fluctuation -small variations in the spray needle position over time
                short int rwt = doc["rwt"]; // Relative Watering Time (Certain plants will get more or less)
                bool water = doc[water]; // Do we want the water on or off while traversing to next target?

                String s = "T" + String(i); // Combine 2 strings
                int n = s.length(); // Get length of combined string
                strcpy(path, s.c_str()); // Convert it to a char array

                WATER_DATA::target_data path = { id, name, hp, hf, vp, vf, sp, sf, rwt, water };
                data_manager.insert_data(path);
            }
        }
        data_manager.print_all_data(); // Show all data_manager water Targets
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void saveTarget()
{

    textIncoming = ESP_BT.readStringUntil('\n');
    debug("Text incoming is ");
    debugln(textIncoming);

    short int id; // Computer created Target ID
    const char* name; // User defined Name of the Target Position
    int hp = Hstepper.currentPosition(); // Horizontal position
    int vp = Vstepper.currentPosition(); // Vertical Position
    int sp = Sstepper.currentPosition(); // Spray position – a needle to break up the spray stream
    short int hf; // Horizontal fluctuation -small variations in the horizontal position over time
    short int vf; // Vertical fluctuation -small variations in the vertical position over time
    short int sf; // Spray fluctuation -small variations in the spray needle position over time
    short int rwt; // Relative Watering Time (Certain plants will get more or less)
    bool water; // Do we want the water on or off while traversing to next target?

    int numFiles = getNumTargetFiles();
    id = numFiles++; // add 1 to the count of existing waterTargets

    debug("New ID for Target is ");
    debugln(numFiles);
    debug("id for new Target is ");
    debugln(id);

    StaticJsonDocument<255> doc; // deserialize data from WaterBot App
    DeserializationError err = deserializeJson(doc, textIncoming);
    if (err) {
        Serial.print(F("deserializeJson() of *MaxPtr failed with code "));
        Serial.println(err.f_str());
    } else {

        name = doc["Name"].as<const char*>();
        hf = doc["H_Flux"];
        vf = doc["V_Flux"];
        sf = doc["S_Flux"];
        rwt = doc["RWT"];
        water = doc["Water"];
    }

    makeWaterTarget(id, name, hp, vf, vp, vf, sp, sf, rwt, water);
    stochar(numFiles); // add int numFiles to 'TARGETS/T'  to create a file name (T1, T2... etc)
    writeFile(LittleFS, path, g_output);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int getNumTargetFiles()
{
    int numFiles = 0;
    numFiles = listFiles(LittleFS, "/TARGETS", 1, 0);
    debug("NumFiles in /TARGETS directory is ");
    debugln(numFiles);
    return numFiles;
}

// Check if /MAX directories have been created ++++++++++++++++++++++++++++
void configureMaximumPositions()
{
    String s;

    if (!isDir(LittleFS, "/MAX", 1))
        createDir(LittleFS, "/MAX");
    if (!isDir(LittleFS, "/TARGETS", 1))
        createDir(LittleFS, "/TARGETS");

    int numFiles = listFiles(LittleFS, "/MAX", 1, 0);
    debug("NumFiles in MAX directory is ");
    debugln(numFiles);

    if (!numFiles == 3) {
        debug("NOT ALL MAXIMUM POSITIONS HAVE BEEN SET: ");
    } else { // load all maximum positions

        s = "/MAX/M1";
        MaxPtr = &H_MaxPos;
        strcpy(path, s.c_str());
        loadMax(path, MaxPtr);

        s = "/MAX/M2";
        MaxPtr = &V_MaxPos;
        strcpy(path, s.c_str());
        loadMax(path, MaxPtr);

        s = "/MAX/M3";
        MaxPtr = &S_MaxPos;
        strcpy(path, s.c_str());
        loadMax(path, MaxPtr);
    }
}

// Loads Maximum position for current stepper pointer ++++++++++++++++++++++
void loadMax(char* path, long* MaxPtr)
{ // This function loads the maximum position for ONLY 1 stepper

    File file = LittleFS.open(path);
    if (!file || file.isDirectory()) {
        debugln("- failed to open MAX directory for reading");
        return;
    }

    debug("- reading from MAX direcory file: ");
    debugln(path);

    int x = 0;
    while (file.available()) {
        g_output[x] = file.read();
        x++;
    }
    g_output[x] = 0; // delimiter

    file.close();

    StaticJsonDocument<255> doc;
    DeserializationError err = deserializeJson(doc, g_output);
    if (err) {
        Serial.print(F("deserializeJson() of *MaxPtr failed with code "));
        Serial.println(err.f_str());
    } else {
        // MaxPtr dereferenced to current stepper Maximum position global variable
        //  long ?_MaxPos (? = H,V or S) doc key ["MSP"] contains a long position
        *MaxPtr = doc["MSP"];
    }
}

// Set Maximum position for current Stepper and writes it to file+++++++++++
void setMax(AccelStepper* Stepper)
{
    int currentStepper = 0;

    if (Stepper == &Hstepper) {
        currentStepper = 1;
        H_MaxPos = Stepper->currentPosition();
        debugln("H_MaxPos set to current position!");
    } else if (Stepper == &Vstepper) {
        currentStepper = 2;
        V_MaxPos = Stepper->currentPosition();
        debugln("V_MaxPos set to current position!");
    } else if (Stepper == &Sstepper) {
        currentStepper = 3;
        S_MaxPos = Stepper->currentPosition();
        debugln("S_MaxPos set to current position!");
    }

    if (currentStepper > 0 && Stepper->currentPosition() > 0) {
        String s = "/MAX/M" + String(currentStepper); // Combine 2 strings
        strcpy(path, s.c_str()); // Convert it to a char array

        StaticJsonDocument<256> doc; // Declare StaticJsonDocument doc
        doc["MSP"] = Stepper->currentPosition(); //''Max Stepper Position'
        serializeJson(doc, g_output);
        writeFile(LittleFS, path, g_output);
    }
}

// Resets Maximum postion of current stepper to 10,000++++++++++++++++++++++
void resetMax(AccelStepper* Stepper)
{
    if (Stepper == &Hstepper) {
        H_MaxPos = 10000;
        debugln("Reset Horz stepper to 10000");
    } else if (Stepper == &Vstepper) {
        V_MaxPos = 10000;
        debugln("Reset Vert stepper to 10000");
    } else if (Stepper == &Sstepper) {
        S_MaxPos = 10000;
        debugln("Reset Spray stepper to 10000");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void makeWaterTarget(short int id, String name, int hp, short int hf, int vp, short int vf, int sp, short int sf, short int rwt, bool water)
{
    const int capacity = 256; // Buffer for StaticJsonDocument doc
    StaticJsonDocument<capacity> doc; // Declare StaticJsonDocument doc

    doc["id"] = id; //(Int) ID  for watering target
    doc["name"] = name;
    doc["hp"] = hp; // Horizontal steps from 'Home'
    doc["hf"] = hf; // Horizontal fluctuation for coverage
    doc["vp"] = vp; // Vertical steps from 'Home'
    doc["vf"] = vf; // Vertical fluctuation for coverage
    doc["sp"] = sp; // Spray steps for diffused spray
    doc["sf"] = sf; // Spray fluctuation for diffused spray
    doc["rwt"] = rwt; // Relative watering time (1-5)
    doc["water"] = water; // Turn water off ? (or leave on while moving to next spot)
    // Serialize the Json object and put into variable (output)
    serializeJson(doc, g_output);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mountLFS()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
        debugln("LittleFS Mount Failed");
        return;
    }
}

// Returns # of files in directory++++++++++++++++++++++++++++++++++++++++++
int listFiles(fs::FS& fs, const char* dirname, uint8_t levels, int numFiles)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        debugln("- failed to open directory");
        return 0;
    }
    if (!root.isDirectory()) {
        debugln(" - not a directory");
        return 0;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            debug("  DIR : ");
            debugln(file.name());
            if (levels) {
                debug("Levels = : ");
                debugln(levels);
                numFiles = listFiles(fs, file.path(), levels - 1, numFiles);
            }
        } else {
            debug("  List-FILE: ");
            debug(file.name());
            debug("\tSIZE: ");
            debugln(file.size());
            numFiles++;
        }
        file = root.openNextFile();
    }

    return numFiles;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void listDir(fs::FS& fs, const char* dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        debugln("- failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        debugln(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            debug("  DIR : ");
            debugln(file.name());
            if (levels) {
                listDir(fs, file.path(), levels - 1);
            }
        } else {
            debug("  FILE: ");
            debug(file.name());
            debug("\tSIZE: ");
            debugln(file.size());
        }
        file = root.openNextFile();
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool isDir(fs::FS& fs, const char* dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        debugln("- failed to open directory");
        return false;
    }
    if (!root.isDirectory()) {
        debugln(" - not a directory");
        return false;
    } else {
        return true;
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void createDir(fs::FS& fs, const char* path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path)) {
        debugln("Dir created");
    } else {
        debugln("mkdir failed");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void removeDir(fs::FS& fs, const char* path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path)) {
        debugln("Dir removed");
    } else {
        debugln("rmdir failed");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void readFile(fs::FS& fs, const char* path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory()) {
        debugln("- failed to open file for reading");
        return;
    }

    debug("- read from file: ");
    while (file.available()) {
        Serial.write(file.read());
    }
    debugln("");
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void writeFile(fs::FS& fs, const char* path, const char* message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        debugln("- failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        debugln("- file written");
    } else {
        debugln("- write failed");
    }
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void appendFile(fs::FS& fs, const char* path, const char* message)
{
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        debugln("- failed to open file for appending");
        return;
    }
    if (file.print(message)) {
        debugln("- message appended");
    } else {
        debugln("- append failed");
    }
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void renameFile(fs::FS& fs, const char* path1, const char* path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        debugln("- file renamed");
    } else {
        debugln("- rename failed");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void deleteFile(fs::FS& fs, const char* path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path)) {
        debugln("- file deleted");
    } else {
        debugln("- delete failed");
    }
}

// SPIFFS-like write and delete file, better use #define
// CONFIG_LITTLEFS_SPIFFS_COMPAT 1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// void writeFile2(fs::FS& fs, const char* path, const char* message)
// {
//     if (!fs.exists(path)) {
//         if (strchr(path, '/')) {
//             Serial.printf("Create missing folders of: %s\r\n", path);
//             char* pathStr = strdup(path);
//             if (pathStr) {
//                 char* ptr = strchr(pathStr, '/');
//                 while (ptr) {
//                     *ptr = 0;
//                     fs.mkdir(pathStr);
//                     *ptr = '/';
//                     ptr = strchr(ptr + 1, '/');
//                 }
//             }
//             free(pathStr);
//         }
//     }

//     Serial.printf("Writing file to: %s\r\n", path);
//     File file = fs.open(path, FILE_WRITE);
//     if (!file) {
//         debugln("- failed to open file for writing");
//         return;
//     }
//     if (file.print(message)) {
//         debugln("- file written");
//     } else {
//         debugln("- write failed");
//     }
//     file.close();
// }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// void deleteFile2(fs::FS& fs, const char* path)
// {
//     Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

//     if (fs.remove(path)) {
//         debugln("- file deleted");
//     } else {
//         debugln("- delete failed");
//     }

//     char* pathStr = strdup(path);
//     if (pathStr) {
//         char* ptr = strrchr(pathStr, '/');
//         if (ptr) {
//             Serial.printf("Removing all empty folders on path: %s\r\n", path);
//         }
//         while (ptr) {
//             *ptr = 0;
//             fs.rmdir(pathStr);
//             ptr = strrchr(pathStr, '/');
//         }
//         free(pathStr);
//     }
// }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// void testFileIO(fs::FS& fs, const char* path)
// {
//     Serial.printf("Testing file I/O with %s\r\n", path);

//     static uint8_t buf[512];
//     size_t len = 0;
//     File file = fs.open(path, FILE_WRITE);
//     if (!file) {
//         debugln("- failed to open file for writing");
//         return;
//     }

//     size_t i;
//     debug("- writing");
//     uint32_t start = millis();
//     for (i = 0; i < 2048; i++) {
//         if ((i & 0x001F) == 0x001F) {
//             debug(".");
//         }
//         file.write(buf, 512);
//     }
//     debugln("");
//     uint32_t end = millis() - start;
//     Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
//     file.close();

//     file = fs.open(path);
//     start = millis();
//     end = start;
//     i = 0;
//     if (file && !file.isDirectory()) {
//         len = file.size();
//         size_t flen = len;
//         start = millis();
//         debug("- reading");
//         while (len) {
//             size_t toRead = len;
//             if (toRead > 512) {
//                 toRead = 512;
//             }
//             file.read(buf, toRead);
//             if ((i++ & 0x001F) == 0x001F) {
//                 debug(".");
//             }
//             len -= toRead;
//         }
//         debugln("");
//         end = millis() - start;
//         Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
//         file.close();
//     } else {
//         debugln("- failed to open file for reading");
//     }
// }
