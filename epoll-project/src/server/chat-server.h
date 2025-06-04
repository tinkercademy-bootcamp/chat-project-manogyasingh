#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "../utils.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <string>
#include <unordered_map>

namespace tt::chat::server {

class ClientDataOnServer{
  int client_socket_fd_;
  std::string username;
};

class Server {
public:
  Server(int port);
  ~Server();
  void handle_connections_epoll();

private:
  int epoll_fd_;
  int server_socket_fd_;
  sockaddr_in address_;
  std::unordered_map<int, ClientDataOnServer> clients_;
  static const int kMaxEvents = 64;
  static constexpr int kBufferSize = 1024;

  void setup_epoll();
  void add_to_epoll(int sock, uint32_t events);
  void remove_from_epoll(int sock);
  void handle_accept(int sock);
  void handle_new_connection();
  void handle_existing_connection(int sock);
  void disconnect_client (int sock);

  static void set_socket_options(int sock, int opt);
  static void set_non_blocking(int sock);
};
} // namespace tt::chat::server

#endif // CHAT_SERVER_H