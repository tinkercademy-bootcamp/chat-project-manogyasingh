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
  sockaddr_in server_address_;
  int socket_fd_;

  static sockaddr_in create_server_address(const std::string &server_ip,
                                           int port);

  void connect_to_server();


  void send_and_receive_message(const std::string &message);
};

#endif