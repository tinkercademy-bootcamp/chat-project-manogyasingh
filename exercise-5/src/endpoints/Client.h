#ifndef CLIENT_H
#define CLIENT_H

#include "../utils.h"
#include <netinet/in.h>
#include <string>

class Client {
public:
  static sockaddr_in create_server_address(const std::string &server_ip,
                                           int port);

  static void connect_to_server(int sock, sockaddr_in &server_address);
};

#endif