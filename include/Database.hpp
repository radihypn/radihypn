#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <RadioStream.hpp>
#include <vector>

struct Database {
    SQLite::Database* conn;

    Database();
    ~Database();

    void insert(RadioStream r);
    void remove(RadioStream r);
    std::vector<RadioStream> listFavourites();

private:
    void importLegacyDatabase();
    void create();
};
