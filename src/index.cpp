
#include "globals.h"

void targetIndex()
{
    if (!isDir(LittleFS, "/INDEX", 1))
        createDir(LittleFS, "/INDEX");
}