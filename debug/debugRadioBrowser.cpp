#include <RadioBrowser.hpp>
#include <curlpp/cURLpp.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    RadioBrowserApiEndpoint ep;
    curlpp::Cleanup cleaner;

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

    std::cout << "Searching 'relax'...\n";
    for (auto it: ep.search("relax")) {
        std::cout << "name: " << it.name <<
        "\n"  << "url: " << it.url << "\n\n";
    }

    return 0;
}
