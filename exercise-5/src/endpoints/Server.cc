#include "Server.h"
#include "../net/chat-sockets.h"

void tt::chat::server::Server::set_socket_options(int sock, int opt) {
  namespace ttc = tt::chat;
  auto err_code = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                             &opt, sizeof(opt));
  ttc::check_error(err_code < 0, "setsockopt() error\n");
}