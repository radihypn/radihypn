#include <RadioStream.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

RadioStream RadioStream::fromJson(std::string jsonData) {
  json parsed = json::parse(jsonData);
  return RadioStream{.name = parsed["name"], .url = parsed["url_resolved"]};
}
