#ifndef CLIENT_H
#define CLIENT_H

#include "../utils.h"
#include "../net/chat-sockets.h"
#include <netinet/in.h>
#include <string>

class Client {
public:
  Client (std::string server_address, int port);
  ~Client();

  static sockaddr_in create_server_address(const std::string &server_ip,
                                           int port);

  static void connect_to_server(int sock, sockaddr_in &server_address);


  static void send_and_receive_message(int sock, const std::string &message);
};

#endif