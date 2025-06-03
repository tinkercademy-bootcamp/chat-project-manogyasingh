#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <netinet/in.h>
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
};
} // namespace tt::chat::server

#endif // CHAT_SERVER_H
