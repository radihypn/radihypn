#include <iostream>
#include <string>
#include <cstdlib>

std::string radihypn_get_icon_path() {
    const char* homeDir = std::getenv("HOME");
    std::string path = std::string(homeDir) + "/.local/share/icons/radihypn.png";
    return path;
}
