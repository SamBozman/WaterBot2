#include "globals.h"

// function to make a char path from a string
void stochar(int x)
{
    String s = "/TARGETS/T" + String(x); // Combine 2 strings
    int n = s.length(); // Get length of combined string
    strcpy(path, s.c_str()); // Convert it to a char array
}

// Create random water Target json files and write them to file system
void createTestFiles(int n)
{
    int numFiles = 0;
    for (int x = 1; x <= n; x++) {
        makeWaterTarget(x, "Test", random(4000), random(4000), random(4000), random(4000), random(4000), random(4000), 3, true);
        // makeWaterTarget(x, "Test", random(4000), random(4000), random(4000), random(4000), random(4000),
        // random(4000), random(4000), false);
        stochar(x); // add int x to 'TARGETS/T' to it to create a file name (T1, T2... etc)
        writeFile(LittleFS, path, g_output);
    }
}

// Remove random water Target json files from file system
void removeTestFiles(int n)
{
    int numFiles = 0;
    for (int x = 1; x <= n; x++) {
        stochar(x);
        deleteFile(LittleFS, path);
    }
}
//++++++++++++++++DATA MANAGER+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void testDataManager()
{
    short int id = 23;
    short int hp = 856;
    String name = "Angel Asian Tree";
    short int vp = 234;
    short int sp = 523;
    short int hf = 5;
    short int vf = 5;
    short int sf = 5;
    short int rwt = 3;
    bool water = true;

    // Sample use of data manager
    WATER_DATA_MANAGER data_manager; // Create a Data Manager variable

    // Add some data
    WATER_DATA::target_data sample_data1 = {1, "Test name 1", 100, 5, 350, 4, 125, 2, 5, false};
    WATER_DATA::target_data sample_data2 = {1, "Test name 2", 320, 5, 350, 4, 125, 2, 5, false};
    WATER_DATA::target_data sample_data3 = {1, "Test name 3", 200, 5, 350, 4, 125, 2, 5, false};
    WATER_DATA::target_data sample_data4 = { 1, "Test name 4", 400, 5, 350, 4, 125, 2, 5, false};
    WATER_DATA::target_data sample_data6 = { id, name, hp, hf, vp, vf, sp, sf, rwt, water };
    //  WATER_DATA::target_data sample_data5;
    //  sample_data5.id = 15;

    data_manager.insert_data(sample_data1);
    data_manager.insert_data(sample_data2);
    data_manager.insert_data(sample_data3);
    data_manager.insert_data(sample_data4);
    data_manager.insert_data(sample_data6);
    //  Print it out, delete one entry and print again
    data_manager.print_all_data();
    data_manager.delete_data(5);
    data_manager.print_all_data();
}