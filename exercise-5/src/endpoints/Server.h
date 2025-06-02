#ifndef SERVER_H
#define SERVER_H

#include "../utils.h"
#include <netinet/in.h>

namespace tt::chat::server {
class Server {
public:
  static void set_socket_options(int sock, int opt);
  static int create_server_socket();
  static void bind_address_to_socket(int sock, sockaddr_in &address);
  static void listen_on_socket(int sock);
};
} // namespace tt::chat::server

#endif