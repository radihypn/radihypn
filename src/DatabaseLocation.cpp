#include <DatabaseLocation.hpp>
#include <cstdlib>

std::string getDatabasePath() {
  std::string dbpath;
  if (const char *env_home = std::getenv("HOME")) {
    dbpath = env_home;
  } else {
    throw("$HOME not defined");
  }

  dbpath += "/.config/radihypn2.sqlite";

  return dbpath;
}

std::string getLegacyDatabasePath() {
  std::string dbpath;
  if (const char *env_home = std::getenv("HOME")) {
    dbpath = env_home;
  } else {
    throw("$HOME not defined");
  }

  dbpath += "/.config/radihypn.sqlite";

  return dbpath;
}
