#include "files.h"
#include "globals.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mountLFS()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
        debugln("LittleFS Mount Failed");
        return;
    }
}