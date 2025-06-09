#ifndef XTC_CLIENT_H
#define XTC_CLIENT_H

#include <netinet/in.h>

#include <string>

#include "../common/error_handling/error_handling.h"
#include "../common/net/socket_helper.h"

namespace xtc::client {
class Client {
 public:
  Client(std::string server_address, int port);
  ~Client();
  int socket_;
  void send_message(std::string payload);
  bool check_messages();

 private:
  static constexpr int kBufferSize = 1024;
  void connect_to_server(sockaddr_in server_address);
  static sockaddr_in create_server_address(const std::string &server_ip,
                                           int port);
};
}  // namespace xtc::client

#endif