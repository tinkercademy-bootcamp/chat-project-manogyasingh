#include "Server.h"
#include "../net/chat-sockets.h"

namespace tt::chat::server {
Server::Server(int port)
    : port_(port), server_socket_fd_(create_server_socket()),
      address_(create_server_address()) {
  bind_address_to_socket();
  listen_on_socket();
  std::cout << "Server listening on port " << port_ << "\n";
  handle_connections();
}

Server::~Server() { close(server_socket_fd_); };

void Server::set_socket_options(int sock, int opt) {
  auto err_code = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                             &opt, sizeof(opt));
  tt::chat::check_error(err_code < 0, "setsockopt() error\n");
}

int Server::create_server_socket() {
  int sock = tt::chat::net::create_socket();
  Server::set_socket_options(sock, 1);
  return sock;
}

void Server::bind_address_to_socket() {
  auto err_code =
      bind(server_socket_fd_, (sockaddr *)&address_, sizeof(address_));
  tt::chat::check_error(err_code < 0, "bind failed\n");
}

void Server::listen_on_socket() {
  namespace ttc = tt::chat;
  auto err_code = listen(server_socket_fd_, 3);
  ttc::check_error(err_code < 0, "listen failed\n");
}

void Server::handle_accept(int client_socket) {
  namespace ttc = tt::chat;
  char buffer[kBufferSize] = {0};
  ssize_t read_size = read(client_socket, buffer, kBufferSize);
  ttc::check_error(read_size < 0, "Read error on client socket " +
                                      std::to_string(client_socket));
  if (read_size > 0) {
    std::cout << "Received:" << buffer << "\n";
    send(client_socket, buffer, read_size, 0);
    std::cout << "Echo message sent\n";
  } else if (read_size == 0) {
    std::cout << "Client disconnected.\n";
  } else {
    std::cerr << "Read error on client socket " << client_socket << "\n";
  }
  close(client_socket);
}

sockaddr_in Server::create_server_address() {
  sockaddr_in address = tt::chat::net::create_address(port_);
  address.sin_addr.s_addr = INADDR_ANY;
  return address;
}

void Server::handle_connections() {
  socklen_t address_size = sizeof(address_);
  while (true) {
    int client_socket =
        accept(server_socket_fd_, (sockaddr *)&address_, &address_size);
    tt::chat::check_error(client_socket < 0, "Couldn't accept the connected socket.\n");
    handle_accept(client_socket);
  }
}

} // namespace tt::chat::server