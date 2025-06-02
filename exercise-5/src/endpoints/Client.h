#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <string>

class Client {
public:
  static sockaddr_in create_server_address(const std::string &server_ip, int port);
};

#endif