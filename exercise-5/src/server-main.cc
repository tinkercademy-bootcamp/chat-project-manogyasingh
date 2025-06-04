#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "endpoints/Server.h"

int main() {
  namespace ttc = tt::chat;
  const int kPort = 8080;

  int my_socket = ttc::server::Server::create_server_socket();
  sockaddr_in address = ttc::server::Server::create_server_address(kPort);

  // start listening on the socket
  ttc::server::Server::bind_address_to_socket(my_socket, address);
  ttc::server::Server::listen_on_socket(my_socket);

  std::cout << "Server listening on port " << kPort << "\n";
  ttc::server::Server::handle_connections(my_socket, address);
  close(my_socket);

  return 0;
}
