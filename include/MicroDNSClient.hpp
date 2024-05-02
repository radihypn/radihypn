/**
\file
\brief header-only library for resolving dns
names.

To build with test client, use
```
g++ -DMICRODNSCLIENT_MAIN_WANTED MicroDNSClient.cpp -o dnsquery
```
and run it like that
```
./dnsquery google.com lua.org
```

To avoid building implementation, use
```
g++ -DMICRODNSCLIENT_DEFINITION_DISABLED MicroDNSClient.cpp
```

Of course you can build just the dynamic library:
```
g++ -fPIC -shared -o libMicroDNSClient.so MicroDNSClient.cpp
```
*/

#ifndef MICRODNSCLIENT_H
#define MICRODNSCLIENT_H

#include <string>
#include <vector>

/**
It can only get addresses for domain names.
Use the only static method.
*/
struct MicroDNSClient {

  /**
  simply get list of ipv6 and ipv4 ips as strings
  for domain name like example.com
  \param[in] hostname hostname to get ip list for
  \return list of ip addresses as vector<string>
  **/
  static std::vector<std::string>
  getAddressesForDomainName(std::string hostname);
};

#endif // MICRODNSCLIENT_H
