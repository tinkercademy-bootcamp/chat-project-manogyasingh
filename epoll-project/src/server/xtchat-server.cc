#include "xtchat-server.h"

#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

xtc::server::Server::Server(int port)
    : server_socket_fd_(xtc::net::create_socket()),
      server_address_(xtc::net::create_address(port)) {
  set_socket_options(server_socket_fd_, 1);
}

xtc::server::Server::~Server() {
  close(epoll_fd_);
  close(server_socket_fd_);
};

void set_socket_options(int sock_fd, int opt) {
  xtc::check_error(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                              &opt, sizeof(opt)) < 0,
                   "setsockopt() error\n");
}