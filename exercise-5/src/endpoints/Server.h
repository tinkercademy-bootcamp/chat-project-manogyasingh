#ifndef SERVER_H
#define SERVER_H

#include "../utils.h"
#include <netinet/in.h>

namespace tt::chat::server {
class Server {
public:
  Server(int port);
  ~Server();

private:
  sockaddr_in create_server_address();
  static int create_server_socket();
  static void set_socket_options(int sock, int opt);
  void bind_address_to_socket();
  void listen_on_socket();
  void handle_connections();
  static void handle_accept(int client_socket);

  int port_;
  sockaddr_in address_;
  int server_socket_fd_;
  static const int kBufferSize = 1024;
};
} // namespace tt::chat::server

#endif