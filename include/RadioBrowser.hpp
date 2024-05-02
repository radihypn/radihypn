#pragma once
#include "RadioStream.hpp"
#include <string>
#include <RadioBrowser.hpp>
#include <Database.hpp>
#include <vector>
#include <string>

struct RadioBrowserApiEndpoint {
    std::string address;

    void setNextEndpoint();

    std::vector<RadioStream> search(std::string term);
    RadioBrowserApiEndpoint();
};
