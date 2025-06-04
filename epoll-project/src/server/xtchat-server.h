#ifndef XTCHAT_SERVER_H
#define XTCHAT_SERVER_H

#include <fcntl.h>
#include <netinet/in.h>

#include <string>
#include <unordered_map>

#include "../common/error_handling/error_handling.h"
#include "../common/net/socket_helper.h"

namespace xtc::server {

class ClientDataOnServer {
  std::string username_;
  std::string client_socket_fd_;
  std::string buffer_;
};

class Server {
 public:
  Server(int port);
  ~Server();
  void start();

 private:
  int epoll_fd_;
  int server_socket_fd_;
  sockaddr_in server_address_;
  std::unordered_map<int, ClientDataOnServer> clients_dict_;
  void set_socket_options(int sock_fd, int opt);

  static const int kMaxEvents = 64;
  static constexpr int kBufferSize = 1024;
};
}  // namespace xtc::server

#endif  // XTCHAT_SERVER_H
