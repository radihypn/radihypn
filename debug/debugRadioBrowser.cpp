#include <RadioBrowser.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    RadioBrowserApiEndpoint ep;

    std::cout << "printing random endpoints\n";

    std::cout << ep.address << "\n";
    ep.setNextEndpoint();
    std::cout << ep.address << "\n";
    ep.setNextEndpoint();
    std::cout << ep.address << "\n";
    ep.setNextEndpoint();
    std::cout << ep.address << "\n";
    ep.setNextEndpoint();
    std::cout << ep.address << "\n";
    ep.setNextEndpoint();

    return 0;
}
