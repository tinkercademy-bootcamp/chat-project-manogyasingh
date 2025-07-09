#ifndef CLIENT_H
#define CLIENT_H

#include "../net/chat-sockets.h"
#include "../utils.h"
#include <string>

namespace tt::chat::client {
class Client {
public:
  Client(std::string server_address, int port);
  ~Client();
  void send_and_receive_message(const std::string &message);

private:
  sockaddr_in server_address_;
  int socket_fd_;
  void connect_to_server();
  static sockaddr_in create_server_address(const std::string &server_ip,
                                           int port);
};
} // namespace tt::chat::client

#endif