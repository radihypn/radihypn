#include <DatabaseLocation.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout <<getLegacyDatabasePath() << std::endl;
    std::cout <<getDatabasePath() << std::endl;
}
