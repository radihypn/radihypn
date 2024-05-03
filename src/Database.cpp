#include <Database.hpp>
#include <DatabaseLocation.hpp>
#include <RadioStream.hpp>

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Statement.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static bool fileExist(std::string filePath) {
  struct stat buffer;
  if (stat(filePath.c_str(), &buffer) == 0) {
    return true;
  } else {
    return false;
  }
}

Database::Database() : conn(nullptr) {
  if (fileExist(getDatabasePath())) {
    conn = new SQLite::Database(getDatabasePath(), SQLite::OPEN_READWRITE);
  } else {
    create();
  }

  for (auto fav : listFavourites()) {
    favs.insert(fav.url);
  }
}

Database::~Database() { delete conn; }

void Database::create() {
  conn = new SQLite::Database(getDatabasePath(),
                              SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

  conn->exec("create table IF NOT EXISTS favourites ("
             "name text NOT NULL,"
             "url text NOT NULL"
             ")");

  if (fileExist(getLegacyDatabasePath())) {
    importLegacyDatabase();
  }
}

std::vector<RadioStream> Database::listFavourites() {
  SQLite::Statement st(*conn, "select name, url from favourites");

  std::vector<RadioStream> r;

  while (st.executeStep()) {
    try {
      std::string name = st.getColumn(0);
      std::string url = st.getColumn(1);
      r.push_back(RadioStream{.name = name, .url = url, .favourite = true});
    } catch (...) {
    }
  }

  return r;
}

std::vector<RadioStream>
Database::filterFavourites(std::vector<RadioStream> tofilter) {
  std::vector<RadioStream> r;

  for (int i = 0; i < tofilter.size(); i++) {
    RadioStream a = tofilter[i];

    if (favs.find(a.url) != favs.end()) {
      a.favourite = true;
      r.push_back(a);
    }
  }

  return r;
}

std::vector<RadioStream>
Database::markFavourites(std::vector<RadioStream> tofilter) {
  for (int i = 0; i < tofilter.size(); i++) {
    RadioStream a = tofilter[i];

    if (favs.find(a.url) != favs.end()) {
      tofilter[i].favourite = true;
    }
  }

  return tofilter;
}

void Database::insert(RadioStream r) {
  SQLite::Statement query(*conn,
                          "INSERT INTO favourites(name, url) values (?, ?)");
  query.bind(1, r.name);
  query.bind(2, r.url);

  query.exec();

  favs.insert(r.url);
}

void Database::remove(RadioStream r) {
  SQLite::Statement query(*conn, "DELETE FROM favourites where url=?");
  query.bind(1, r.url);
  query.exec();

  favs.erase(r.url);
}

void Database::importLegacyDatabase() {
  SQLite::Database legacyConn(getLegacyDatabasePath(), SQLite::OPEN_READONLY);
  SQLite::Statement st(legacyConn, "select * from favourites");

  while (st.executeStep()) {
    try {
      std::string jsonText = st.getColumn(0);
      // std::cout << jsonText;
      json j = json::parse(jsonText);
      insert(RadioStream{
          .name = j["name"], .url = j["url_resolved"], .favourite = true});
    } catch (...) {
    }
  }
}
