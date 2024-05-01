#include <RadioStream.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    RadioStream a {
        .name = "HelloStream", 
        .url = "example-url", 
        .shortdesc = "shortdesc"};
    
    std::cout << a.name <<
        "\n" << a.url <<
        "\n" << a.shortdesc <<
        "\n";

    return 0;
}