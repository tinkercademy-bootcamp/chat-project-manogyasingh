#include "xtchat-server.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace xtc::server {
Server::Server(int port)
    : server_socket_fd_(xtc::net::create_socket()),
      server_address_(xtc::net::create_address(port)),
      port_(port) {
  opt_bind_listen();
  epoll_fd_ = epoll_create1(0);
  check_error(epoll_fd_ < 0, "Couldn't make epoll socket");
  add_to_epoll(server_socket_fd_, EPOLLIN | EPOLLET);
}

Server::~Server() {
  close(epoll_fd_);
  close(server_socket_fd_);
}

// the one function to set socket options, address options,
// bind, and start listening on
// (the server socket file descriptor)
void Server::opt_bind_listen() {
  int err_code;

  // set the socket option to allow reusing address and port
  int opt = 1;
  err_code = setsockopt(server_socket_fd_, SOL_SOCKET,
                        SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  xtc::check_error(err_code < 0, "setsockopt() error\n");

  // allow any address to connect
  server_address_.sin_addr.s_addr = INADDR_ANY;

  // bind socket to the address
  err_code = bind(server_socket_fd_, (sockaddr *)&server_address_,
                  sizeof(server_address_));
  xtc::check_error(err_code < 0, "bind failed\n");

  // listen on the server fd_
  err_code = listen(server_socket_fd_, 5);
  xtc::check_error(err_code < 0, "listen failed\n");
  std::cout << "Server listening on port " << port_ << "\n";
}


void Server::add_to_epoll(int sock, uint32_t events) {
  epoll_event ev;
  ev.events = events;
  ev.data.fd = sock;

  int err_code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock, &ev);
  xtc::check_error(err_code < 0, "Could not add event to epoll");
}

void Server::remove_from_epoll(int sock) {
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock, nullptr);
}

void Server::set_non_blocking(int sock) {
  int flags = fcntl(sock, F_GETFL, 0);
  int err_code = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
  xtc::check_error(err_code < 0, "Failed to set server socket to non blocking");
}

void Server::run() {
  epoll_event events[kMaxEvents];
  while (true) {
    int num_events = epoll_wait(epoll_fd_, events, kMaxEvents, -1);
    xtc::check_error(num_events < 0, "epoll_wait error");

    for (int i = 0; i < num_events; i++) {
      if (events[i].data.fd == server_socket_fd_) {
        handle_new_connection();
      } else {
        handle_client_data(events[i].data.fd);
      }
    }
  }
}

void Server::handle_new_connection() {
  sockaddr_in client_address;
  socklen_t client_addr_len = sizeof(client_address);
  int client_socket_fd =
      accept(server_socket_fd_, (sockaddr *)&client_address, &client_addr_len);
  check_error(client_socket_fd < 0, "accept error");
  set_non_blocking(client_socket_fd);
  add_to_epoll(client_socket_fd, EPOLLIN | EPOLLET);
  SPDLOG_INFO("New connection from client fd: {}", client_socket_fd);
  // also add to the db
}

void Server::handle_client_data(int client_sock){
  char buffer[kBufferSize];
  ssize_t bytes_read = recv(client_sock, buffer, kBufferSize - 1, 0);

  if (bytes_read <= 0) {
    SPDLOG_INFO("Client fd {} disconnected", client_sock);
    remove_from_epoll(client_sock);
    close(client_sock);
    return;
  }

  buffer[bytes_read] = '\0';
  spdlog::info("Received from client fd {}: {}", client_sock, buffer);

  // Echo back to client
  ssize_t bytes_sent = send(client_sock, buffer, bytes_read, 0);
  if (bytes_sent < 0) {
    SPDLOG_ERROR("Failed to send data to client fd {}", client_sock);
  } else {
    SPDLOG_INFO("Sent echo back to {}", client_sock);
  }
}

}  // namespace xtc::server