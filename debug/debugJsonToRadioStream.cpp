#include <RadioStream.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    auto a = RadioStream::fromJson(R"({
    "name": "HelloStream",
    "url_resolved": "example-url"
    })");
    
    std::cout << a.name <<
        "\n" << a.url <<
        "\n";

    return 0;
}
