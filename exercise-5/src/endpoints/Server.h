#ifndef SERVER_H
#define SERVER_H

#include "../utils.h"
#include <netinet/in.h>

namespace tt::chat::server {
class Server {
public:
  Server(int port);
  ~Server();
  static const int kBufferSize = 1024;
  int port_;
  int server_socket_fd_;
  sockaddr_in address_;
  static void set_socket_options(int sock, int opt);
  static int create_server_socket();
  void listen_on_socket();
  static void handle_accept(int client_socket);
  
  private:
  sockaddr_in create_server_address();
  void bind_address_to_socket();
  void handle_connections();
};
} // namespace tt::chat::server

#endif