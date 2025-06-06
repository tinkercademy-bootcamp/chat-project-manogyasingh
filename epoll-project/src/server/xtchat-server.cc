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
      port_(port),
      server_address_(xtc::net::create_address(port)) {
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
  SPDLOG_INFO("Server started..!");
  epoll_event events[kMaxEvents];

  while (true) {
    int nfds = epoll_wait(epoll_fd_, events, kMaxEvents, -1);
    xtc::check_error(nfds < 0, "epoll_wait error");

    for (int i = 0; i < nfds; ++i) {
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
  int sock =
      accept(server_socket_fd_, (sockaddr *)&client_address, &client_addr_len);
  check_error(sock < 0, "accept error");
  set_non_blocking(sock);
  add_to_epoll(sock, EPOLLIN | EPOLLET);

  std::string username = "user-" + std::to_string(sock);

  socket_from_username_[username] = sock;
  username_from_socket_[sock] = username;

  SPDLOG_INFO(
      "New connection from client fd: {}, Assigned temporary username: @{}",
      sock, username);

  send_to_user(sock, help_text);
}

void Server::handle_client_data(int sock) {
  char buffer[kBufferSize];
  ssize_t bytes_read = recv(sock, buffer, kBufferSize - 1, 0);

  if (bytes_read <= 0) {
    purge_user(username_from_socket_[sock]);
  }

  buffer[bytes_read] = '\0';
  spdlog::info("Received from client fd {}: {}", sock, buffer);

  std::string payload(buffer,bytes_read);

  

  //////////////////////////////////
  //////////////////////////////////
  //  This is the part where we actually do stuff with their data //
  //////////////////////////////////

  // Echo back to client
  send_to_user(sock, std::string(buffer));
}

void Server::send_to_user(int sock, std ::string payload) {
  std::string username = username_from_socket_[sock];
  send_to_user(username, payload);
}

void Server::send_to_user(std::string username, std::string payload) {
  int sock = socket_from_username_[username];
  const char *cpayload = payload.c_str();
  size_t cpl_len = payload.length();
  send(sock, cpayload, cpl_len, 0);
  SPDLOG_INFO("Sent payload to {}", sock);
}

void Server::purge_user(std::string username) {
  int sock = socket_from_username_[username];
  socket_from_username_.erase(username);
  username_from_socket_.erase(sock);
  remove_from_epoll(sock);
  close(sock);
  SPDLOG_INFO("Client @{} with fd: {} disconnected", username, sock);
  return;
}
void Server::purge_user(int sock) {
  std::string username = username_from_socket_[sock];
  socket_from_username_.erase(username);
  username_from_socket_.erase(sock);
  remove_from_epoll(sock);
  close(sock);
  SPDLOG_INFO("Client @{} with fd: {} disconnected", username, sock);
  return;
}

}  // namespace xtc::server