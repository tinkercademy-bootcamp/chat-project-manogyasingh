#ifndef XTC_SERVER_H
#define XTC_SERVER_H

#include <fcntl.h>
#include <netinet/in.h>

#include <string>
#include <unordered_map>

#include "../common/error_handling/error_handling.h"
#include "../common/net/socket_helper.h"

namespace xtc::server {

struct ClientData{
  std::string username_;
  std::string client_fd_;
  std::string buffer_;
};

class Server {
 public:
  Server(int port);
  ~Server();
  void run();
  
  private:
  int port_;
  int epoll_fd_;
  int server_socket_fd_;
  sockaddr_in server_address_;
  
  void opt_bind_listen();
  void setup_epoll();
  void add_to_epoll(int sock, uint32_t events);
  void remove_from_epoll(int sock);
  void set_non_blocking(int sock);

  static const int kMaxEvents = 64;
  static constexpr int kBufferSize = 1024;

  void handle_new_connection();
  void handle_client_data(int client_sock);
};
}  // namespace xtc::server

#endif  // XTCHAT_SERVER_H
