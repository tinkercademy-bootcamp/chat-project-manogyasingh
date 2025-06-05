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
  std::string send_message(std::string target_username, std::string message);

 private:
  int socket_;
  std::string username_;
  sockaddr_in server_address_;
  void set_username();
  void connect_to_server();
  static sockaddr_in create_server_address(const std::string &server_ip,
                                           int port);
};
}  // namespace xtc::client

#endif