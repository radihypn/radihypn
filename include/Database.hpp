#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <RadioStream.hpp>
#include <vector>
#include <set>
#include <string>

struct Database {
    SQLite::Database* conn;

    Database();
    ~Database();

    void insert(RadioStream r);
    void remove(RadioStream r);
    std::vector<RadioStream> listFavourites();
    std::vector<RadioStream> filterFavourites(std::vector<RadioStream>);
    std::vector<RadioStream> markFavourites(std::vector<RadioStream>);

private:
    void importLegacyDatabase();
    void create();
    std::set<std::string> favs;
};
