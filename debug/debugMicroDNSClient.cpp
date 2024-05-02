#include <vector>
#include <string>
#include <iostream>
#include <MicroDNSClient.hpp>

int main2(int argc, const char *argv[]) {
    std::vector<std::string> addrlist;

    for (int i = 0; i < argc; i++) {
        std::string hostname = argv[i];
        std::cout << "Hostname: " << hostname << "\n";
        try {
            addrlist = MicroDNSClient::getAddressesForDomainName(hostname);
        } catch (std::string err) {
            std::cout << "Resolving error: " << err << "\n";
            continue;
        }
        std::cout << "Number of results: " << addrlist.size() << "\n";
        for (int i = 0; i < addrlist.size(); i++) {
            std::cout << "ip: " << addrlist[i] << "\n";
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    const char* addrs[] = {"google.com", "lua.org"};
    return main2(2, addrs);
}
