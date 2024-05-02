#pragma once
#include <string>

struct RadioStream {
    std::string name;
    std::string url;
    bool favourite = false;

    static RadioStream fromJson(std::string json);
};
