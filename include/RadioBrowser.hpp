#pragma once
#include <string>

struct RadioBrowserApiEndpoint {
    std::string address;

    void setNextEndpoint();

    RadioBrowserApiEndpoint();
};
