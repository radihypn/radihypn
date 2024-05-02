#include <LegacyDatabaseImport.hpp>
#include <DatabaseLocation.hpp>

#include <sys/stat.h>
#include <string>

static bool fileExist(std::string filePath) {
    struct stat buffer;
    if (stat(filePath.c_str(), &buffer) == 0) {
        return true;
    } else {
        return false;
    }
}

bool databaseImportNeeded() {
    if (fileExist(getDatabasePath())) return false;
    if (!fileExist(getLegacyDatabasePath())) return false;
    return true;
}
