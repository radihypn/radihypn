#include <Database.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    Database db;
    for (auto a: db.listFavourites()) {
        std::cout << "name: " << a.name <<
            "\n"  << "url: " << a.url << "\n\n";
    };
    return 0;
}
