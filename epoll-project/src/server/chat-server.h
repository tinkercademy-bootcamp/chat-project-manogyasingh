#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <fcntl.h>
#include <netinet/in.h>
#include "../utils.h"

namespace tt::chat::server {

class Server {
public:
  Server(int port);
  ~Server();
  void handle_connections();

private:
  int socket_;
  sockaddr_in address_;
  static constexpr int kBufferSize = 1024;

  
  void handle_accept(int sock);
  static void set_socket_options(int sock, int opt);
  static void set_non_blocking (int sock);
};
} // namespace tt::chat::server

#endif // CHAT_SERVER_H
