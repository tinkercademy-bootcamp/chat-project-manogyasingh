#include "Server.h"
#include "../net/chat-sockets.h"

namespace tt::chat::server {
void Server::set_socket_options(int sock, int opt) {
  namespace ttc = tt::chat;
  auto err_code = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                             &opt, sizeof(opt));
  ttc::check_error(err_code < 0, "setsockopt() error\n");
}
int Server::create_server_socket() {
  int sock = net::create_socket();
  Server::set_socket_options(sock, 1);
  return sock;
}
void bind_address_to_socket(int sock, sockaddr_in &address) {
  namespace ttc = tt::chat;
  auto err_code = bind(sock, (sockaddr *)&address, sizeof(address));
  ttc::check_error(err_code < 0, "bind failed\n");
}
void listen_on_socket(int sock) {
  namespace ttc = tt::chat;
  auto err_code = listen(sock, 3);
  ttc::check_error(err_code < 0, "listen failed\n");
}
} // namespace tt::chat::server
