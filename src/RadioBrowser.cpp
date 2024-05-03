#include <MicroDNSClient.hpp>
#include <RadioBrowser.hpp>
#include <random>
#include <string>
#include <vector>

#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

RadioBrowserApiEndpoint::RadioBrowserApiEndpoint() { setNextEndpoint(); }

void RadioBrowserApiEndpoint::setNextEndpoint() {
  // Create a random device
  std::random_device rd;

  // Create a random number generator
  std::mt19937 rng(rd());

  std::vector<std::string> addrs =
      MicroDNSClient::getAddressesForDomainName("all.api.radio-browser.info");
  // Create a distribution object
  std::uniform_int_distribution<int> dist(0, addrs.size() - 1);

  // Generate a random index
  int randomIndex = dist(rng);

  address = addrs[randomIndex];
}

std::vector<RadioStream> RadioBrowserApiEndpoint::search(std::string term) {
  std::vector<RadioStream> r;

  curlpp::Easy request;
  std::string url = "http://" + address + "/json/stations/search?" +
                    "name=" + curlpp::escape(term) + "&hide_broken=true";
  request.setOpt(curlpp::options::Url(url));
  request.setOpt(curlpp::options::FollowLocation(true));

  std::stringstream buff;
  buff << request;
  json parsed;
  buff >> parsed;

  for (json::iterator it = parsed.begin(); it != parsed.end(); ++it) {
    auto item = *it;
    r.push_back(RadioStream{.name = item["name"], .url = item["url_resolved"]});
  }

  return r;
}
