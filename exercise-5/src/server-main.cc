#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "net/chat-sockets.h"
#include "utils.h"
#include "endpoints/Server.h"

namespace tt::chat::server {

sockaddr_in create_server_address(int port) {
  namespace ttn = tt::chat::net;
  sockaddr_in address = ttn::create_address(port);
  address.sin_addr.s_addr = INADDR_ANY;
  return address;
}

void handle_connections(int sock, sockaddr_in &address) {
  socklen_t address_size = sizeof(address);

  while (true) {
    int accepted_socket = accept(sock, (sockaddr *)&address, &address_size);
    check_error(accepted_socket < 0, "Accept error n ");
    Server::handle_accept(accepted_socket);
  }
}

} // namespace tt::chat::server

int main() {
  namespace ttc = tt::chat;
  const int kPort = 8080;

  int my_socket = ttc::server::Server::create_server_socket();
  sockaddr_in address = ttc::server::create_server_address(kPort);

  // start listening on the socket
  ttc::server::Server::bind_address_to_socket(my_socket, address);
  ttc::server::Server::listen_on_socket(my_socket);

  std::cout << "Server listening on port " << kPort << "\n";
  ttc::server::handle_connections(my_socket, address);
  close(my_socket);

  return 0;
}
