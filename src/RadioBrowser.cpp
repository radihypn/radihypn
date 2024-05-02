#include <RadioBrowser.hpp>
#include <MicroDNSClient.hpp>
#include <random>
#include <vector>
#include <string>

RadioBrowserApiEndpoint::RadioBrowserApiEndpoint() {
    setNextEndpoint();
}

void RadioBrowserApiEndpoint::setNextEndpoint() {
    // Create a random device
    std::random_device rd;

    // Create a random number generator
    std::mt19937 rng(rd());

    std::vector<std::string> addrs = MicroDNSClient::getAddressesForDomainName("all.api.radio-browser.info");
    // Create a distribution object
    std::uniform_int_distribution<int> dist(0, addrs.size() - 1);

    // Generate a random index
    int randomIndex = dist(rng);

    address = addrs[randomIndex];
}
