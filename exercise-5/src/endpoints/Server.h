#ifndef SERVER_H
#define SERVER_H

#include "../utils.h"
#include <netinet/in.h>

namespace tt::chat::server {
class Server {
public:
  Server(int port);
  ~Server();
  int port_;
  static void set_socket_options(int sock, int opt);
  static int create_server_socket();
  static void bind_address_to_socket(int sock, sockaddr_in &address);
  static void listen_on_socket(int sock);
  static void handle_accept(int sock);
  static sockaddr_in create_server_address(int port);
  static void handle_connections(int sock, sockaddr_in &address);
};
} // namespace tt::chat::server

#endif